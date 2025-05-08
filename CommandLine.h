#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <string>
#include "GameOfLife.h"

/**
 * @class CommandLine
 * @brief Handles user interaction for Conway's Game of Life
 * 
 * This class provides a command-line interface for interacting with
 * the Game of Life simulation. It processes user commands and manages
 * the visualization settings.
 */
class CommandLine {
private:
    GameOfLife* world;        // Pointer to the game world
    bool printEnabled;        // Whether to print the world after each generation
    int delayTime;            // Delay time between generations in milliseconds
    bool stabilityCheckEnabled; // Whether to check for stability

    /**
     * @brief Process a single command
     * @param input Command input string
     * @return true to continue, false to exit
     */
    bool processCommand(const std::string& input);

    /**
     * @brief Run the simulation for a specified number of generations
     * @param generations Number of generations to simulate
     */
    void runSimulation(int generations);

    /**
     * @brief Print help information
     */
    void printHelp();

public:
    /**
     * @brief Constructor
     */
    CommandLine();

    /**
     * @brief Destructor
     */
    ~CommandLine();

    /**
     * @brief Main loop for command processing
     */
    void run();
};

#endif // COMMAND_LINE_H