# 2D Multiplayer Pacman-style Game (C++ / SGG) 🎮

## 📌 Project Overview
This project is a 2D multiplayer Pacman-style game developed in C++ using the SGG (Simple Game Graphics) library. The game features two controllable players navigating through a structured level, collecting items while avoiding collisions.

The system is designed using object-oriented principles and includes real-time rendering, collision detection, and game state management.

---

## 🧠 Key Features

- Two-player gameplay (keyboard-controlled)
- Real-time game loop (update/render)
- Collectible system with score tracking
- Collision detection (player–environment & player–player)
- Multiple game states (Start, Playing, Game Over, Victory)
- Object-oriented architecture

---

## 🕹️ Gameplay

- Player 1 uses:
  - `WASD` for movement
- Player 2 uses:
  - Arrow keys for movement

### Objective:
- Collect all items in the level
- Avoid colliding with the other player (collision leads to Game Over)

---

## 🏗️ Architecture

### Core Components

- **GameState**
  - Singleton class managing game flow, players, and level
  - Handles update loop and rendering

- **Level**
  - Manages game states (start, gameplay, win, game over)
  - Handles collisions and collectible logic
  - Stores environment (blocks) and collectibles

- **Player**
  - Two-player system with independent movement
  - Physics-based movement using velocity and acceleration
  - Sprite-based rendering depending on direction

- **Collectible**
  - Interactive objects collected by the player
  - Tracks collection state

- **Box**
  - Collision system using axis-aligned bounding boxes (AABB)
  - Supports collision detection for both players :contentReference[oaicite:0]{index=0}

---

## ⚙️ Technical Highlights

- **Collision System**
  - AABB collision detection
  - Separate handling for each player
  - Player-to-player collision detection

- **Game State Machine**
  - Start screen
  - Gameplay
  - Victory screen
  - Game Over screen

- **Physics & Movement**
  - Velocity-based movement
  - Friction for smooth stopping
  - Direction-based sprite system

- **Rendering**
  - Sprite-based rendering using textures
  - Background and UI screens (start, win, lose)

---

## 🛠️ Technologies Used

- C++
- SGG Graphics Library (`sgg/graphics.h`)
- Object-Oriented Programming (OOP)

---

## 🚀 How to Run

1. Open the project in Visual Studio (`.sln` file)
2. Ensure the SGG graphics library is properly configured
3. Build and run the project

The game will launch in a window and start automatically.

---
## ⚠️ Challenges Faced

- Handling collisions between two independent players
- Designing a stable collision system with multiple objects
- Managing different game states cleanly
- Synchronizing collectible logic with game progression

---

## 💡 Key Learnings

- Multiplayer game logic design
- Real-time collision detection (AABB)
- Game state management
- Object-oriented architecture in game development
- Input handling for multiple players

---

## 👨‍💻 Authors

- Antonios Rousettos
- Nikolaos Marios Fountas
