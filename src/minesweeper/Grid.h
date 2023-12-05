//
// Created by beerpsi on 12/3/2023.
//

#ifndef MINESWEEPER_GRID_H
#define MINESWEEPER_GRID_H

#include <vector>
#include "Tile.h"


class Grid {
public:
    Grid(int width, int height, int mineDensityPercentage);
    Grid(std::vector<Tile> tiles, int width, int height, int mineCount);

    [[nodiscard]] bool isInBounds(int x, int y) const;

    Tile* getTileAt(int x, int y);
    Tile* getTileAtIndex(int index);
    std::array<Tile*, 8> getNearbyTiles(int x, int y);

    int countNearbyFlags(int x, int y);

    int const width;
    int const height;
    int const tileCount;
    int const mineCount;
private:
    void createSquares();
    void generateMines();

    std::vector<Tile> tiles;
};


#endif //MINESWEEPER_GRID_H
