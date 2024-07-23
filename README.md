# OLED SNAKE
Snake game written in ESP8266 with OLED display


## Game function
The snake will move faster as it gains more points


## Build

requirements:
- [ESP8266 RTOS SDK](https://github.com/espressif/ESP8266_RTOS_SDK)
- [ESP8266 RTOS SSD1306](https://github.com/Fonger/ESP8266-RTOS-SSD1306)
- [ESP8266 RTOS FONTS](https://github.com/Fonger/ESP8266-RTOS-FONTS)

hardware requirements:
- NodeMCU (ESP8266)
- SSD1306 0.96 inch OLED display
- 4x push button
- 4x 10k or 4.7k resistor
- bread board

to build this project:
```bash
git clone "https://github.com/Empitrix/oled-snake" && cd ./oled-snake
make menuconfig  # to setup port and flash size
```
after setting up port and flash size in `menuconfig` compile it using:
```bash
make all
make flash  # to upload into ESP8266
```

### Connection

All of the keys are pull-down

- `D6` for (UP) key
- `D7` for (DOWN) key
- `D8` for (RIGHT) key
- `D5` for (LEFT) key

