//
// Created by beerpsi on 12/3/2023.
//

#include "Minesweeper.h"

#include <array>

Minesweeper::Minesweeper(int width, int height, int mineDensityPercentage)
    : grid(Grid(width, height, mineDensityPercentage)) {
}

void Minesweeper::flag(int x, int y) {
    Tile *tile = grid.getTileAt(x, y);

    if (!tile) {
        return;
    }

    if (tile->isFlagged()) {
        correctlyFlagged -= tile->hasMine();
        totalFlagged--;
    } else {
        correctlyFlagged += tile->hasMine();
        totalFlagged++;
    }

    tile->toggleFlagged();

    if (won()) {
        openAll();
    }
}

void Minesweeper::open(int x, int y) {
    Tile *tile = grid.getTileAt(x, y);

    if (!tile) {
        return;
    }

    open(tile);
}

bool Minesweeper::won() const {
    return correctlyFlagged == grid.mineCount && correctlyFlagged + opened == grid.tileCount;
}

bool Minesweeper::lost() const {
    return isLost;
}

int Minesweeper::flags() const {
    return correctlyFlagged;
}

int Minesweeper::remainingFlags() const {
    return grid.mineCount - totalFlagged;
}

void Minesweeper::open(Tile *tile) {
    if (tile->isOpened()) {
        // Some Minesweeper implementations automatically open adjacent tiles
        // if the current tile is open and is surrounded by the correct number
        // of flags. We implement this quality-of-life feature.
        if (tile->nearbyMinesCount > 0 && tile->nearbyMinesCount == grid.countNearbyFlags(tile->x, tile->y)) {
            std::array<Tile*, 8> nearbyTiles = grid.getNearbyTiles(tile->x, tile->y);

            for (Tile* t : nearbyTiles) {
                if (t && !t->isOpened()) {
                    open(t);
                }
            }
        }

        return;
    }

    if (tile->isFlagged()) {
        return;
    }

    opened++;
    tile->setOpened();

    if (tile->hasMine()) {
        isLost = true;
        openMines();
    } else if (tile->nearbyMinesCount == 0) {
        openNearby(tile, true);
    }
}

void Minesweeper::openAll() {
    for (int i = 0; i < grid.tileCount; i++) {
        Tile *tile = grid.getTileAtIndex(i);
        if (tile) {
            open(tile);
        }
    }
}

void Minesweeper::openMines() {
    for (int i = 0; i < grid.tileCount; i++) {
        Tile *tile = grid.getTileAtIndex(i);
        if (tile && tile->hasMine()) {
            opened++;
            tile->setOpened();
        }
    }
}

void Minesweeper::openNearby(Tile* tile, bool isRoot) {
    if (!tile->isOpened() || isRoot) {
        if (!isRoot) {
            opened++;
            tile->setOpened();
        }

        if (tile->nearbyMinesCount == 0) {
            std::array<Tile*, 8> nearbyTiles = grid.getNearbyTiles(tile->x, tile->y);

            for (Tile* t : nearbyTiles) {
                if (t) {
                    openNearby(t, false);
                }
            }
        }
    }
}

Grid &Minesweeper::getGrid() {
    return grid;
}
