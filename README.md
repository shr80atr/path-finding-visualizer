# 🧭 Pathfinding Visualizer

A real-time pathfinding algorithm visualizer built with **C** and **Raylib**.

Explore how different pathfinding algorithms navigate a grid, discover routes, avoid obstacles, and find the shortest path — all through an interactive graphical interface.

---

## ✨ Features

- 🎯 **Multiple Pathfinding Algorithms**
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - Dijkstra's Algorithm
  - A* Search Algorithm

- 🎨 **Real-Time Visualization**  
  Watch algorithms explore the grid step by step.

- 🧱 **Interactive Grid**  
  Create obstacles and customize the environment.

- ⏯️ **Execution Controls**
  - Start algorithm execution
  - Pause and resume execution
  - Adjust visualization speed

- 🔄 **Algorithm Selection**  
  Switch between different algorithms using the dropdown menu.

- 🛣️ **Path Reconstruction**  
  Visualize the discovered path from the starting point to the destination.

---

## 🧠 Algorithms

| Algorithm | Description |
|---|---|
| **BFS** | Explores nodes level by level and finds the shortest path in an unweighted grid. |
| **DFS** | Explores as far as possible along one branch before backtracking. |
| **Dijkstra** | Finds the shortest path using the minimum accumulated distance. |
| **A\*** | Combines traveled distance and a heuristic to efficiently search for a path. |

---

## 🛠️ Built With

- **C** — Core programming language
- **Raylib** — Graphics, rendering, and input handling
- **GCC** — Compiler

---

## 📋 Requirements

Before building the project, make sure you have:

- GCC
- Raylib
- Linux (Ubuntu recommended)
- Required development libraries for Raylib

---

## 🚀 Installation & Execution

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/PathFindingVisualizer.git
```

### 2. Navigate to the Project Directory

```bash
cd PathFindingVisualizer
```

### 3. Compile the Project

```bash
gcc -std=c11 -Wall -Wextra \
-I./include \
src/main.c \
src/core/node.c \
src/core/grid.c \
src/data_structures/queue.c \
src/data_structures/stack.c \
src/algorithms/bfs.c \
src/algorithms/dfs.c \
src/algorithms/dijkstra.c \
src/algorithms/astar.c \
src/ui/renderer.c \
src/ui/input.c \
src/ui/controls.c \
src/utils/colors.c \
-o PathfindingVisualizer \
-lraylib -lm -ldl -lpthread \
-lX11 -lXrandr -lXi -lXcursor -lXinerama
```

### 4. Run the Application

```bash
./PathfindingVisualizer
```

---

## 🎮 How to Use

1. Launch the application.
2. Set up your grid by placing obstacles.
3. Select a pathfinding algorithm from the dropdown menu.
4. Click **Start** to begin visualization.
5. Use **Pause** and **Resume** to control execution.
6. Adjust the speed slider to change the visualization speed.
7. Compare how different algorithms explore the same environment.

---

## 📁 Project Structure

```text
PathFindingVisualizer/
│
├── include/
│   ├── data_structures/
│   ├── ui/
│   ├── utils/
│   ├── astar.h
│   ├── bfs.h
│   ├── dfs.h
│   ├── dijkstra.h
│   ├── grid.h
│   ├── input.h
│   └── node.h
│
├── src/
│   ├── algorithms/
│   │   ├── astar.c
│   │   ├── bfs.c
│   │   ├── dfs.c
│   │   └── dijkstra.c
│   │
│   ├── core/
│   │   ├── grid.c
│   │   └── node.c
│   │
│   ├── data_structures/
│   │   ├── queue.c
│   │   └── stack.c
│   │
│   ├── ui/
│   │   ├── controls.c
│   │   ├── input.c
│   │   └── renderer.c
│   │
│   ├── utils/
│   │   └── colors.c
│   │
│   └── main.c
│
└── README.md
```

---

## 🎯 Project Goals

This project was created to:

- Understand how pathfinding algorithms work.
- Compare different search strategies visually.
- Practice C programming and modular software design.
- Work with graphical programming using Raylib.
- Implement fundamental data structures such as queues and stacks.

---

## 🤝 Contributing

Contributions, suggestions, and improvements are welcome!

If you find a bug or have an idea for a new feature, feel free to open an issue or submit a pull request.

---

## 📄 License

This project is open source. Add a license file to the repository to specify the terms under which others can use, modify, and distribute the project.

---

<p align="center">
  Made with ❤️ using C and Raylib
</p>
