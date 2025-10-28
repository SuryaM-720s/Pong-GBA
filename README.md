# Pong-GBA

A classic Pong game developed for the **Game Boy Advance (GBA)** handheld console using modern **C++** and the high-level **Butano Engine**.

This project was completed as **Lab 4: Pong on Game Boy Advance** for the **CSC 3210 Computer Organization Programming** course at Georgia State University.

---

##  Features

* **Core Gameplay:** A single-paddle Pong experience where the player must prevent the ball from hitting the bottom edge of the screen.
* **Collision and Physics:** Features accurate collision detection, allowing the ball to bounce off the screen's top, left, and right edges.
* **Dynamic Paddle Hit:** The ball's **horizontal speed is dynamically adjusted** based on where it strikes the paddle, increasing the challenge.
* **Visual Flair:** A unique **trace effect** to visualize the ball's movement path.
* **Scoring & State:** An active scoreboard that updates reliably with each paddle hit, and a **"Game Over"** screen which appears upon losing, offering a restart option.
* **Sound Integration:** Sound effects are included for collisions and game events, enhancing the experience.

---

##  Gameplay Preview

Here is a screenshot of the game running on the **mGBA** emulator:

(<img width="669" height="389" alt="pong_game_ss" src="https://github.com/user-attachments/assets/570b43c0-c917-40d3-aec4-e0772095ea54" />)


***Note:** Please replace `screenshot.png` with the actual image file in your repository, and update the filename in the markdown.*

---

## Technical Highlights & Learning Outcomes

This project focused on the challenges of **embedded systems programming** and optimizing code for the GBA's limited memory and processing speed.

* **GBA Resource Management:** The dotted trace effect was implemented with a strict **size limit** to prevent memory overflow, demonstrating practical resource optimization for a constrained environment.
* **Collision Detection Logic:** Successfully implemented complex real-time collision detection, including adjusting the ball's trajectory based on the hit point on the paddle.
* **Development Toolchain Mastery:** Gained hands-on experience setting up and utilizing the complete GBA development toolchain, including **devkitARM**, the **Butano Engine**, and the **mGBA** emulator.

---

##  How to Build and Run

To compile and run this game, you must have the **devkitARM** toolchain and **Butano Engine** dependencies installed on your system.

### 1. Prerequisites

* **devkitARM:** The cross-compiler and toolchain provided by devkitPro.
* **Butano Engine:** The C++ GBA engine library.
* **Make:** The build utility (`make`).
* **mGBA:** A GBA emulator (optional, but recommended for testing).

### 2. Clone the Repository and Dependencies

```bash
# 1. Clone this repository
git clone <[your_repo_url](https://github.com/SuryaM-720s/Pong-GBA.git)>
cd <Pong-GBA>

# 2. Initialize the Butano submodule (if Butano is configured as a git submodule)
git submodule update --init
