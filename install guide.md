# ESP32 Speed Measuring Device Circuit Diagram

```mermaid
flowchart LR
  subgraph PWR[Power]
    LCD5V[(LCD 5V rail)]
    V33RAIL[(3V3 rail)]
    GND[(GND)]
  end

  subgraph CTRL[ESP32 Controller]
    ESP32["ESP32 30-pin DevKit"]
  end

  subgraph LCD_BLOCK[Display]
    LCD["16x2 LCD\nHD44780, 4-bit mode"]
  end

  subgraph SENSORS[Object Detection Sensors]
    S1["IR obstacle sensor module 1"]
    S2["IR obstacle sensor module 2"]
  end

  subgraph ALERTS[Indicators and Input]
    LED["Warning LED\nwith series resistor"]
    BTN["Panel-mount momentary pushbutton\n(reset)"]
  end

  ESP32 -- "single 3V3 pin" --> V33RAIL
  ESP32 -- "single GND pin" --> GND

  LCD5V --- LCD
  V33RAIL --- S1
  V33RAIL --- S2
  GND --- LCD
  GND --- S1
  GND --- S2
  GND --- LED
  GND --- BTN

  ESP32 -- "GPIO 18 RS" --> LCD
  ESP32 -- "GPIO 19 EN" --> LCD
  ESP32 -- "GPIO 21 D4" --> LCD
  ESP32 -- "GPIO 22 D5" --> LCD
  ESP32 -- "GPIO 23 D6" --> LCD
  ESP32 -- "GPIO 25 D7" --> LCD

  ESP32 -- "GPIO 27 OUT" --> S1
  ESP32 -- "GPIO 26 OUT" --> S2
  ESP32 -- "GPIO 32" --> LED
  ESP32 -- "GPIO 33" --> BTN

  BTN --- "one terminal to GND, one to GPIO 33" --- GND
```

## Wiring Table

| Part | ESP32 Pin | Notes |
| --- | --- | --- |
| Sensor 1 VCC | 3V3 | Branch from shared 3V3 rail |
| Sensor 2 VCC | 3V3 | Branch from shared 3V3 rail |
| Sensor 1 OUT | GPIO 27 | Active LOW input |
| Sensor 2 OUT | GPIO 26 | Active LOW input |
| Reset button | GPIO 33 | Panel-mount momentary pushbutton to GND, internal pull-up enabled |
| Warning LED | GPIO 32 | Use a series resistor, about 220-330 ohm |
| LCD RS | GPIO 18 | Parallel LCD control |
| LCD EN | GPIO 19 | Parallel LCD control |
| LCD D4 | GPIO 21 | Parallel LCD data |
| LCD D5 | GPIO 22 | Parallel LCD data |
| LCD D6 | GPIO 23 | Parallel LCD data |
| LCD D7 | GPIO 25 | Parallel LCD data |
| LCD VCC | LCD 5V rail | Depends on your LCD module |
| LCD GND | GND | Common ground |

## Notes

- Keep all grounds common between the ESP32, sensors, LCD, LED, and button; one ESP32 GND pin can fan out to a shared ground rail.
- ESP32 has one 3V3 pin; split that pin to a common 3V3 rail, then feed both sensors from that rail.
- Use the panel-mount pushbutton as a simple two-terminal switch: one side to GPIO 33, the other side to GND.
- If your LCD module needs 5V power, connect it to the LCD 5V rail and verify the module is compatible with ESP32 wiring.
- If you want, I can also turn this into a breadboard-style ASCII schematic or a formal Fritzing wiring list.
