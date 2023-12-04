//
// Created by beerpsi on 12/3/2023.
//
#include <stdexcept>

#include <ncurses/ncurses.h>

#include "Game.h"
#include "graphics/ColorPair.h"
#include "graphics/Print.h"
#include "minesweeper/Minesweeper.h"
#include "utils.h"

#define PLAYFIELD_Y_OFFSET (2)
#define PLAYING_FACE ("((-.-))...")
#define WIN_FACE (" \\(^o^)/  ")
#define LOSE_FACE (" (x_x)    ")

using Color = graphics::Color;

Game::Game(int width, int height, int mineDensityPercentage) : minesweeper(Minesweeper(width, height, mineDensityPercentage)) {

}

void Game::start() {
    const time_t startTime = time_since_epoch();

    drawAll();

    time_t lastTime = currentTime;
    while (!minesweeper.won() && !minesweeper.lost()) {
        refresh();

        currentTime = time_since_epoch() - startTime;
        if (currentTime != lastTime) {
            lastTime = currentTime;
            updateTimer();
        }

        processInput(wgetch(stdscr));
        napms(20);
    }

    end();
}


void Game::processInput(int key) {
    const int prevCursorX = cursorX;
    const int prevCursorY = cursorY;
    Grid& grid = minesweeper.getGrid();

    bool shouldUpdateGrid = false;
    bool shouldUpdateFlagCount = false;

    MEVENT event;

    switch (key) {
#pragma region Movement
        case 'w':
            [[fallthrough]];
        case 'k':
            [[fallthrough]];
        case KEY_UP:
            if (cursorY > 0) { cursorY--; }
            break;
        case 's':
            [[fallthrough]];
        case 'j':
            [[fallthrough]];
        case KEY_DOWN:
            if (cursorY < grid.height - 1) { cursorY++; }
            break;
        case 'a':
            [[fallthrough]];
        case 'h':
            [[fallthrough]];
        case KEY_LEFT:
            if (cursorX > 0) { cursorX--; }
            break;
        case 'd':
            [[fallthrough]];
        case 'l':
            [[fallthrough]];
        case KEY_RIGHT:
            if (cursorX < grid.width - 1) { cursorX++; }
            break;
#pragma endregion
#pragma region Cell inputs
        case ' ':
            minesweeper.open(cursorX, cursorY);
            shouldUpdateGrid = true;
            break;
        case 'f':
            minesweeper.flag(cursorX, cursorY);
            shouldUpdateGrid = true;
            shouldUpdateFlagCount = true;
            break;
#pragma endregion
#pragma region Mouse inputs
        // TODO: make mouse inputs work
        case KEY_MOUSE:
            if (getmouse(&event) == OK) {
                // TODO: move cursor
                // TODO: open or flag cell
                if (event.y < PLAYFIELD_Y_OFFSET || event.y > grid.height + PLAYFIELD_Y_OFFSET) {
                    return;
                }

                if (event.x < 0 || event.x > grid.width * 3) {
                    return;
                }

                cursorX = event.x / 3;
                cursorY = event.y - PLAYFIELD_Y_OFFSET;

                if (event.bstate & BUTTON1_CLICKED) {
                    minesweeper.open(cursorX, cursorY);
                    shouldUpdateGrid = true;
                } else if (event.bstate & BUTTON3_CLICKED) {
                    minesweeper.flag(cursorX, cursorY);
                    shouldUpdateGrid = true;
                    shouldUpdateFlagCount = true;
                }
            }
            break;
#pragma endregion
        case KEY_RESIZE:
            clear();
            drawAll();
            break;
        default:
            return;
    }

    if (prevCursorX != cursorX || prevCursorY != cursorY) {
        moveCursorFrom(prevCursorX, prevCursorY);
    }

    if (shouldUpdateGrid) {
        drawGrid();
    }

    if (shouldUpdateFlagCount) {
        updateFlagCount();
    }
}

void Game::moveCursorFrom(int prevX, int prevY) const {
    graphics::print(prevX * 3, prevY + PLAYFIELD_Y_OFFSET, " ");
    graphics::print(prevX * 3 + 2, prevY + PLAYFIELD_Y_OFFSET, " ");

    graphics::print(cursorX * 3, cursorY + PLAYFIELD_Y_OFFSET, "[");
    graphics::print(cursorX * 3 + 2, cursorY + PLAYFIELD_Y_OFFSET, "]");

    move(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3);
}

void Game::drawGrid() {
    Grid& grid = minesweeper.getGrid();

    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            Tile* tile = grid.getTileAt(x, y);
            int cellY = y + PLAYFIELD_Y_OFFSET;
            int cellX = x * 3 + 1;

            if (tile->isOpened()) {
                if (tile->hasMine() && tile->isFlagged()) {
                    graphics::print(cellX, cellY, "?", Color::WHITE, Color::GREEN);
                } else if (tile->hasMine()) {
                    graphics::print(cellX, cellY, "*", Color::WHITE, Color::RED);
                } else if (tile->nearbyMinesCount) {
                    graphics::print(cellX, cellY, std::to_string(tile->nearbyMinesCount), getColorForMineCount(tile->nearbyMinesCount));
                } else {
                    graphics::print(cellX, cellY, " ");
                }
            } else if (tile->isFlagged()) {
                graphics::print(cellX, cellY, "?", Color::YELLOW);
            } else {
                graphics::print(cellX, cellY, "-");
            }
        }
    }
}

void Game::drawAll() {
    updateTimer();
    updateFlagCount();
    drawGrid();

    Grid& grid = minesweeper.getGrid();
    const int gridWidth = grid.width * 3;

    graphics::print((gridWidth - 11) / 2, 0, PLAYING_FACE);

    int xOffset = gridWidth + 3;

    graphics::print(xOffset, 2, "unopened: -");
    graphics::print(xOffset, 3, "opened:   (blank)");
    graphics::print(xOffset, 4, "mine:     ");
    graphics::print(xOffset + 10, 4, "*", Color::RED);
    graphics::print(xOffset, 5, "flagged:  ");
    graphics::print(xOffset + 10, 5, "?", Color::YELLOW);

    graphics::print(xOffset, 7, "up:    w / k / up arrow");
    graphics::print(xOffset, 8, "down:  s / j / down arrow");
    graphics::print(xOffset, 9, "left:  a / h / left arrow");
    graphics::print(xOffset, 10, "right: d / l / right arrow");
    graphics::print(xOffset, 11, "open:  space");
    graphics::print(xOffset, 12, "flag:  f");

    graphics::print(cursorX * 3, cursorY + PLAYFIELD_Y_OFFSET, "[");
    graphics::print(cursorX * 3 + 2, cursorY + PLAYFIELD_Y_OFFSET, "]");
    move(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3);
}

void Game::updateTimer() const {
    mvprintw(0, 0, "Time: %03lld", currentTime);
}

void Game::updateFlagCount() {
    Grid& grid = minesweeper.getGrid();
    mvprintw(0, grid.width * 3 - 11, "Flags: %03d", minesweeper.remainingFlags());
}


graphics::Color Game::getColorForMineCount(int nearbyMineCount) {
    switch (nearbyMineCount) {
        case 1:
            return Color::BLUE;
        case 2:
            return Color::GREEN;
        case 3:
            return Color::RED;
        case 4:
            return Color::BLUE;
        case 5:
            return Color::CYAN;
        case 6:
            return Color::YELLOW;
        case 7:
            return Color::RED;
        case 8:
            return Color::MAGENTA;
        default:
            throw std::runtime_error("yeah this isn't happening");
    }
}

void Game::end() {
    Grid& grid = minesweeper.getGrid();
    const int gridWidth = grid.width * 3;

    if (minesweeper.won()) {
        graphics::print((gridWidth - 11) / 2, 0, WIN_FACE);
    } else {
        graphics::print((gridWidth - 11) / 2, 0, LOSE_FACE);
    }

    while (getch() != ' ');
}
