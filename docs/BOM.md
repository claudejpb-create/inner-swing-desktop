# Inner Swing — Bill of Materials (prototype)

A desktop swing that sways with the user's breathing pace and grows with their RSA
(respiratory sinus arrhythmia), driven by a web page that reads a chest-strap heart-rate sensor.

- **Status:** working prototype (UNO + stepper, USB link tested). Bluetooth (HM-10) and battery are planned parts.
- **Prices:** approximate, in CAD, for single units from hobby retailers or Amazon.ca (2026). They vary a lot, so treat them as estimates.
- **Part numbers:** listed only where they are standard. "Generic" means many interchangeable sellers.
- **Dimensions:** in mm.

---

## 1. Electronics (as built)

| # | Part | Qty | Part number / spec | Supplier (examples) | Approx. cost |
|---|------|-----|--------------------|---------------------|--------------|
| E1 | Microcontroller board, Arduino UNO R3 compatible | 1 | Elegoo UNO R3 (used in prototype); original: Arduino A000066 | Amazon.ca, Elegoo, Arduino Store | $20 (Elegoo) / $35 (Arduino) |
| E2 | Stepper motor, 5 V, geared 1:64 | 1 | 28BYJ-48 (5 V version); mounting tabs cut off in this build | Amazon.ca, AliExpress (usually sold with E3) | $4 |
| E3 | Stepper driver board | 1 | ULN2003 driver board (28BYJ-48 kit) | Sold with E2 | incl. with E2 (kit of 1–5: $6–15) |
| E4 | Jumper wires, female–male | 6 | Generic Dupont, 20 cm | Amazon.ca | $1 (from a $8 pack) |
| E5 | USB cable, A to B | 1 | Generic "printer cable" | Comes with UNO / Amazon.ca | $0–6 |

## 2. Bluetooth (planned, parts on order)

| # | Part | Qty | Part number / spec | Supplier (examples) | Approx. cost |
|---|------|-----|--------------------|---------------------|--------------|
| B1 | Bluetooth Low Energy serial module | 1 | HM-10 (CC2541; genuine modules often sold as DSD TECH HM-10). Must be BLE; an HC-05/HC-06 will **not** work with a web page | Amazon.ca | $12 |
| B2 | Resistor 1 kΩ, ¼ W | 1 | Voltage divider for HM-10 RXD (UNO pin 3 is 5 V, HM-10 is 3.3 V) | Any | <$1 |
| B3 | Resistor 2 kΩ, ¼ W | 1 | (or 2.2 kΩ) — second half of the divider | Any | <$1 |

## 3. Power (planned, choose one route)

**Route 1: power bank (no soldering, recommended first)**

| # | Part | Qty | Part number / spec | Supplier (examples) | Approx. cost |
|---|------|-----|--------------------|---------------------|--------------|
| P1 | USB power bank with "Always On" mode | 1 | Voltaic Systems V25 (~6,700 mAh). An ordinary power bank switches off at low current and stops the swing | voltaicsystems.com (not on Amazon.ca) | US$45 + shipping |
| P2 | Short USB-A to USB-B cable, 30 cm | 1 | Generic. Use USB-C to USB-B if the power bank only has USB-C outputs | Amazon.ca | $6–10 |
| P3 | Panel-mount USB-B socket, round, with cable | 1 | Generic "USB-B panel mount connector round with cable" — used as the charging port in the box wall | Amazon.ca | $10–15 |
| P4 | Adapter, USB-B female to USB-C male | 1 | Generic. Joins P3 to the power bank's charging input (check the input type of P1 first) | Amazon.ca | $5–10 |
| P5 | Velcro strips, adhesive | 1 set | Holds the power bank under the lid | Hardware store | $5–10 |
| P6 | USB inline on/off switch cable (optional) | 1 | Generic USB-A male–female with button | Amazon.ca | $5–8 |

**Route 2: wireless charging, no plugs (later, needs soldering)**

| # | Part | Qty | Part number / spec | Supplier (examples) | Approx. cost |
|---|------|-----|--------------------|---------------------|--------------|
| P10 | LiPo charger + 5 V boost | 1 | Adafruit PowerBoost 1000C (Adafruit #2465) | Adafruit, Digi-Key, Elmwood Electronics, PiShop.ca | $30 |
| P11 | LiPo battery 3.7 V, 2,500 mAh, JST connector | 1 | Adafruit #328 | as above | $20 |
| P12 | Qi wireless receiver, 5 V | 1 | Adafruit Universal Qi Receiver (#1901) or generic | as above, Amazon.ca | $10–15 |
| P13 | Reed switch + small magnet (on/off "key") | 1 | Generic, normally open | Digi-Key, Amazon.ca | $2–4 |

## 4. Mechanical: frame, axle, seat

| # | Part | Qty | Spec / size | Supplier (examples) | Approx. cost |
|---|------|-----|-------------|---------------------|--------------|
| M1 | Wooden box (base) | 1 | Acacia box, outside 199 × 150 × 79, lid 11 thick. The lid is the swing platform | Amazon.ca (wooden storage/gift box) | $25–35 |
| M2 | Plywood stiffening plate | 1 | 178 × 129 × 12, glued under the lid. The electronics hang from it | Hardware store / offcut | $0–5 |
| M3 | Copper tube (legs) | ~0.75 m | ⅜″ OD (9.5 mm). 4 legs at 12°: wire leg 181 long; others 157 + socket-hole depth | Hardware/plumbing store (¼″ nominal copper pipe is ⅜″ OD) | $10–15 |
| M4 | Brass tube (axle) | 1 | 6 mm OD × 0.5 mm wall (5.0 ID), cut to ~154. Glued directly onto the 5 mm motor shaft | Hobby shop (K&S Precision Metals metric tube or similar) | $5–8 |
| M5 | Axle filler rod | 1 | 5 mm rod or dowel, 50 long, inside the axle where the hanger holes are | Hobby shop / offcut | $1 |
| M6 | Brass rod (hangers) | 2 | 2.5 mm × 300 mm, each cut to 175 | Hobby shop, Amazon.ca | $3–6 each |
| M7 | Hardwood for the two top blocks + motor cover | 1 blank | Each block 48 wide × 44 tall × 26 deep (motor block 23 + 3 mm cover). Profile: R18 round top, sides flared 12°. One ~350 mm strip makes ~6 swings | Lumber yard / woodworking shop (walnut planned; prototype in oak) | $12–18 per strip |
| M8 | Hardwood for seat | 1 | 52 × 22 × 5 | Offcut | $1 |
| M9 | Steel washers (ballast) | 4–8 | Large flat washers glued to the box floor so the swing doesn't tip | Hardware store | $3 |
| M10 | Neodymium magnets | 2 | ~6 × 2 mm discs, glued in the motor cover. They hold the cover onto the motor's steel case | Hardware store, Amazon.ca | $2 |

## 5. Consumables

| # | Item | Notes | Approx. cost |
|---|------|-------|--------------|
| C1 | Epoxy, 30-minute | Legs into lid and blocks, axle onto motor shaft | $10 |
| C2 | CA (super) glue | Hanger rods in axle and seat | $5 |
| C3 | Wood finish | Hardwax oil or Danish oil (tinted to match the box if desired) | $15 |
| C4 | Clear lacquer or wax for copper and brass | After steel wool (0000) | $10 |
| C5 | Paste wax | Free end of the axle in its wood bearing hole (opened to 6.5 mm) | $5 |

## 6. Optional parts

| # | Part | Notes | Approx. cost |
|---|------|-------|--------------|
| O1 | 3D-printed motor cradle | Replaces the precise blind motor pocket with a simple Ø30 through-hole. Print in black SLA resin or MJF nylon (JLC3DP, Craftcloud). STL: `inner_swing_motor_cradle.stl` | US$3–8 |
| O2 | Fit-test rings for the cradle | `inner_swing_cradle_fit_rings.stl` | US$1–2 |

## 7. Not part of the device, but required

| Item | Notes | Approx. cost |
|------|-------|--------------|
| Chest-strap heart-rate sensor with RR intervals | Any strap using the standard Bluetooth Heart Rate Service (0x180D) **with RR intervals**: Polar H10 (prototype uses an Optimal HRV-branded strap) | $100–140 |
| Phone or computer with Chrome or Edge | Web Bluetooth is needed (Android, Windows, Mac, ChromeOS). iPhone browsers do not support it | — |
| The web page | `rsa_pendulum.html` (hosted over https for phones, e.g. Netlify) | — |
| The firmware | `InnerSwing/InnerSwing.ino` (Arduino IDE) | — |

---

## Cost summary (prototype, one unit)

| Group | Approx. cost (CAD) |
|-------|---------------------|
| Electronics (as built) | $30–45 |
| Bluetooth | $13 |
| Power, Route 1 | $90–130 |
| Mechanical | $70–100 |
| Consumables | $45 (mostly reusable over several builds) |
| **Total, standalone (Bluetooth + battery)** | **≈ $250–330** |

The production estimate is separate: a single ESP32-C3 board replacing UNO + ULN2003 + HM-10, at volume. See `Inner_Swing_Cost_Model.xlsx`.

---

## Wiring (prototype)

| From | To |
|------|----|
| ULN2003 IN1, IN2, IN3, IN4 | UNO pins 8, 9, 10, 11 |
| ULN2003 + / − | UNO 5V / GND |
| 28BYJ-48 plug | ULN2003 white socket (wires run down the wire leg; plug rebuilt under the lid) |
| HM-10 VCC / GND | UNO 5V (or 3.3V, per module) / GND |
| HM-10 TXD | UNO pin 2 |
| HM-10 RXD | UNO pin 3 **through a 1 kΩ / 2 kΩ divider** (pin 3 → 1 kΩ → RXD → 2 kΩ → GND) |
| Power | USB into the UNO (from a computer, or from the power bank) |
