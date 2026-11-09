# Inner Swing Desktop

A desktop swing that moves with your breathing. It is a physical version of the Inner Swing practice: the swing sways in time with your breath, and when your heart rate rises and falls in step with it (respiratory sinus arrhythmia, or RSA), the swing grows larger. Your heart is the controller.

The idea comes from the playground swing. You don't push a swing. You match its natural rhythm, and it rises. Your body works the same way: the baroreflex sets a natural rhythm, typically 3.5 to 7 breaths per minute, and resonance breathing at that rhythm makes heart rate and breathing work together.

Built by Claude Bellefeuille, [innerswing.ca](https://innerswing.ca).

## License

**Free to build and modify for personal, educational, and non-commercial use. Commercial use requires a license from Claude Bellefeuille.**

- **Firmware and web page code** (`firmware/`, `web/`): [PolyForm Noncommercial License 1.0.0](LICENSE)
- **Hardware design, documents, and photos** (`docs/`, `photos/`): [Creative Commons Attribution-NonCommercial-ShareAlike 4.0](LICENSE-HARDWARE.md)

To sell kits, manufacture the device, or use it in a business, contact Claude Bellefeuille at **claude@innerswing.ca** for a commercial license.

This project is not a medical device. It is for wellness and education. Do not use it to diagnose or treat any condition.

## What is in this repository

```
Inner-Swing-Device/
├── firmware/     Arduino code for the swing controller (InnerSwing.ino)
├── web/          The Inner Swing web page that reads your heart rate and drives the swing
├── docs/         Bill of materials, assembly guide, templates
├── photos/       Pictures of the build
└── README.md
```

## How it works

1. **Your heart rate.** An Optimal HRV optical reader (a light-based sensor) sends your heartbeats over Bluetooth to the web page.
2. **Your RSA.** The page measures how much your heart rate rises and falls with each breath, and how closely that rise and fall follows the breathing pace.
3. **The swing.** The page sends the pace, the position in the breath, and the height to the swing over Bluetooth (or USB, for testing). The Arduino drives a stepper motor that moves the swing smoothly.
4. **The feedback.** When your RSA is strong and in step with your breathing, the swing rises higher, reaching the ring when you hit your goal.

The swing keeps a gentle resting sway at your Inner Swing rhythm, stored in the Arduino's memory, even when no one is practicing.

## Hardware

| Part | Notes |
|---|---|
| Arduino UNO R3 (or compatible) | Runs the firmware |
| 28BYJ-48 stepper motor with ULN2003 driver board | Moves the swing |
| HM-10 Bluetooth Low Energy module | Connects the swing to the web page |
| Two resistors, 1 kΩ and 2 kΩ | Voltage divider for the HM-10 receive line (see wiring) |
| Wood, copper, brass rod | Frame, uprights, and axle |
| Optimal HRV optical reader | Source of heartbeats (light-based, worn on the body, no chest strap) |

The full bill of materials and assembly guide are in `docs/`.

### Wiring

| Arduino pin | Connects to |
|---|---|
| 8, 9, 10, 11 | ULN2003 IN1, IN2, IN3, IN4 |
| 2 | HM-10 TXD |
| 3 | HM-10 RXD, through the 1 kΩ / 2 kΩ divider |
| 5 V and GND | Motor driver and HM-10 power |

**Important:** the HM-10 receive input is 3.3 V. Use the voltage divider on the Arduino-to-HM-10 line so you do not damage the module.

## Setting it up

1. Install the Arduino IDE, then install the **SoftwareSerial** and **EEPROM** libraries (both are included with the IDE).
2. Open `firmware/InnerSwing.ino` and select **Arduino UNO** and the correct port.
3. Let the swing hang straight down when you switch it on. That position is taken as the centre.
4. Upload the sketch. The Serial Monitor (9600 baud, "Newline") shows the version and the stored rhythm.
5. Open `web/` in **Chrome** or **Edge** on a computer or Android phone. Bluetooth does not work in Safari or on iPhone.
6. Turn on your Optimal HRV reader and put it on as directed, then click **Connect strap** on the page to connect it, and **Connect swing** to connect the desktop swing. Start a session.

### Serial commands

| Command | Example | Meaning |
|---|---|---|
| `S<pace>,<phase>,<height>` | `S3.5,0.4231,23.4` | Sent by the page: pace, position in the breath, height in degrees |
| `R<rhythm>` | `R3.5` | Set and store your Inner Swing rhythm (3 to 7 breaths per minute) |
| `N<degrees>` | `N1` or `N-1` | Nudge the centre if the swing sways more to one side (stored) |
| `?` | | Report the version and stored rhythm |

## Safety

- Mount the swing on a stable base, away from the edge of a desk, and keep fingers and hair clear of the moving arm.
- Keep liquids away from the electronics.
- Unplug the swing before adjusting the mechanism.

## Contributing

Ideas, fixes, and build photos are welcome. Please open an issue before large changes. By contributing, you agree that your contribution is licensed under the same terms as the part you changed.

## Credits

The swing metaphor, the Inner Swing method, and the "don't try, just allow" approach are by Claude Bellefeuille. The resonance measurements follow the approach of Lehrer, Gevirtz, Shaffer, and colleagues in HRV biofeedback research.
