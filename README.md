# Queens_and_crossword-pazzles


This repository contains a C-based implementation of two classic **Constraint Satisfaction Problems (CSP)**: the "Queens Battle" (a variant of N-Queens with regions) and a Crossword Puzzle Generator. The project demonstrates advanced usage of **recursive backtracking** algorithms and efficient memory manipulation in C.

## 🧩 Features

### 1. Queens Battle Solver
A complex variation of the classic N-Queens puzzle.
* **The Rules:**
    * Place $N$ queens on an $N \times N$ grid.
    * No two queens can share the same row, column, or diagonal.
    * **Twist:** The board is divided into irregular color regions. No two queens can share the same color region.
* **Technique:** Recursive Backtracking with rigorous pruning (Constraint checking before placement).

### 2. Crossword Generator
An algorithmic engine that fills a grid with words based on structural constraints.
* **Input:** A list of slots (position, length, direction) and a dictionary of words.
* **Logic:**
    * Iterates through slots and available words.
    * Checks for intersection conflicts between horizontal and vertical words.
    * Uses a "usage map" to allow overlapping characters without accidental deletion during backtracking.

## 🛠️ Installation & Usage

1.  Clone the repository:
    ```bash
    git clone [https://github.com/YOUR_USERNAME/CSP-Solver-Suite.git](https://github.com/YOUR_USERNAME/CSP-Solver-Suite.git)
    ```
2.  Compile the code:
    ```bash
    gcc -o solver main.c
    ```
3.  Run the application:
    ```bash
    ./solver
    ```

## 🧠 Algorithmic Concepts Demonstrated
* **Backtracking:** The core engine for both solvers, exploring potential solutions and abandoning paths that fail constraints.
* **Recursion:** Used to simplify the state management of the board during exploration.
* **Pruning:** Optimizing performance by validating constraints (Region/Adjacency) *before* diving deeper into the recursion tree.
* **Memory Management:** efficient handling of 2D arrays and string pointers.

---
*Created by [Your Name] as part of an Advanced C Programming portfolio.*
