#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>
#include <string>

/**
 * @class GameOfLife
 * @brief Implements Conway's Game of Life with a toroidal world
 * 
 * This class handles the logic for Conway's Game of Life cellular automaton.
 * It maintains the grid state and provides methods for evolution and pattern manipulation.
 * The world is toroidal, meaning the edges wrap around.
 */
class GameOfLife {
private:
    int width;                  // Width of the grid
    int height;                 // Height of the grid
    std::vector<std::vector<bool>> current; // Current generation
    std::vector<std::vector<bool>> next;    // Next generation
    std::vector<std::vector<std::vector<bool>>> history; // History of previous states for stability check

    /**
     * @brief Count live neighbors for a cell
     * @param x X coordinate
     * @param y Y coordinate
     * @return Number of live neighbors
     */
    int countNeighbors(int x, int y);

public:
    /**
     * @brief Constructor for new world with given dimensions
     * @param w Width of the world
     * @param h Height of the world
     */
    GameOfLife(int w, int h);

    /**
     * @brief Constructor for loading from file
     * @param filename File to load from
     */
    GameOfLife(const std::string& filename);

    /**
     * @brief Evolve the world one generation
     */
    void evolve();

    /**
     * @brief Check if the world is stable
     * @return true if only still lives or period-2 oscillators exist, false otherwise
     */
    bool isStable();

    /**
     * @brief Print the current state of the world
     */
    void print();

    /**
     * @brief Load world from file
     * @param filename File to load from
     * @return true if successful, false otherwise
     */
    bool load(const std::string& filename);

    /**
     * @brief Save world to file
     * @param filename File to save to
     * @return true if successful, false otherwise
     */
    bool save(const std::string& filename);

    /**
     * @brief Get cell state from 2D coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if cell is alive, false otherwise
     */
    bool getCell(int x, int y);

    /**
     * @brief Get cell state from 1D index
     * @param position 1D position in the grid
     * @return true if cell is alive, false otherwise
     */
    bool getCell(int position);

    /**
     * @brief Set cell state using 2D coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param state Cell state (true=alive, false=dead)
     */
    void setCell(int x, int y, bool state);

    /**
     * @brief Set cell state using 1D index
     * @param position 1D position in the grid
     * @param state Cell state (true=alive, false=dead)
     */
    void setCell(int position, bool state);

    /**
     * @brief Add a glider pattern
     * @param x X coordinate for top-left corner
     * @param y Y coordinate for top-left corner
     */
    void addGlider(int x, int y);

    /**
     * @brief Add a toad pattern (period-2 oscillator)
     * @param x X coordinate for top-left corner
     * @param y Y coordinate for top-left corner
     */
    void addToad(int x, int y);

    /**
     * @brief Add a beacon pattern (period-2 oscillator)
     * @param x X coordinate for top-left corner
     * @param y Y coordinate for top-left corner
     */
    void addBeacon(int x, int y);

    /**
     * @brief Add a methuselah pattern (R-pentomino)
     * @param x X coordinate for top-left corner
     * @param y Y coordinate for top-left corner
     */
    void addMethuselah(int x, int y);

    /**
     * @brief Add random patterns
     * @param count Number of random patterns to add
     */
    void addRandomPatterns(int count);

    /**
     * @brief Get world width
     * @return Width of the world
     */
    int getWidth() const;

    /**
     * @brief Get world height
     * @return Height of the world
     */
    int getHeight() const;
};

#endif // GAME_OF_LIFE_H