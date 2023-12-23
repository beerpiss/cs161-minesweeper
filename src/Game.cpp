//
// Created by beerpsi on 12/3/2023.
//
#include <stdexcept>
#include <fstream>
#include <ctime>
#include <chrono>

#include "curses.h"
#include "Game.h"

#include "graphics/ColorPair.h"
#include "graphics/Print.h"
#include "minesweeper/Minesweeper.h"
#include "utils.h"

#define ctrl(x) ((x) & 0x1f)
#define PLAYFIELD_Y_OFFSET (2)
#define PLAYING_FACE ("((-.-))...")
#define HOLDING_FACE ("((o.o))...")
#define WIN_FACE (" \\(^o^)/  ")
#define LOSE_FACE (" (x_x)    ")

using Color = graphics::Color;

Game::Game(int width, int height, int mineDensityPercentage, int difficultyId) : minesweeper(Minesweeper(width, height, mineDensityPercentage)), difficultyId(difficultyId) {
}

Game::Game(Minesweeper minesweeper, int difficultyId) : minesweeper(std::move(minesweeper)), difficultyId(difficultyId) {

}

void Game::start() {
    // If this was loaded from a previous save, currentTime > 0
    const time_t offset = currentTime;
    const time_t startTime = time_since_epoch();

    drawAll();

    time_t lastTime = currentTime;
    while (!minesweeper.won() && !minesweeper.lost()) {
        refresh();

        currentTime = time_since_epoch() - startTime + offset;
        if (currentTime != lastTime) {
            lastTime = currentTime;
            updateTimer();
        }

        if (currentTime % 30 == 0 && currentTime != 0) {
            saveGameToFile("save.mine");
        }

        processInput(wgetch(stdscr));
        napms(10);
    }

    end();
}

bool Game::won() const {
    return minesweeper.won();
}

time_t Game::getCurrentTime() const {
    return currentTime;
}


void Game::processInput(const int key) {
    const int prevCursorX = cursorX;
    const int prevCursorY = cursorY;
    Grid& grid = minesweeper.getGrid();

    bool shouldUpdateGrid = false;
    bool shouldUpdateFlagCount = false;

    MEVENT event{};

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
        case KEY_MOUSE:
            if (getmouse(&event) == OK) {
                if (event.y < PLAYFIELD_Y_OFFSET || event.y > grid.height + PLAYFIELD_Y_OFFSET) {
                    return;
                }

                if (event.x < 0 || event.x > grid.width * 3) {
                    return;
                }

                cursorX = event.x / 3;
                cursorY = event.y - PLAYFIELD_Y_OFFSET;

                if (event.bstate & BUTTON1_PRESSED && !grid.getTileAt(cursorX, cursorY)->isOpened()) {
                    updateFace(HOLDING_FACE);
                } else if (event.bstate & BUTTON1_RELEASED) {
                    updateFace(PLAYING_FACE);
                } else if (event.bstate & BUTTON1_CLICKED) {
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
#pragma region Keyboard shortcuts
        case ctrl('o'): {
            saveGameToFile("save.mine");
            break;
        }
        case ctrl('x'): {
            // TODO: Prompt the user if they haven't saved
            exit(0);
        }
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
            const Tile* tile = grid.getTileAt(x, y);
            const int cellY = y + PLAYFIELD_Y_OFFSET;
            const int cellX = x * 3 + 1;

            if (tile->isOpened()) {
                if (tile->hasMine() && tile->isFlagged()) {
                    graphics::print(cellX, cellY, "►", Color::WHITE, Color::GREEN);
                } else if (tile->hasMine()) {
                    graphics::print(cellX, cellY, "✶", Color::WHITE, Color::RED);
                } else if (tile->nearbyMinesCount) {
                    graphics::print(cellX, cellY, std::to_string(tile->nearbyMinesCount), getColorForMineCount(tile->nearbyMinesCount));
                } else {
                    graphics::print(cellX, cellY, " ");
                }
            } else if (tile->isFlagged()) {
                graphics::print(cellX, cellY, "►", Color::YELLOW);
            } else {
                graphics::print(cellX, cellY, "-");
            }
        }
    }
}

void Game::drawAll() {
    const Grid& grid = minesweeper.getGrid();
    const int gridWidth = grid.width * 3;
    const int xOffset = gridWidth + 3;

    resize_term(std::max(14, grid.height + PLAYFIELD_Y_OFFSET + 2), xOffset + 27);

    updateTimer();
    updateFlagCount();
    drawGrid();

    updateFace(PLAYING_FACE);

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

    graphics::print(xOffset, 14, "save: ctrl + o");
    graphics::print(xOffset, 15, "exit: ctrl + x");

    graphics::print(cursorX * 3, cursorY + PLAYFIELD_Y_OFFSET, "[");
    graphics::print(cursorX * 3 + 2, cursorY + PLAYFIELD_Y_OFFSET, "]");
    move(cursorY + PLAYFIELD_Y_OFFSET, cursorX * 3);
}

void Game::updateFace(const char *face) {
    const Grid& grid = minesweeper.getGrid();
    const int gridWidth = grid.width * 3;

    graphics::print((gridWidth - 11) / 2, 0, face);
}

void Game::updateTimer() const {
    mvprintw(0, 0, "Time: %03lld", currentTime);
}

void Game::updateFlagCount() {
    const Grid& grid = minesweeper.getGrid();
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
    if (minesweeper.won()) {
        updateFace(WIN_FACE);
    } else {
        updateFace(LOSE_FACE);
    }
}

Game Game::importGameState(std::istream &is) {
    std::string buf;

    if (!std::getline(is, buf)) {
        throw std::runtime_error("Truncated stream: could not get magic bytes");
    }

    if (buf != "Minesweeper Format Version 2.00") {
        throw std::runtime_error("Invalid save file.");
    }

    time_t currentTime;
    int width, height, cursorX, cursorY, difficultyId;

    is >> currentTime >> width >> height >> cursorX >> cursorY >> difficultyId;

    std::getline(is, buf);

    int mineCount = 0;
    std::vector<Tile> tiles;

    for (int i = 0; i < width * height; i++) {
        int state;
        is >> state;

        const int x = i % width;
        const int y = i / width;

        Tile tile(x, y);

        tile.nearbyMinesCount = state & 15;

        if (state & 16) {
            mineCount++;
            tile.setMine();
        }

        if (state & 32) {
            tile.setOpened();
        }

        if (state & 64) {
            tile.toggleFlagged();
        }

        tiles.push_back(tile);
    }

    const Grid grid(tiles, width, height, mineCount);
    const Minesweeper minesweeper(grid);
    Game game(minesweeper, difficultyId);

    game.currentTime = currentTime;
    game.cursorX = cursorX;
    game.cursorY = cursorY;

    return game;
}

void Game::exportGameState(std::ostream &os) {
    Grid& grid = minesweeper.getGrid();

    os << "Minesweeper Format Version 2.00\n";
    os << currentTime << ' ' << grid.width << ' ' << grid.height << ' ' << cursorX << ' ' << cursorY << ' ' << difficultyId << '\n';

    for (int i = 0; i < grid.tileCount; i++) {
        const Tile* tile = grid.getTileAtIndex(i);

        if (!tile) {
            break;
        }

        const int state = tile->nearbyMinesCount
                            | tile->hasMine() << 4
                            | tile->isOpened() << 5
                            | tile->isFlagged() << 6;

        os << state << ' ';
    }
}

void Game::saveGameToFile(const std::string& filename) {
    const Grid& grid = minesweeper.getGrid();
    const int yOffset = grid.height + PLAYFIELD_Y_OFFSET + 1;

    std::ofstream saveFile(filename);
    const std::time_t saveTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    exportGameState(saveFile);
    saveFile.close();

    mvprintw(yOffset, 1, "Saved current game at %s", std::ctime(&saveTime));
}
