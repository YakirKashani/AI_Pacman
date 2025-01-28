# Pacman Game with AI

This project is a modern take on the classic **Pacman** game, incorporating **Artificial Intelligence (AI)** to drive the behavior of the characters within the game. The game simulates a maze where Pacman moves to collect coins while avoiding ghosts that chase him. The AI-powered ghosts use advanced pathfinding techniques to chase down Pacman, creating a more engaging and challenging gameplay experience.

## Game Overview
This game is a reimagining of the **Pacman** arcade game with enhanced AI for the ghosts and pathfinding for Pacman. Pacman moves through the maze to collect coins and avoid being caught by the ghosts. The ghosts are controlled using **A\*** pathfinding, while Pacman utilizes **BFS** for optimal movement.

The game introduces strategic elements where Pacman must plan its moves to collect coins efficiently while avoiding traps set by the AI-controlled ghosts.

## Features
- **AI-Controlled Ghosts**: The ghosts use **A\*** to find the most efficient path to catch Pacman.
- **Pacman's Pathfinding**: Pacman uses **BFS** to navigate the maze and collect coins in the most efficient way.
- **Dynamic Ghost Behavior**: The ghosts adapt to Pacman's movements, making the gameplay more challenging and unpredictable.
- **Classic Gameplay**: Players observe the game from a top-down perspective as Pacman and the ghosts interact autonomously.

## AI in the Game

### **AI Algorithms Used**

This project leverages two key **Artificial Intelligence (AI)** algorithms to provide intelligent and strategic behavior for the characters in the game.

#### **1. A\* Algorithm (Ghosts' Movement)**
The **A\* search algorithm** is used to control the movement of the ghosts in the game. A\* is a popular AI pathfinding algorithm known for finding the **shortest path** between two points on a grid. The ghosts dynamically calculate the most efficient route to chase down **Pacman**, ensuring challenging and engaging gameplay. By using a combination of **actual distance** and a **heuristic** estimate of remaining distance to Pacman, the ghosts behave in an intelligent manner, adapting to the player's movements.

#### **2. Breadth-First Search (BFS) for Pacman**
**Pacman** automatically moves in the maze and uses the **Breadth-First Search (BFS)** algorithm to find the shortest path to collect nearby coins. BFS explores the game grid by evaluating all possible moves level by level, ensuring that Pacman always chooses the most optimal route to complete its goals. This pathfinding method helps Pacman efficiently collect coins while avoiding ghosts, making for a more strategic gameplay experience.

### **Why AI?**
The integration of these AI algorithms creates an exciting and dynamic gameplay environment. The ghosts use **A\*** to intelligently pursue Pacman, while Pacman’s BFS-driven movements make it a skilled and strategic player in its own right. Together, they form an adaptive and engaging interaction that challenges the user to observe and predict the movements of the characters.


![pacman](https://github.com/user-attachments/assets/32c4a6b4-3132-4f2e-8734-46e2e0ef41ee)

