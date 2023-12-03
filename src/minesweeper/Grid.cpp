//
// Created by beerpsi on 12/3/2023.
//

#include "Grid.h"

#include <random>
#include <array>

Grid::Grid(int width, int height, int mineDensityPercentage) :
    width(width), height(height), tileCount(width * height), mineCount(width * height * mineDensityPercentage / 100) {
    createSquares();
    generateMines();
}

/**
 * Initialize grid.
 */
void Grid::createSquares() {
    const int size = width * height;

    tiles.reserve(size);

    for (int i = 0; i < size; i++) {
        tiles.emplace_back(i % width, i / width);
    }
}

/**
 * Place mines across the grid, and set the proper mine counts for each tile.
 */
void Grid::generateMines() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, tileCount - 1);

    int _mines = mineCount;

    while (_mines) {
        Tile* randomTile = &tiles[dist(mt)];

        if (randomTile->hasMine()) {
            continue;
        }

        _mines--;
        randomTile->setMine();

        std::array<Tile*, 8> adjacentTiles = getNearbyTiles(randomTile->x, randomTile->y);
        for (Tile* t : adjacentTiles) {
            if (t) {
                t->nearbyMinesCount++;
            }
        }
    }
}

bool Grid::isInBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

/**
 * Retrieves the tile at the specified coordinates.
 * @param x The x position of the tile
 * @param y The y position of the tile
 * @return The tile at the specified coordinates, or `nullptr` if the location
 * is out of bounds.
 */
Tile *Grid::getTileAt(int x, int y) {
    if (!isInBounds(x, y)) {
        return nullptr;
    }

    return &tiles[y * width + x];
}

Tile *Grid::getTileAtIndex(int index) {
    if (index >= tileCount) {
        return nullptr;
    }

    return &tiles[index];
}

/**
 * Get the 8 tiles adjacent to the tile at the specified coordinates.
 * @param x The x position of the tile
 * @param y The y position of the tile
 * @return An array of 8 adjacent tiles. These may contain null references.
 */
std::array<Tile*, 8> Grid::getNearbyTiles(int x, int y) {
    std::array<Tile*, 8> adjacentTiles{};

    adjacentTiles[0] = getTileAt(x - 1, y - 1);
    adjacentTiles[1] = getTileAt(x - 1, y);
    adjacentTiles[2] = getTileAt(x - 1, y + 1);
    adjacentTiles[3] = getTileAt(x, y - 1);
    adjacentTiles[4] = getTileAt(x, y + 1);
    adjacentTiles[5] = getTileAt(x + 1, y - 1);
    adjacentTiles[6] = getTileAt(x + 1, y);
    adjacentTiles[7] = getTileAt(x + 1, y + 1);

    return adjacentTiles;
}

int Grid::countNearbyFlags(int x, int y) {
    int nearbyFlags = 0;

    for (Tile* t : getNearbyTiles(x, y)) {
        if (t && t->isFlagged()) {
            nearbyFlags++;
        }
    }

    return nearbyFlags;
}
