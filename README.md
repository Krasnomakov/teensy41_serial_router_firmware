# Echoes of Tomorrow - Serial routing Firmware

This repository contains the embedded firmware for the microcontroller that operate as a serial router in the **Echoes of Tomorrow** GLOW 2025 project.

Full-project can be found here: https://github.com/GLOW-Delta-2025

## Overview
- Written in C++
- Compiled and uploaded to microcontrollers (Teensy 4.1)
- Connects the Mac mini to the microcontrollers via serial

```
arm/
├── firmware/           # C++ firmware source
├── tests/              # Diagnostic sketches/tests
└── documentation/      # Schematics and setup guides
```

## Setup Instructions
1. Open `firmware/arm.ino` in the Arduino IDE
2. Select the correct board (Teensy 4.1)
3. Upload to the connected board

1. Generate the protocol diagram: `cd states_diagram && python3 diagram_generator.py`
2. Build the host simulator: `cd host_sim && g++ -std=c++17 -Wall -Wextra -pedantic main.cpp base_connector.cpp -o host_sim_demo`
3. Flash firmware:
   - Teensy 4.1: open `actual_code/router_protocol_bridge/teensy_router.ino` in the Arduino IDE and upload with FQBN `teensy:avr:teensy41`.
   - ESP32: open `actual_code/router_protocol_bridge/esp_router.ino`, select the appropriate ESP32 board, and upload (pins GPIO26/25 map to Teensy RX1/TX1).

## Documentation
See `documentation/` or the [Wiki](https://github.com/GLOW-Delta-2025/master/wiki) for details on architecture, function descriptions, and setup.

- Open the Teensy USB Serial monitor at 115200 baud and send protocol frames (for example `!!ARM1:REQUEST:MAKE_STAR{speed=3,color=red,brightness=80,size=10}##`).
- Watch the Teensy console for `[ROUTER]` debug lines and the echoed frames that return to the MAC side.
- Optional: connect to the ESP’s USB serial port to view its `[ESP]` logs.

## Contributing Checklist

- Keep command/state names identical to the specification tables (including underscores and capitalization).
- When adding new protocol elements, update `states_diagram/teensy_esp_router_states.md` and regenerate the diagram.
- Extend the host simulator alongside firmware changes so desktop tests cover new flows.
- Run `git status` and review the generated artifacts before committing.
