#include "GameOfLife.h"
#include <iostream>
#include <fstream>
#include <random>

// Helper function to calculate the number of live neighbors for a cell
int GameOfLife::countNeighbors(int x, int y) {
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

// Constructor for new world with given dimensions
GameOfLife::GameOfLife(int w, int h) : width(w), height(h) {
    // Initialize current and next generations
    current.resize(height, std::vector<bool>(width, false));
    next.resize(height, std::vector<bool>(width, false));
    
    // Initialize history with current state
    history.push_back(current);
}

// Constructor for loading from file
GameOfLife::GameOfLife(const std::string& filename) {
    load(filename);
}

// Evolve the world one generation
void GameOfLife::evolve() {
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
bool GameOfLife::isStable() {
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
void GameOfLife::print() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::cout << (current[y][x] ? "■" : "□") << " ";
        }
        std::cout << std::endl;
    }
}

// Load world from file
bool GameOfLife::load(const std::string& filename) {
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
bool GameOfLife::save(const std::string& filename) {
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
bool GameOfLife::getCell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    return current[y][x];
}

// Get cell state from 1D index
bool GameOfLife::getCell(int position) {
    int x = position % width;
    int y = position / width;
    return getCell(x, y);
}

// Set cell state using 2D coordinates
void GameOfLife::setCell(int x, int y, bool state) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    current[y][x] = state;
}

// Set cell state using 1D index
void GameOfLife::setCell(int position, bool state) {
    int x = position % width;
    int y = position / width;
    setCell(x, y, state);
}

// Add a glider pattern
void GameOfLife::addGlider(int x, int y) {
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
void GameOfLife::addToad(int x, int y) {
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
void GameOfLife::addBeacon(int x, int y) {
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
void GameOfLife::addMethuselah(int x, int y) {
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
void GameOfLife::addRandomPatterns(int count) {
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
int GameOfLife::getWidth() const {
    return width;
}

// Get world height
int GameOfLife::getHeight() const {
    return height;
}