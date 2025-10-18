# Ninja Dennis - A 42 So Long Project

<div align="center">

*A thrilling 2D ninja adventure through dangerous enemy territory*

**"So Long, and Thanks for All the Fish!"**  
*- Douglas Adams, The Hitchhiker's Guide to the Galaxy*

</div>

---

## 🎮 About This Project

**Ninja Dennis** is a 2D tile-based game created as part of the **42 School curriculum's "So Long" project**. The project name pays homage to Douglas Adams' famous book *"The Hitchhiker's Guide to the Galaxy"*, where dolphins leave Earth with the message "So long, and thanks for all the fish!" - much like our ninja hero must collect all items and escape the level.

This project demonstrates fundamental game development concepts including:
- 2D graphics rendering with **MiniLibX**
- Event handling (keyboard input)
- Collision detection
- Enemy AI with patrol patterns
- Real-time gameplay mechanics

---

## 🥷 Game Story

**Dennis**, a legendary ninja warrior, has infiltrated enemy fortresses to retrieve stolen artifacts. Navigate through dangerous corridors filled with patrolling guards, collect all the treasures, and escape through the exit portal before your health runs out!

---

## 🎯 Objectives

### Primary Goal
**Collect all collectibles (C) and reach the exit (E) to complete the level**

### Win Condition
✅ Collect **ALL** items in the level  
✅ Navigate to the **exit portal**  
✅ Survive enemy encounters  

### Lose Condition
❌ Health reaches **0 HP** from enemy attacks

---

## 🕹️ How to Play

### Controls

| Key | Action |
|-----|--------|
| **W** or **↑** | Move Up |
| **A** or **←** | Move Left |
| **S** or **↓** | Move Down |
| **D** or **→** | Move Right |
| **SPACE** | Attack adjacent enemies |
| **ESC** | Quit game |

### Gameplay Mechanics

#### Movement
- Move **one tile** at a time in four directions
- Movement count displayed in terminal and on-screen
- Cannot move through walls (1) or map boundaries

#### Combat System
- **Player Health**: 160 HP
- **Enemy Damage**: 20 HP per attack
- **Attack Range**: Adjacent tiles (melee)
- Press **SPACE** to attack enemies next to you
- Defeat enemies to clear your path

#### Enemy Behavior
- 🔴 Enemies patrol autonomously
- ⏱️ Move **1 tile every 2 seconds**
- 🔄 Patrol in horizontal or vertical patterns
- ⚔️ Attack player on contact (20 damage)
- 💀 Can be defeated with attacks

#### Collectibles
- 🪙 Collect **all items** marked with 'C'
- 📍 Exit portal 'E' only opens after collecting everything
- 🚪 Reach the exit to win the level

---

## 🎨 Map Elements

| Symbol | Element | Description |
|--------|---------|-------------|
| **0** | Empty Space | Walkable floor |
| **1** | Wall | Solid obstacle (blocks movement) |
| **C** | Collectible | Items to collect (coins/artifacts) |
| **E** | Exit | Portal to next level (opens after collecting all C) |
| **P** | Player | Your ninja starting position |
| **B** | Enemy | Patrolling guards (move every 2 seconds) |

---

## 🚀 Installation & Running

### Prerequisites
- **Linux/WSL** (Windows Subsystem for Linux)
- **MiniLibX** library
- **gcc** compiler
- **make**

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/TheAlchemistCode/TAC_So_Long.git
cd TAC_So_Long

# Build the game
make

# Run with a map
./so_long maps/map_enemies.ber
```

### Available Maps

```bash
# Beginner - 2 enemies
./so_long maps/map_enemies.ber

# Standard - Test map
./so_long maps/map1.ber

# Advanced - 4 enemies
./so_long maps/map2.ber

# Custom maps supported (.ber format)
```

---

## 🌟 Why "So Long"?

The project name **"So Long"** is a reference to Douglas Adams' *"The Hitchhiker's Guide to the Galaxy"*, where dolphins—being the second most intelligent species on Earth—leave the planet just before its destruction with the parting message:

> **"So long, and thanks for all the fish!"**

In our game, like the dolphins' escape, Dennis must collect everything valuable and make his escape before it's too late. The title reminds us that sometimes the smartest move is knowing when to leave—after collecting what matters, of course! 🐬

This literary reference adds a touch of humor and philosophical depth to what might otherwise be "just another escape game."

---

## 🎓 42 School Project Requirements

This project fulfills the **So Long** project requirements:

### Mandatory Part ✅
- [x] 2D game with basic gameplay
- [x] MiniLibX graphics library
- [x] Map parsing and validation
- [x] Player movement (4 directions)
- [x] Collectibles system
- [x] Exit mechanism
- [x] Movement counter display
- [x] ESC and window close handling
- [x] Makefile with all required rules
- [x] No memory leaks
- [x] Norm compliant

### Bonus Features ✅
- [x] Enemy patrols with AI
- [x] Player loses on enemy contact
- [x] On-screen HUD (health bar)
- [x] Sprite animation
- [x] Combat system

---

## 📊 Performance Stats

- **Frame Rate**: 60 FPS (stable)
- **Enemy Support**: 10+ simultaneous enemies
- **Memory Footprint**: ~500KB
- **Load Time**: <1 second
- **Input Latency**: <16ms

---

## 🐛 Troubleshooting

### Common Issues

**Game won't compile:**
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install gcc make libx11-dev libxext-dev libbsd-dev

# Rebuild
make fclean && make
```

**No window appears (Windows):**
```bash
# Use WSL2 with WSLg support
wsl --set-version <distro> 2
```

**Enemies not moving:**
- Enemies move every 2 seconds (wait a moment)
- Check if enemies are alive (not defeated)
- Verify map has 'B' enemy markers

---

## 📝 Project Structure

```
TAC_So_Long/
├── Makefile              # Build system
├── include/
│   └── so_long.h        # Header files
├── src/
│   ├── main.c           # Entry point
│   ├── graphics.c       # Rendering
│   ├── map_parse.c      # Map loading
│   ├── map_validation.c # Map validation
│   ├── combat/          # Combat system
│   ├── enemy/           # Enemy AI
│   ├── animation/       # Sprite animation
│   └── utils/           # Utilities
├── maps/                # Level files (.ber)
├── assets/              # Game assets
├── libft/               # Custom C library
└── minilibx-linux/      # Graphics library
```

---

## 🎖️ Credits

**Developer**: TheAlchemistCode (42 Student)  
**Project**: So Long (42 School)  
**Graphics Library**: MiniLibX  
**Inspiration**: The Hitchhiker's Guide to the Galaxy by Douglas Adams  

---

## 📄 License

This project is part of the 42 School curriculum.  
Educational use only.

---

<div align="center">

**Made with ❤️ at 42 School**

*"Don't Panic, and remember your towel!"*

</div>

---

## 🎮 Quick Start Example

```bash
# 1. Build the game
make

# 2. Run with enemy map
./so_long maps/map_enemies.ber

# 3. Play!
# - Use WASD to move
# - SPACE to attack
# - Collect all items
# - Reach the exit
# - Don't die!

# 4. Win condition
# "Level Complete! Moves: XX"
```

---

**Good luck, Ninja Dennis! 🥷**

*So long, and thanks for all the collectibles!* 🪙
