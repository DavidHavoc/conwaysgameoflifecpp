#include "CommandLine.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>

// Constructor
CommandLine::CommandLine() : world(nullptr), printEnabled(true), delayTime(100), stabilityCheckEnabled(true) {}

// Destructor
CommandLine::~CommandLine() {
    if (world) delete world;
}

// Process a single command
bool CommandLine::processCommand(const std::string& input) {
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
void CommandLine::runSimulation(int generations) {
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
void CommandLine::printHelp() {
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
void CommandLine::run() {
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