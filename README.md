# Conway's Game of Life in C++

This repository contains an implementation of Conway's Game of Life, a classic cellular automaton devised by mathematician John Conway. The program is written in C++ and is designed to be efficient, modular, and easy to use.

## About Conway's Game of Life

Conway's Game of Life is a zero-player game, meaning its evolution is determined by its initial state and requires no further input. It consists of a grid of cells, each of which can be alive or dead. The state of the grid evolves in steps based on the following rules:

1. **Underpopulation**: A live cell with fewer than two live neighbors dies.
2. **Survival**: A live cell with two or three live neighbors continues to live.
3. **Overpopulation**: A live cell with more than three live neighbors dies.
4. **Reproduction**: A dead cell with exactly three live neighbors becomes a live cell.

This program simulates these rules and displays the evolution of the grid over time.

---

## Features

- **Efficient Implementation**: Optimized for performance in C++.
- **Customizable Grid Size**: Define the size of the grid at runtime.
- **Interactive or Preset Patterns**: Start with manually defined patterns or use preset ones (e.g., gliders, oscillators).

---

## Getting Started

### Prerequisites

To build and run this project, you need:

- A C++ compiler (e.g., GCC, Clang, MSVC)
- [CMake](https://cmake.org/) for build automation
- A terminal or console environment

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/DavidHavoc/conwaysgameoflifecpp.git
   cd conwaysgameoflifecpp
   ```

2. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the executable:
   ```bash
   ./GameOfLife
   ```

---

If you want to enable the documentation generation or testing, you can pass those options when configuring CMake:
   ```bash  
   cmake -DBUILD_DOCS=ON -DBUILD_TESTS=ON ..
   ```
## Usage

Once the program is running:

1. You can input the grid size and choose a starting pattern.
2. The simulation will display the grid in the terminal.

## Contributing

Contributions are welcome! If you’d like to improve the project, feel free to:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Submit a pull request for review.

---

## License

This project is licensed under the MIT License.

---

## Acknowledgments

- Inspired by John Conway's original concept of the Game of Life.
- Special thanks to the open-source community for their tools and libraries.

---
