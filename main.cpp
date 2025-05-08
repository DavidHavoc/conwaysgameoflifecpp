#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include <ctime>
#include <algorithm>

// GameOfLife class - handles the game logic and world state
class GameOfLife {
private:
    int width;                  // Width of the grid
    int height;                 // Height of the grid
    std::vector<std::vector<bool>> current; // Current generation
    std::vector<std::vector<bool>> next;    // Next generation
    std::vector<std::vector<std::vector<bool>>> history; // History of previous states for stability check

    // Helper function to calculate the number of live neighbors for a cell
    int countNeighbors(int x, int y) {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue; // Skip the cell itself
                
                // Handle toroidal world by wrapping around edges
                int nx = (x + i + width) % width;
                int ny = (y + j + height) % height;
                
                if (current[ny][nx]) count++;
            }
        }
        return count;
    }

public:
    // Constructor for new world with given dimensions
    GameOfLife(int w, int h) : width(w), height(h) {
        // Initialize current and next generations
        current.resize(height, std::vector<bool>(width, false));
        next.resize(height, std::vector<bool>(width, false));
        
        // Initialize history with current state
        history.push_back(current);
    }

    // Constructor for loading from file
    GameOfLife(const std::string& filename) {
        load(filename);
    }

    // Evolve the world one generation
    void evolve() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbors = countNeighbors(x, y);
                bool isAlive = current[y][x];
                
                // Apply Conway's Game of Life rules
                if (isAlive && (neighbors < 2 || neighbors > 3)) {
                    next[y][x] = false; // Cell dies due to under/overpopulation
                } else if (!isAlive && neighbors == 3) {
                    next[y][x] = true;  // Cell is born
                } else {
                    next[y][x] = isAlive; // Cell remains the same
                }
            }
        }
        
        // Update current generation with the next generation
        current = next;
        
        // Add current state to history for stability checking
        history.push_back(current);
        
        // Keep only the last 3 generations for stability checks
        if (history.size() > 3) {
            history.erase(history.begin());
        }
    }

    // Check if the world is stable (contains only still lifes or period-2 oscillators)
    bool isStable() {
        if (history.size() < 3) return false;
        
        // Check for still lifes (current equals previous)
        if (history[1] == history[2]) {
            return true;
        }
        
        // Check for period-2 oscillators (current equals second previous)
        if (history[0] == history[2]) {
            return true;
        }
        
        return false;
    }

    // Print the current state of the world
    void print() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::cout << (current[y][x] ? "■" : "□") << " ";
            }
            std::cout << std::endl;
        }
    }

    // Load world from file
    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }
        
        file >> width >> height;
        
        // Resize and initialize grids
        current.resize(height, std::vector<bool>(width, false));
        next.resize(height, std::vector<bool>(width, false));
        
        // Read cell states
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int state;
                file >> state;
                current[y][x] = (state == 1);
            }
        }
        
        // Reset history
        history.clear();
        history.push_back(current);
        
        file.close();
        return true;
    }

    // Save world to file
    bool save(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }
        
        // Write dimensions
        file << width << " " << height << std::endl;
        
        // Write cell states
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file << (current[y][x] ? 1 : 0) << " ";
            }
            file << std::endl;
        }
        
        file.close();
        return true;
    }

    // Get cell state from 2D coordinates
    bool getCell(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return false;
        }
        return current[y][x];
    }

    // Get cell state from 1D index
    bool getCell(int position) {
        int x = position % width;
        int y = position / width;
        return getCell(x, y);
    }

    // Set cell state using 2D coordinates
    void setCell(int x, int y, bool state) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        current[y][x] = state;
    }

    // Set cell state using 1D index
    void setCell(int position, bool state) {
        int x = position % width;
        int y = position / width;
        setCell(x, y, state);
    }

    // Add a glider pattern
    void addGlider(int x, int y) {
        // Glider pattern:
        // □■□
        // □□■
        // ■■■
        setCell((x + 1) % width, y % height, true);
        setCell((x + 2) % width, (y + 1) % height, true);
        setCell(x % width, (y + 2) % height, true);
        setCell((x + 1) % width, (y + 2) % height, true);
        setCell((x + 2) % width, (y + 2) % height, true);
    }

    // Add a toad pattern (period-2 oscillator)
    void addToad(int x, int y) {
        // Toad pattern:
        // □□□□
        // □■■■
        // ■■■□
        // □□□□
        setCell((x + 1) % width, (y + 1) % height, true);
        setCell((x + 2) % width, (y + 1) % height, true);
        setCell((x + 3) % width, (y + 1) % height, true);
        setCell(x % width, (y + 2) % height, true);
        setCell((x + 1) % width, (y + 2) % height, true);
        setCell((x + 2) % width, (y + 2) % height, true);
    }

    // Add a beacon pattern (period-2 oscillator)
    void addBeacon(int x, int y) {
        // Beacon pattern:
        // ■■□□
        // ■■□□
        // □□■■
        // □□■■
        setCell(x % width, y % height, true);
        setCell((x + 1) % width, y % height, true);
        setCell(x % width, (y + 1) % height, true);
        setCell((x + 1) % width, (y + 1) % height, true);
        setCell((x + 2) % width, (y + 2) % height, true);
        setCell((x + 3) % width, (y + 2) % height, true);
        setCell((x + 2) % width, (y + 3) % height, true);
        setCell((x + 3) % width, (y + 3) % height, true);
    }

    // Add a methuselah pattern (R-pentomino)
    void addMethuselah(int x, int y) {
        // R-pentomino pattern (a long-lived methuselah):
        // □■■
        // ■■□
        // □■□
        setCell((x + 1) % width, y % height, true);
        setCell((x + 2) % width, y % height, true);
        setCell(x % width, (y + 1) % height, true);
        setCell((x + 1) % width, (y + 1) % height, true);
        setCell((x + 1) % width, (y + 2) % height, true);
    }

    // Add random patterns
    void addRandomPatterns(int count) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disX(0, width - 1);
        std::uniform_int_distribution<> disY(0, height - 1);
        std::uniform_int_distribution<> disPattern(0, 3);
        
        for (int i = 0; i < count; i++) {
            int x = disX(gen);
            int y = disY(gen);
            int pattern = disPattern(gen);
            
            switch (pattern) {
                case 0: addGlider(x, y); break;
                case 1: addToad(x, y); break;
                case 2: addBeacon(x, y); break;
                case 3: addMethuselah(x, y); break;
            }
        }
    }

    // Get world width
    int getWidth() const {
        return width;
    }

    // Get world height
    int getHeight() const {
        return height;
    }
};

// CommandLine class - handles user interaction
class CommandLine {
private:
    GameOfLife* world;
    bool printEnabled;
    int delayTime;
    bool stabilityCheckEnabled;

public:
    CommandLine() : world(nullptr), printEnabled(true), delayTime(100), stabilityCheckEnabled(true) {}

    ~CommandLine() {
        if (world) delete world;
    }

    // Process a single command
    bool processCommand(const std::string& input) {
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "exit" || command == "quit") {
            return false;
        } else if (command == "create") {
            int width, height;
            iss >> width >> height;
            if (width > 0 && height > 0) {
                if (world) delete world;
                world = new GameOfLife(width, height);
                std::cout << "Created a new world of size " << width << "x" << height << std::endl;
            } else {
                std::cout << "Invalid dimensions. Please provide positive values for width and height." << std::endl;
            }
        } else if (command == "load") {
            std::string filename;
            iss >> filename;
            if (!filename.empty()) {
                if (world) delete world;
                world = new GameOfLife(filename);
                std::cout << "Loaded world from " << filename << std::endl;
            } else {
                std::cout << "Please provide a filename." << std::endl;
            }
        } else if (command == "save") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            std::string filename;
            iss >> filename;
            if (!filename.empty()) {
                if (world->save(filename)) {
                    std::cout << "Saved world to " << filename << std::endl;
                }
            } else {
                std::cout << "Please provide a filename." << std::endl;
            }
        } else if (command == "print") {
            int value;
            iss >> value;
            printEnabled = (value != 0);
            std::cout << "Printing is now " << (printEnabled ? "enabled" : "disabled") << std::endl;
        } else if (command == "delay") {
            iss >> delayTime;
            std::cout << "Delay set to " << delayTime << " ms" << std::endl;
        } else if (command == "stability") {
            int value;
            iss >> value;
            stabilityCheckEnabled = (value != 0);
            std::cout << "Stability check is now " << (stabilityCheckEnabled ? "enabled" : "disabled") << std::endl;
        } else if (command == "run") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int generations;
            iss >> generations;
            if (generations <= 0) {
                std::cout << "Please provide a positive number of generations." << std::endl;
                return true;
            }
            
            runSimulation(generations);
        } else if (command == "set") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int arg1, arg2, arg3;
            if (iss >> arg1 >> arg2 >> arg3) {
                // 2D coordinates
                world->setCell(arg1, arg2, arg3 != 0);
                std::cout << "Set cell at (" << arg1 << ", " << arg2 << ") to " << (arg3 != 0 ? "alive" : "dead") << std::endl;
            } else {
                // Reset stream and try with 1D coordinate
                iss.clear();
                iss.seekg(0);
                iss >> command >> arg1 >> arg2;
                world->setCell(arg1, arg2 != 0);
                std::cout << "Set cell at position " << arg1 << " to " << (arg2 != 0 ? "alive" : "dead") << std::endl;
            }
        } else if (command == "get") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int arg1, arg2;
            if (iss >> arg1 >> arg2) {
                // 2D coordinates
                bool state = world->getCell(arg1, arg2);
                std::cout << "Cell at (" << arg1 << ", " << arg2 << ") is " << (state ? "alive" : "dead") << std::endl;
            } else {
                // Reset stream and try with 1D coordinate
                iss.clear();
                iss.seekg(0);
                iss >> command >> arg1;
                bool state = world->getCell(arg1);
                std::cout << "Cell at position " << arg1 << " is " << (state ? "alive" : "dead") << std::endl;
            }
        } else if (command == "glider") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int x, y;
            iss >> x >> y;
            world->addGlider(x, y);
            std::cout << "Added glider at (" << x << ", " << y << ")" << std::endl;
        } else if (command == "toad") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int x, y;
            iss >> x >> y;
            world->addToad(x, y);
            std::cout << "Added toad at (" << x << ", " << y << ")" << std::endl;
        } else if (command == "beacon") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int x, y;
            iss >> x >> y;
            world->addBeacon(x, y);
            std::cout << "Added beacon at (" << x << ", " << y << ")" << std::endl;
        } else if (command == "methuselah") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int x, y;
            iss >> x >> y;
            world->addMethuselah(x, y);
            std::cout << "Added methuselah at (" << x << ", " << y << ")" << std::endl;
        } else if (command == "random") {
            if (!world) {
                std::cout << "No world exists. Create or load a world first." << std::endl;
                return true;
            }
            
            int count;
            iss >> count;
            if (count > 0) {
                world->addRandomPatterns(count);
                std::cout << "Added " << count << " random patterns" << std::endl;
            } else {
                std::cout << "Please provide a positive number of patterns." << std::endl;
            }
        } else if (command == "help") {
            printHelp();
        } else {
            std::cout << "Unknown command. Type 'help' for a list of commands." << std::endl;
        }
        
        return true;
    }

    // Run the simulation for a specified number of generations
    void runSimulation(int generations) {
        if (!world) return;
        
        // Start time measurement
        auto start = std::chrono::high_resolution_clock::now();
        
        // Enter alternate screen mode for cleaner visualization
        if (printEnabled) {
            std::cout << "\033[?1049h";
        }
        
        bool isStable = false;
        for (int i = 0; i < generations && !isStable; i++) {
            if (printEnabled) {
                // Clear screen and move cursor to top-left
                std::cout << "\033[2J\033[H";
                std::cout << "Generation " << i + 1 << " of " << generations << std::endl;
                world->print();
                std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
            }
            
            world->evolve();
            
            // Check stability if enabled
            if (stabilityCheckEnabled && i >= 2 && world->isStable()) {
                isStable = true;
                std::cout << "World has reached a stable state after " << i + 1 << " generations." << std::endl;
            }
        }
        
        // Leave alternate screen mode
        if (printEnabled) {
            std::cout << "\033[?1049l";
        }
        
        // End time measurement
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        
        std::cout << "Simulation of " << generations << " generations completed in " 
                  << elapsed.count() << " ms" << std::endl;
        
        if (isStable) {
            std::cout << "The world reached a stable state." << std::endl;
        }
    }

    // Print help information
    void printHelp() {
        std::cout << "Conway's Game of Life - Command Line Interface" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "Available commands:" << std::endl;
        std::cout << "  create <width> <height>    - Create a new world with specified dimensions" << std::endl;
        std::cout << "  load <filename>            - Load a world from a file" << std::endl;
        std::cout << "  save <filename>            - Save the current world to a file" << std::endl;
        std::cout << "  print <0|1>                - Disable/enable printing the world after each generation" << std::endl;
        std::cout << "  delay <ms>                 - Set the delay time in milliseconds between generations" << std::endl;
        std::cout << "  stability <0|1>            - Disable/enable stability check" << std::endl;
        std::cout << "  run <n>                    - Run the simulation for n generations" << std::endl;
        std::cout << "  set <x> <y> <0|1>          - Set cell at (x,y) dead or alive" << std::endl;
        std::cout << "  set <pos> <0|1>            - Set cell at position pos dead or alive" << std::endl;
        std::cout << "  get <x> <y>                - Get state of cell at (x,y)" << std::endl;
        std::cout << "  get <pos>                  - Get state of cell at position pos" << std::endl;
        std::cout << "  glider <x> <y>             - Add a glider pattern at (x,y)" << std::endl;
        std::cout << "  toad <x> <y>               - Add a toad pattern at (x,y)" << std::endl;
        std::cout << "  beacon <x> <y>             - Add a beacon pattern at (x,y)" << std::endl;
        std::cout << "  methuselah <x> <y>         - Add a methuselah pattern at (x,y)" << std::endl;
        std::cout << "  random <n>                 - Add n random patterns to the world" << std::endl;
        std::cout << "  help                       - Display this help information" << std::endl;
        std::cout << "  exit/quit                  - Exit the program" << std::endl;
    }

    // Main loop for command processing
    void run() {
        std::cout << "Conway's Game of Life" << std::endl;
        std::cout << "Type 'help' for a list of commands." << std::endl;
        
        std::string input;
        bool running = true;
        
        while (running) {
            std::cout << "> ";
            std::getline(std::cin, input);
            running = processCommand(input);
        }
        
        std::cout << "Exiting..." << std::endl;
    }
};

// Main function to create the command line interface
int main() {
    CommandLine cli;
    cli.run();
    return 0;
}