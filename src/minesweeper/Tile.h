//
// Created by beerpsi on 12/3/2023.
//

#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H

#include <cstdint>

class Tile {
public:
    Tile(int x, int y);

    void setMine();
    [[nodiscard]] bool hasMine() const;

    void toggleFlagged();
    [[nodiscard]] bool isFlagged() const;

    void setOpened();
    [[nodiscard]] bool isOpened() const;

    const int x;
    const int y;

    uint8_t nearbyMinesCount = 0;
private:
    bool mine = false;
    bool flagged = false;
    bool opened = false;
};


#endif //MINESWEEPER_TILE_H
