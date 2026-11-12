# The Last Train - Programming Fundamentals Project (Procedural / No Structs)

CS-1002: Programming Fundamentals (AI & DS)

## Architecture Overview
This project strictly follows primitive procedural programming concepts:
- **No Structs / No OOP**: All variables, game statuses, inventory, and building rooms are stored in primitive types (`int`, `char`, `bool`) and multi-dimensional parallel arrays.
- `include/`: Modular C++ header definitions (`config.h`, `state.h`, `level.h`, `simulator.h`, `physics.h`, `switches.h`, `logger.h`, `util.h`, `viewer_iface.h`).
- `src/core/`: Modular implementation files.
- `data/`: Building map (`building.txt`) and configuration (`initial-state.txt`).
- `out/`: Runtime save states (`savegame.txt`) and session summary reports (`game_summary.txt`).
- `docs/`: Blueprint diagrams (`Project_Diagram.jpg`) and documentation.

## Build Instructions (g++ C++17)
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/core/*.cpp -o last_train
./last_train
```
