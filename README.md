# Serial Router Playground

Routing firmware, simulation scaffolding, and design documentation for the GLOW 2025 choreography protocol between the Mac controller, a Teensy 4.1 router, and ESP32-based endpoints (arms / centerpiece). The repository now includes a formal design document, acceptance & integration plan, and an updated UML state diagram.

## High-Level Layout (Simplified)

```
serial-router/
├─ .github/                              # CI/CD Workflows & Templates
│  ├─ workflows/                         # GitHub Actions (test.yml, release.yml)
│  └─ ISSUE_TEMPLATE/                    # Bug/Feature templates
├─ actual_code/
│  ├─ platformio.ini                     # PlatformIO Configuration (Native + Teensy)
│  ├─ lib/                               # Shared Libraries
│  │  └─ Protocol/                       # Protocol parsing logic (C++ agnostic)
│  ├─ test/                              # Unit Tests
│  │  └─ test_packet_parsing.cpp         # Native unit tests
│  ├─ final_serial_router_protocol_bridge/ # Production Firmware
│  │  ├─ teensy_router.ino
│  │  └─ esp_router.ino
│  └─ development/                       # Legacy/Dev sketches
├─ host_sim/                             # C++ MAC↔Router↔ESP protocol simulator (uses CommandLibary/CmdLib.h)
│  ├─ main.cpp
│  ├─ base_connector.cpp|hpp
│  ├─ mock_serial.hpp
│  └─ README.md
├─ states_diagram/
│  ├─ design/
│  │  ├─ teensy_router_states.puml       # PlantUML state machine (aws-orange theme, orthogonal)
│  │  ├─ uml_state_diagram_plan.md       # Approved plan → diagram mapping
│  │  └─ teensy_router_design.md         # Teensy router design document (timing, resources)
│  ├─ development/                       # Historic & generator assets
│  │  ├─ diagram_generator.py            # Markdown→DOT renderer for legacy diagram
│  │  ├─ teensy_esp_router_states.md     # Earlier protocol spec
│  │  └─ final_router_protocol_bridge/   # Prior v2 states + DOT output
│  └─ README.md
├─ docs/                                 # Documentation
   └─ GLOW 2025 Project Analysis & Advice.pdf
   └─ GLOW 2025_ Serial Router Firmware acceptance test & integration.pdf
   └─ GLOW 2025_ Serial Router Firmware Technical Advice.pdf 
   └─ GLOW hardware communication agreements.pdf
   └─ GLOW 2025_ Serial Router Firmware Design.pdf
   └─ GLOW 2025 Firmware & Automated test realization.pdf
├─ CONTRIBUTING.md                       # Contribution Guidelines
└─ README.md
```


## Firmware Pair (Production)
- Teensy 4.1: `actual_code/final_serial_router_protocol_bridge/teensy_router.ino`
  - Responsibilities: frame parsing, routing, address rewriting, acknowledgements (`STAR_ARRIVED`, `CLIMAX_READY`, `COMM_ERROR`).
- ESP32 debug/responder: `actual_code/final_serial_router_protocol_bridge/esp_router.ino`
  - Simulates endpoint confirmations and triggers follow-up notifications (e.g. emits `STAR_ARRIVED` after `SEND_STAR`).

### Bench Wiring
| Teensy | ESP32 | Function |
|--------|-------|----------|
| Pin 0 (RX1) | GPIO26 (TX) | ESP→Teensy data |
| Pin 1 (TX1) | GPIO25 (RX) | Teensy→ESP data |
| GND | GND | Common ground |

Baud: 115200 (USB and UART). Maintain 3.3V logic; add level shifting if any peripheral is 5V.

## Quick Start
1. Build simulation (optional desktop test):
   See `host_sim/README.md` for more. In short:
   ```bash
   cd host_sim
   # Build with CommandLibary include path
   g++ -std=c++17 -Wall -Wextra -pedantic \
     -I../../CommandLibary/CommandLibary \
     main.cpp base_connector.cpp -o host_sim_demo
   ./host_sim_demo
   ```
2. Flash Teensy firmware (Arduino IDE or PlatformIO) using board `Teensy 4.1`.
3. Flash ESP32 firmware (choose appropriate ESP32 board). Wire UART as above.
4. Open Teensy USB serial @115200 and send a frame:
   ```
   !!ARM1:REQUEST:MAKE_STAR{speed=3,color=red,brightness=80,size=10}##
   ```
5. Observe routing logs `[ROUTER] -> ESP ...` and incoming ESP confirmations.

## Performance Targets (from Design Doc)
- Parse + route overhead (T2→T4): ≤ 2 ms typical, ≤ 5 ms worst (INFO logging).
- End-to-end Mac→ESP 64-byte frame: ≤ 10 ms typical.
- Soak (100 frames/min, 10 min): 0 dropped, 0 duplicates.

## State Machine Overview
Refer to `teensy_router_states.puml` for the routing FSM: Idle → Receiving → Parsing → Routing → Forwarding → (Acknowledging if ESP-originated & ack-required) → Idle (or ErrorRecovery on fault).

## Adding / Modifying Protocol Elements
1. Update spec: add new command or notification to `states_diagram/development/teensy_esp_router_states.md` (if system-level) or adjust code comments in firmware.
2. Update state plan: modify `uml_state_diagram_plan.md` (states/events/guards).
3. Regenerate or edit UML: adjust `teensy_router_states.puml`.
4. Reflect changes in acceptance criteria (`docs/acceptance_integration_plan.md`).

## Contributing Checklist
- Maintain exact command names & capitalization.
- Keep diagram and code aligned (update both on routing/ack logic changes).
- Remove `Serial1.flush()` and introduce timestamp logging before claiming performance improvements.
- Run `git status` and commit diagram changes alongside code updates.

## Project Professionalization (Manage & Control)
To meet the Fontys HBO-i "Manage & Control" Level 3.1 KPI, this repository has been upgraded with:

### 1. Automated Testing (CI)
- **GitHub Actions**: `.github/workflows/test.yml` automatically builds the firmware and runs unit tests on every push.
- **Unit Testing**: Native C++ logic tests using `Unity` and `ArduinoFake` in `actual_code/test/`.
- **Simulation Check**: CI verifies the host simulation code compiles.

### 2. Release Management (CD)
- **Automated Releases**: Pushing a tag (e.g., `v1.0.0`) triggers `.github/workflows/release.yml`, which builds `firmware.hex` and attaches it to a GitHub Release.

### 3. Teamwork Support
- **Contribution Guidelines**: See `CONTRIBUTING.md` for git flow and commit conventions.
- **Standardized Templates**: Issue and Pull Request templates to ensure quality reporting.

### 3. Teamwork Support
- **Contribution Guidelines**: See `CONTRIBUTING.md` for git flow and commit conventions.
- **Standardized Templates**: Issue and Pull Request templates to ensure quality reporting.

## Local Development & Verification
To maintain the "Manage & Control" standards, you can run verification steps locally before pushing.

### 1. Prerequisites
- **PlatformIO**: Required for building firmware and running native tests.
  - Install via pip: `pip install platformio`
  - Or via Brew: `brew install platformio`
  - *Note*: Ensure `pio` is in your PATH.

### 2. Running Unit Tests (Software Logic)
Run the native C++ unit tests (no hardware required) to verify protocol parsing:
```bash
cd actual_code
pio test -e native
```

### 3. Verification Build (Hardware Compatibility)
Verify that the firmware compiles correctly for the Teensy 4.1:
```bash
cd actual_code
pio run -e teensy41
```

## License & Usage
- Non-commercial, no-derivatives license (see `LICENSE`). Internal experimentation and academic study permitted; distribution of modified code requires separate permission.

## Contact / Next Steps
- Pending items: timeout purge of partial frames, buffer overflow partial-drop strategy, pending entry TTL.
- After implementing performance hooks, capture empirical T1–T5 timing and integrate into acceptance reporting.

---
