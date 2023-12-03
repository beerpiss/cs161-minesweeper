//
// Created by beerpsi on 12/3/2023.
//
#include <string>

#include <ncurses/ncurses.h>

#include "Game.h"
#include "minesweeper/Minesweeper.h"
#include "utils.h"

#define PLAYFIELD_Y_OFFSET 2

const char* playingFace = "((-.-))...";
const char* wonFace = "  \\(^o^)/ ";
const char* lostFace = "   (x_x)  ";

Game::Game(int width, int height, int mineDensityPercentage) : minesweeper(Minesweeper(width, height, mineDensityPercentage)) {

}

void Game::start() {
    const time_t startTime = time_since_epoch();

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED, nullptr);

    drawAll();

    int iter = 0;
    while (!minesweeper.won() && !minesweeper.lost()) {
        iter++;
        if (iter % 50 == 0) {
            iter = 0;
            currentTime = time_since_epoch() - startTime;
            updateTimer();
        }

        processInput(getch());
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
    mvprintw(prevY + PLAYFIELD_Y_OFFSET, prevX * 3, " ");
    mvprintw(prevY + PLAYFIELD_Y_OFFSET, prevX * 3 + 2, " ");

    mvprintw(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3, "[");
    mvprintw(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3 + 2, "]");
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
                if (tile->hasMine()) {
                    mvprintw(cellY, cellX, "*");
                } else if (tile->nearbyMinesCount) {
                    mvprintw(cellY, cellX, "%d", tile->nearbyMinesCount);
                } else {
                    mvprintw(cellY, cellX, " ");
                }
            } else if (tile->isFlagged()) {
                mvprintw(cellY, cellX, "?");
            } else {
                mvprintw(cellY, cellX, "-");
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

    mvprintw(0, (gridWidth - 11) / 2, "%s", playingFace);

    int xOffset = gridWidth + 3;

    mvprintw(2, xOffset, "unopened: -");
    mvprintw(3, xOffset, "opened:   (blank)");
    mvprintw(4, xOffset, "mine:     *");
    mvprintw(5, xOffset, "flagged:  ?");

    mvprintw(7, xOffset,  "up:    w / k / up arrow");
    mvprintw(8, xOffset,  "down:  s / j / down arrow");
    mvprintw(9, xOffset,  "left:  a / h / left arrow");
    mvprintw(10, xOffset, "right: d / l / right arrow");
    mvprintw(11, xOffset, "open:  space");
    mvprintw(12, xOffset, "flag:  f");

    mvprintw(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3, "[");
    mvprintw(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3 + 2, "]");
    move(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3);
}

void Game::updateTimer() const {
    mvprintw(0, 0, "Time: %03lld", currentTime);
}

void Game::updateFlagCount() {
    Grid& grid = minesweeper.getGrid();
    mvprintw(0, grid.width * 3 - 11, "Flags: %03d", minesweeper.remainingFlags());
}

void Game::end() {
    Grid& grid = minesweeper.getGrid();
    const int gridWidth = grid.width * 3;

    if (minesweeper.won()) {
        mvprintw(0, (gridWidth - 11) / 2, "%s", wonFace);
    } else {
        mvprintw(0, (gridWidth - 11) / 2, "%s", lostFace);
    }

    while (getch() != ' ');
}
