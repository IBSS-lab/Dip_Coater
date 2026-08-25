# Dip Coater — dipcoater_1_1

Arduino firmware that drives a stepper-motor-based dip coater. The stage
(sample holder) moves up or down along a lead screw / rail, controlled over
serial by single-character commands. Motion is handled with the
[AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/) library
in constant-speed mode, and each run is automatically stopped by a safety
timer if no `stop` command is received.

## Hardware

- Arduino-compatible board (any board supported by AccelStepper / Serial)
- External 2-pin stepper driver (e.g. A4988 / DRV8825 / TB6600-style STEP/DIR
  driver)
- Stepper motor mounted on the coater's vertical axis
- Onboard LED (`LED_BUILTIN`) used as a simple "motor running" indicator

### Wiring

| Signal | Arduino pin |
|--------|-------------|
| STEP   | 7           |
| DIR    | 6           |

Configured in code as:
```cpp
AccelStepper stepper(AccelStepper::DRIVER, 7, 6);
```

## Dependencies

- [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/)
  library (install via Arduino IDE Library Manager or PlatformIO)

## Serial protocol

Open a serial connection at **9600 baud**. Send a single ASCII character to
control the stage; commands are case-insensitive.

| Command | Action                                   | Speed variable |
|---------|-------------------------------------------|-----------------|
| `u` / `U` | Move up (slow — dip/withdraw speed)     | `upSpeed` = 30    |
| `d` / `D` | Move down (fast — into solution)        | `downSpeed` = 1000 |
| `t` / `T` | Move up (fast — return to start)        | `upupSpeed` = 1000 |
| `s` / `S` | Stop                                     | 0               |
| any other | Stop (fallback / unrecognized input)    | 0               |

Speed values are set with `stepper.setSpeed()` (steps/second as configured in
`AccelStepper`); the `up`/`down` naming in the code labels them in µm/s, which
assumes a fixed steps-per-µm calibration for your specific lead screw and
microstepping setting — adjust `upSpeed`, `downSpeed`, and `upupSpeed` to
match your hardware.

The board echoes what it receives and reports timestamps over serial, e.g.:
```
I received: u
Start time: 12
```

### Auto-stop safety timer

Whenever a move command (`u`, `d`, or `t`) starts, the firmware records the
start time. If the stage is still running after `runtime` seconds (default
`1400` s ≈ 23.3 min) without an intervening `s` command, it is automatically
stopped:
```cpp
const unsigned long runtime = 1400; // s
```
Adjust this constant to match the maximum expected travel time for your rig.

## Configuration

All tunables are at the top of `dipcoater_1_1.ino`:

```cpp
const int upSpeed = 30;      // slow upward (withdraw) speed, um/s
const int downSpeed = 1000;  // downward (dip) speed, um/s
const int upupSpeed = 1000;  // fast upward (return) speed, um/s
const unsigned long runtime = 1400; // auto-stop timeout, seconds
```

`stepper.setMaxSpeed(4000)` bounds the maximum speed AccelStepper will honor;
increase it if you need speeds above 4000 steps/s.

## Build & upload

1. Install the AccelStepper library.
2. Open `dipcoater_1_1.ino` in the Arduino IDE (or use `arduino-cli` /
   PlatformIO).
3. Select your board and port, then upload.
4. Connect via Serial Monitor (or your own host application) at 9600 baud and
   send `u`, `d`, `t`, or `s` to control the stage.

## Notes / known limitations

- There is no software limit-switch or endstop handling — the auto-stop timer
  is the only built-in safety cutoff against over-travel.
- Speed units (µm/s) in the comments are nominal and depend on your stepper's
  steps/rev, microstepping, and lead screw pitch; there is no conversion
  factor in the firmware itself.
