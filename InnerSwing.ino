/*
  Inner Swing — desktop swing controller
  Board:  Arduino UNO R3 (Elegoo)
  Motor:  28BYJ-48 stepper on a ULN2003 driver board  (IN1–IN4 → pins 8, 9, 10, 11)
  Radio:  HM-10 Bluetooth Low Energy module          (TXD → pin 2, RXD ← pin 3 through a 1k/2k divider)

  What it does
  - On its own, the swing keeps a gentle resting sway at the owner's Inner Swing rhythm,
    which is stored in the UNO's memory and survives power-off.
  - When the Inner Swing page connects (Bluetooth, or USB for testing), the page sends the
    breathing pace, where it is in the breath, and how high to swing. The swing makes its own
    smooth motion from that, so Bluetooth delays never make it jerky.
  - Like a real swing, the height only changes at the ends of each arc, where the seat is
    momentarily still, and timing corrections are made by easing the speed very slightly,
    never by jumping. So the speed through each arc is always smooth.
  - If the page goes quiet for 3 seconds, the swing eases back into its resting sway.

  Messages (one per line, from the page or from the Serial Monitor at 9600 baud, "Newline"):
    S<pace>,<phase>,<height>   e.g. S3.5,0.4231,23.4   follow the page (height in degrees each side)
    R<rhythm>                  e.g. R3.5               set and store the Inner Swing rhythm (3–7 breaths/min)
    N<degrees>                 e.g. N1 or N-1          nudge the centre if the swing sways more to one side (stored)
    ?                                                  report version and stored rhythm

  Before switching on, let the seat hang straight down: that position is taken as the centre.
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>

// ---------- Settings you might change ----------
const bool  REVERSE      = true;    // true if the swing moves the opposite way to the screen
const float MAX_HEIGHT   = 55.0;    // degrees each side at most (the page sends up to this at the RSA goal)
const float REST_HEIGHT  = 12.0;    // degrees each side for the resting sway
const float DEFAULT_RHYTHM = 3.5;   // breaths per minute, until the page sets one
const bool  SAVE_POWER   = false;   // true on battery: the motor rests between steps (uses less power, a little less smooth)
                                    // false on USB power: the motor stays switched on while the swing moves (smoothest)

// ---------- Hardware ----------
const byte IN1 = 8, IN2 = 9, IN3 = 10, IN4 = 11;
SoftwareSerial bt(2, 3);                         // RX, TX  (to HM-10 TXD, RXD)
const float STEPS_PER_DEG = 4096.0 / 360.0;      // 28BYJ-48 in half-step mode
const unsigned long MIN_STEP_US = 1500;          // fastest stepping, about 10 rpm
const unsigned long RELEASE_MS = SAVE_POWER ? 8 : 3000;   // switch coils off after this long without a step
const unsigned long LINK_TIMEOUT_MS = 3000;      // page silent this long → resting sway

const byte SEQ[8][4] = {
  {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,1,0},
  {0,0,1,0}, {0,0,1,1}, {0,0,0,1}, {1,0,0,1}
};

// ---------- Memory layout (EEPROM) ----------
const int EE_MAGIC = 0, EE_RHYTHM = 1, EE_CENTRE = 5;   // magic byte, then the rhythm and the centre nudge (floats)
const byte MAGIC = 0xA7;

// ---------- State ----------
float rhythm = DEFAULT_RHYTHM;   // the owner's Inner Swing rhythm (stored)
float pace = DEFAULT_RHYTHM;     // current pace: the page's while linked, otherwise the rhythm
float phase = 0;                 // 0..1 through one breath (first half = in, second half = out)
float height = REST_HEIGHT;      // current swing height, degrees each side
float targetHeight = REST_HEIGHT;
float rateTrim = 0;              // small speed correction (cycles per second) to stay in time with the page
float halfFrom = 0, halfTo = 0;  // start and end of the current arc, degrees
float curAngle = 0;              // where the seat is now, degrees (before centre offset)
int8_t lastHalf = -1;
float centreOffset = 0;          // degrees, from N commands; stored, so it survives power-off
long  position = 0;              // motor position in steps; 0 = where the seat hung at power-up
byte  seqIndex = 0;
bool  coilsOn = false;
bool  linked = false;
unsigned long lastLoopUs, lastStepUs, lastMoveMs, lastMsgMs;

char usbLine[32], btLine[32];
byte usbLen = 0, btLen = 0;

// ---------- Helpers ----------
float easeHalf(float t) { return 0.5 - 0.5 * cos(PI * t); }


void setCoils(byte i) {
  digitalWrite(IN1, SEQ[i][0]); digitalWrite(IN2, SEQ[i][1]);
  digitalWrite(IN3, SEQ[i][2]); digitalWrite(IN4, SEQ[i][3]);
  coilsOn = true;
}

void releaseCoils() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  coilsOn = false;
}

void stepOnce(int dir) {
  seqIndex = (seqIndex + (dir > 0 ? 1 : 7)) & 7;
  setCoils(seqIndex);
  position += dir;
}

void loadRhythm() {
  if (EEPROM.read(EE_MAGIC) == MAGIC) {
    float r;
    EEPROM.get(EE_RHYTHM, r);
    float c;
    EEPROM.get(EE_CENTRE, c);
    if (c >= -20 && c <= 20) centreOffset = c;           // anything else is an empty memory: keep 0
    if (r >= 3.0 && r <= 7.0) rhythm = r;
  }
  pace = rhythm;
}

void saveRhythm(float r) {
  if (r < 3.0 || r > 7.0) return;
  if (fabs(r - rhythm) < 0.01 && EEPROM.read(EE_MAGIC) == MAGIC) return;   // unchanged: spare the memory
  rhythm = r;
  EEPROM.put(EE_RHYTHM, rhythm);
  EEPROM.update(EE_MAGIC, MAGIC);
}

void reportStatus(Stream &out) {
  out.print(F("InnerSwing 1 R"));
  out.println(rhythm, 1);
}

// ---------- Messages ----------
void handleLine(char *line, Stream &out) {
  switch (line[0]) {
    case 'S': {                                   // S<pace>,<phase>,<height>
      char *a = line + 1, *b = strchr(a, ','), *c = b ? strchr(b + 1, ',') : NULL;
      if (!b || !c) return;
      float p = atof(a), ph = atof(b + 1), h = atof(c + 1);
      if (p < 2.0 || p > 12.0 || ph < 0 || ph >= 1.0) return;
      pace = p;
      targetHeight = constrain(h, 0, MAX_HEIGHT);
      float err = ph - phase;                      // how far ahead (+) or behind (-) the page is
      if (err > 0.5) err -= 1; else if (err < -0.5) err += 1;
      if (fabs(err) > 0.25) { phase = ph; rateTrim = 0; }   // far out (just connected): jump once
      else {                                       // otherwise close the gap over about 4 s, by easing the speed
        float lim = 0.08 * p / 60.0;               //   never more than 8% faster or slower
        rateTrim = constrain(err / 4.0, -lim, lim);
      }
      linked = true;
      lastMsgMs = millis();
      break;
    }
    case 'R':                                     // R<rhythm>
      saveRhythm(atof(line + 1));
      reportStatus(out);
      break;
    case 'N':                                     // N<degrees>: nudge the centre
      centreOffset = constrain(centreOffset + atof(line + 1), -20, 20);
      EEPROM.put(EE_CENTRE, centreOffset);
      EEPROM.update(EE_MAGIC, MAGIC);
      out.print(F("OK centre ")); out.println(centreOffset, 1);
      break;
    case '?':
      reportStatus(out);
      break;
  }
}

void readStream(Stream &in, char *buf, byte &len) {
  while (in.available()) {
    char ch = in.read();
    if (ch == '\n' || ch == '\r') {
      if (len) { buf[len] = 0; handleLine(buf, in); len = 0; }
    } else if (len < 31) {
      buf[len++] = ch;
    } else {
      len = 0;                                    // too long: discard
    }
  }
}

// ---------- Setup and main loop ----------
void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  releaseCoils();
  Serial.begin(9600);
  bt.begin(9600);
  loadRhythm();
  reportStatus(Serial);
  lastLoopUs = micros();
  lastStepUs = lastLoopUs;
  lastMoveMs = millis();
}

void loop() {
  readStream(Serial, usbLine, usbLen);
  readStream(bt, btLine, btLen);

  unsigned long nowUs = micros();
  float dt = (nowUs - lastLoopUs) / 1e6;
  lastLoopUs = nowUs;
  if (dt > 0.1) dt = 0.1;

  // Page gone quiet: back to the resting sway at the stored rhythm
  if (linked && millis() - lastMsgMs > LINK_TIMEOUT_MS) { linked = false; rateTrim = 0; }
  if (!linked) {
    float t = millis() / 1000.0;
    pace = rhythm;
    targetHeight = REST_HEIGHT + 1.5 * sin(TWO_PI * t / 47.0) + 0.8 * sin(TWO_PI * t / 23.0);
  }

  // Ease the height: quickly while following the page (it already smooths), slowly on its own
  float tau = linked ? 0.4 : 2.0;
  height += (targetHeight - height) * min(1.0, dt / tau);

  // Advance through the breath
  phase += dt * (pace / 60.0 + rateTrim);
  if (phase >= 1) phase -= 1; else if (phase < 0) phase += 1;

  // Same motion as the swing on the page: eased from one side to the other on the in-breath, back on
  // the out-breath. A new height is taken up only at a turning point, where the seat is still.
  int8_t half = phase < 0.5 ? 0 : 1;
  if (half != lastHalf) {
    halfFrom = curAngle;
    halfTo = (half == 0 ? 1 : -1) * height;
    lastHalf = half;
  }
  curAngle = halfFrom + (halfTo - halfFrom) * easeHalf(half == 0 ? phase / 0.5 : (phase - 0.5) / 0.5);

  // Where the seat should be now, in motor steps
  float angle = (REVERSE ? -curAngle : curAngle) + centreOffset;   // the centre nudge stays put either way
  long target = lround(angle * STEPS_PER_DEG);

  if (target != position && nowUs - lastStepUs >= MIN_STEP_US) {
    if (!coilsOn) setCoils(seqIndex);              // re-energise where we left off before moving
    stepOnce(target > position ? 1 : -1);
    lastStepUs = nowUs;
    lastMoveMs = millis();
  } else if (coilsOn && millis() - lastMoveMs > RELEASE_MS) {
    releaseCoils();                                 // the gearbox holds the light swing in place while it rests
  }
}
