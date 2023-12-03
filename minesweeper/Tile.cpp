//
// Created by beerpsi on 12/3/2023.
//

#include "Tile.h"

Tile::Tile(int x, int y) : x(x), y(y) { }

/**
 * Declare that this tile has a mine on it.
 */
void Tile::setMine() {
    mine = true;
}

/**
 * Checks if this tile has a mine.
 * @return whether the tile has a mine.
 */
bool Tile::hasMine() const {
    return mine;
}

/**
 * Flag this tile, or remove the flag if it's already correctlyFlagged.
 */
void Tile::toggleFlagged() {
    flagged = !flagged;
}

/**
 * Check whether the tile has a flag.
 * @return
 */
bool Tile::isFlagged() const {
    return flagged;
}

/**
 * Irreversibly open this tile.
 */
void Tile::setOpened() {
    opened = true;
}

/**
 * Check whether the tile is opened.
 * @return
 */
bool Tile::isOpened() const {
    return opened;
}
