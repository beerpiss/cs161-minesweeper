//
// Created by beerpsi on 12/3/2023.
//

#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include "minesweeper/Minesweeper.h"

class Game {
public:
    Game(int width, int height, int mineDensityPercentage);
    void start();
private:
    Minesweeper minesweeper;

    time_t currentTime = 0;

    int cursorX = 0;
    int cursorY = 0;

    void processInput(int key);
    void moveCursorFrom(int prevX, int prevY) const;
    void drawAll();
    void drawGrid();
    void updateTimer() const;
    void updateFlagCount();
    void end();
};


#endif //MINESWEEPER_GAME_H
