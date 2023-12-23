//
// Created by beerpsi on 12/3/2023.
//

#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include <iostream>
#include "graphics/ColorPair.h"
#include "graphics/Graphics.h"
#include "minesweeper/Minesweeper.h"

class Game {
public:
    Game(int width, int height, int mineDensityPercentage, int difficultyId);
    Game(Minesweeper minesweeper, int difficultyId);

    static Game importGameState(std::istream& is);
    void start();

    [[nodiscard]] bool won() const;
    [[nodiscard]] time_t getCurrentTime() const;

    const int difficultyId;
private:
    Minesweeper minesweeper;

    time_t currentTime = 0;

    int cursorX = 0;
    int cursorY = 0;

    void processInput(int key);
    void moveCursorFrom(int prevX, int prevY) const;
    void drawAll();
    void drawGrid();
    void updateFace(const char* face);
    void updateTimer() const;
    void updateFlagCount();
    static graphics::Color getColorForMineCount(int nearbyMineCount);
    void end();
    void exportGameState(std::ostream& os);
    void saveGameToFile(const std::string& filename);
};


#endif //MINESWEEPER_GAME_H
