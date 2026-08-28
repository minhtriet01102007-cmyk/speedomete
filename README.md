# ESP32 Speed Meter

Project code for measuring the speed of an object using ESP32, 2 object sensors, a reset button, a warning LED, and a 16x2 LCD.

See [circuit-diagram.md](circuit-diagram.md) for a visual wiring diagram.

## Assumptions

- The LCD is a 16x2 HD44780-compatible module in 4-bit parallel mode.
- The object sensors output a digital signal and are active LOW, which is common for IR obstacle sensors.

## Default Wiring

- Sensor 1 signal -> GPIO 27
- Sensor 2 signal -> GPIO 26
- Reset button -> GPIO 33 to GND
- Warning LED -> GPIO 32 through a resistor
- LCD RS -> GPIO 18
- LCD EN -> GPIO 19
- LCD D4 -> GPIO 5
- LCD D5 -> GPIO 17
- LCD D6 -> GPIO 16
- LCD D7 -> GPIO 4
 - LCD RS -> GPIO 18
 - LCD EN -> GPIO 19
 - LCD D4 -> GPIO 21
 - LCD D5 -> GPIO 22
 - LCD D6 -> GPIO 23
 - LCD D7 -> GPIO 25
- LCD VCC -> 5V
- LCD GND -> GND

## Notes

- If your LCD wiring uses a different pinout, update the LCD pin constants in the sketch to match it.
- Change `SENSOR_DISTANCE_M` to the real distance between the two sensors.
- Change `SPEED_LIMIT_MPS` to the speed threshold you want for the alarm.
- If your sensors are active HIGH, set `SENSOR_ACTIVE_LOW` to `false`.

## Behavior

1. When sensor 1 detects the object, timing starts.
2. When sensor 2 detects the object, timing stops.
3. The ESP32 calculates speed using `v = s / t`.
4. The speed is shown on the LCD.
5. If the speed is greater than the limit, the LED turns on.
6. Press the reset button to clear the result and prepare for the next measurement.
