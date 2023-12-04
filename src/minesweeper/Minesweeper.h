//
// Created by beerpsi on 12/3/2023.
//

#ifndef MINESWEEPER_MINESWEEPER_H
#define MINESWEEPER_MINESWEEPER_H

#include <vector>
#include "Grid.h"
#include "Tile.h"


class Minesweeper {
public:
    Minesweeper(int width, int height, int mineDensityPercentage);
    explicit Minesweeper(Grid grid);

    void flag(int x, int y);
    void open(int x, int y);

    [[nodiscard]] bool won() const;
    [[nodiscard]] bool lost() const;
    [[nodiscard]] int flags() const;
    [[nodiscard]] int remainingFlags() const;

    Grid& getGrid();
private:
    void open(Tile *tile);
    void openAll();
    void openMines();
    void openNearby(Tile* tile, bool isRoot);

    Grid grid;

    bool isLost = false;

    int correctlyFlagged = 0;
    int totalFlagged = 0;
    int opened = 0;
};


#endif //MINESWEEPER_MINESWEEPER_H
