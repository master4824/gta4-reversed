# gta4-reversed

[![Build ASI (x86)](https://github.com/master4824/gta4-reversed/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/master4824/gta4-reversed/actions/workflows/build.yml)

An open-source C++20 reverse engineering, documentation, and runtime reimplementation project targeting **Grand Theft Auto IV (`1.0.7.0` x86)** — inspired by [`gta-reversed`](https://github.com/gta-reversed/gta-reversed).

The long-term goal is to document every engine/game class and incrementally reimplement `GTAIV.exe` (`43,740` functions) in clean, verified C++20 (`0` `Core::Hook::Call` fallbacks). The project builds into `gta4_reversed.asi` and hooks reversed RAGE engine and game classes at runtime, allowing live replacement and verification inside a real copy of `GTAIV.exe` without modifying original game files on disk.

> **Disclaimer:** This repository contains **no copyrighted game assets, binaries, or proprietary data**. It is strictly an open-source research and runtime reimplementation effort and requires a legitimate copy of Grand Theft Auto IV (`1.0.7.0`) to run.
---

## Current Progress

See [CLASS_MAP.md](CLASS_MAP.md) for the full IDA RTTI/vtable coverage map and per-module `Hook::Call` dependency status.

- **Target Binary:** `GTAIV.exe` `1.0.7.0` (x86, `43,740` functions)
- **Hooked Functions:** `1,332` implemented (`~3.05%`)
- **Pure C++ Modules (`0` `Core::Hook::Call` fallbacks):**
  - `Core`: `Matrix.cpp`, `Pool.cpp`, `Vector.cpp`
  - `Game`: `General.cpp`, `IplStore.cpp`, `LoadingScreens.cpp`
  - `Rage`: `phBound.cpp`, `phBoundComposite.cpp`, `phCollider.cpp`, `phInst.cpp`, `phArticulatedCollider.cpp`, `phConstraint.cpp`, `phLevelNew.cpp`

### Module Breakdown

| Module | File | Hooked Functions | Remaining `Hook::Call` | Notes |
| --- | --- | ---: | ---: | --- |
| Core | `source/Core/Matrix.cpp` | 1 | 0 | Pure C++ |
| Core | `source/Core/Pool.cpp` | 9 | 0 | Pure C++ |
| Core | `source/Core/Vector.cpp` | 2 | 0 | Pure C++ |
| Game | `source/Game/Building.cpp` | 5 | 3 | |
| Game | `source/Game/Cam.cpp` | 14 | 9 | |
| Game | `source/Game/Camera.cpp` | 15 | 6 | |
| Game | `source/Game/Cheat.cpp` | 40 | 21 | |
| Game | `source/Game/Clock.cpp` | 10 | 2 | |
| Game | `source/Game/DummyObject.cpp` | 8 | 1 | |
| Game | `source/Game/DynamicEntity.cpp` | 40 | 88 | |
| Game | `source/Game/Entity.cpp` | 85 | 173 | |
| Game | `source/Game/Explosion.cpp` | 10 | 6 | |
| Game | `source/Game/FileMgr.cpp` | 14 | 3 | |
| Game | `source/Game/Font.cpp` | 71 | 26 | |
| Game | `source/Game/Game.cpp` | 7 | 29 | |
| Game | `source/Game/Garages.cpp` | 13 | 5 | |
| Game | `source/Game/General.cpp` | 4 | 0 | Pure C++ |
| Game | `source/Game/IplStore.cpp` | 15 | 0 | Pure C++ |
| Game | `source/Game/LoadingScreens.cpp` | 1 | 0 | Pure C++ (intro skip enabled) |
| Game | `source/Game/ModelInfo.cpp` | 11 | 1 | |
| Game | `source/Game/Object.cpp` | 4 | 10 | |
| Game | `source/Game/Pad.cpp` | 14 | 1 | |
| Game | `source/Game/Ped.cpp` | 159 | 534 | |
| Game | `source/Game/PedAudio.cpp` | 79 | 371 | |
| Game | `source/Game/PedData.cpp` | 29 | 36 | |
| Game | `source/Game/PedIntelligence.cpp` | 78 | 197 | |
| Game | `source/Game/PedMoveBlend.cpp` | 45 | 284 | `ProcessMovement` (`0xA3A200`) currently disabled |
| Game | `source/Game/Physical.cpp` | 62 | 208 | |
| Game | `source/Game/Pickups.cpp` | 10 | 12 | |
| Game | `source/Game/PlayerInfo.cpp` | 34 | 120 | |
| Game | `source/Game/Radar.cpp` | 25 | 63 | |
| Game | `source/Game/Stats.cpp` | 19 | 8 | |
| Game | `source/Game/Streaming.cpp` | 19 | 7 | |
| Game | `source/Game/Timer.cpp` | 10 | 16 | |
| Game | `source/Game/TxdStore.cpp` | 23 | 3 | |
| Game | `source/Game/Vehicle.cpp` | 170 | 380 | |
| Game | `source/Game/Wanted.cpp` | 10 | 8 | |
| Game | `source/Game/WeaponInfo.cpp` | 70 | 139 | |
| Game | `source/Game/Weather.cpp` | 5 | 9 | |
| Game | `source/Game/World.cpp` | 4 | 3 | `CPhysicsWorld::InjectHooks()` currently disabled in `CWorld::InjectHooks()` |
| Game | `source/Game/Zones.cpp` | 8 | 10 | |
| Rage | `source/Rage/fiStream.cpp` | 7 | 6 | |
| Rage | `source/Rage/phArchetype.cpp` | 15 | 6 | |
| Rage | `source/Rage/phArticulatedCollider.cpp` | 4 | 0 | Pure C++ |
| Rage | `source/Rage/phBound.cpp` | 4 | 0 | Pure C++ |
| Rage | `source/Rage/phBoundBVH.cpp` | 8 | 5 | |
| Rage | `source/Rage/phBoundComposite.cpp` | 10 | 0 | Pure C++ |
| Rage | `source/Rage/phCollider.cpp` | 9 | 0 | Pure C++ |
| Rage | `source/Rage/phConstraint.cpp` | 5 | 0 | Pure C++ |
| Rage | `source/Rage/phInst.cpp` | 8 | 0 | Pure C++ |
| Rage | `source/Rage/phLevelNew.cpp` | 3 | 0 | Pure C++ |
| Rage | `source/Rage/phSimulator.cpp` | 1 | 1 | |

---

## Compatibility & Diagnostics (`dllmain.cpp`, `Tools/`)

- **Complete Edition Compatibility on `1.0.7.0`** *(for downgraded Complete Edition installs)*:
  - Multi-WTD DLC radio HUD logo fix
  - Stubbing missing Complete Edition script natives (e.g., `0x2C89A1F9` / `SCRT70`)
- **PDB-Backed VEH Crash Handler (`dllmain.cpp`):**
  - Resolves `gta4_reversed.pdb` symbols and source file/line numbers via `dbghelp.dll` (`StackWalk64` + raw `ESP` dump + `Access Violation` read/write details).
- **In-Game Test Menu (`source/Tools/DebugMenu.*`) & `[TEST]` Logger (`Core::Log::InfoOnce`):**
  - Dear ImGui D3D9 overlay (`F10` / `Insert` toggle, mouse-controlled with DirectInput8/WndProc isolation) with `World & Time`, `Player & Gameplay`, and `Test Events Log` tabs.

---

## Repository Structure

```text
source/
├── Core/        # Hook engine, logging (InfoOnce), math (Vector/Matrix), Pool helpers
├── Game/        # GTA IV game classes (CEntity, CPed, CVehicle, CCam, CWeaponInfo, CWorld, ...)
├── Rage/        # RAGE physics engine (phBound*, phArchetype, phInst, phCollider*, phLevelNew, ...)
├── Tools/       # In-game Dear ImGui D3D9 DebugMenu
└── dllmain.cpp  # ASI entry point, PDB VEH crash logger, compatibility & hook registration
third_party/
└── imgui/       # Vendored Dear ImGui (Win32 + D3D9 backends)
```

---

## Building

### Requirements
- Windows (x86 target)
- MSVC x86 C++20 toolchain (Visual Studio / Build Tools or Portable MSVC)
- CMake `3.20+` and Ninja (or Visual Studio generator)
- No external system dependencies required (`third_party/imgui` is vendored)

### Build Steps

You can run `build.bat` directly, or build manually from an **x86 Native Tools Command Prompt**:

```bat
cmake -S . -B build/Release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release
```

The compiled output is written to:
- `bin/Release/gta4_reversed.asi`
- `bin/Release/gta4_reversed.pdb`

### Running in Game
1. Make sure you have a legitimate GTA IV `1.0.7.0` installation with an ASI loader (e.g., Ultimate ASI Loader) and **ZolikaPatch** installed.
2. In `ZolikaPatch.ini`, **enable `LaunchWithPlayGTAIV=1` and `SkipLauncher=1`** (both must be enabled so `1.0.7.0` launches cleanly without base-game `RMN60` / `DFA` startup errors, which are unrelated to `gta4_reversed.asi`). Keep all other optional `ZolikaPatch` features disabled (`=0`) to avoid hook conflicts.
3. Copy `bin/Release/gta4_reversed.asi` (and optionally `.pdb` for crash stack traces) into your GTA IV root folder.
4. Launch the game (`PlayGTAIV.exe` / `GTAIV.exe`) and check `gta4_reversed.log` for initialization and any VEH diagnostics.

---

## Contributing

Contributions are very welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for coding conventions, hooking rules, struct verification guidelines, and good starting areas.
