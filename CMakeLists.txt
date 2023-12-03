cmake_minimum_required(VERSION 3.26)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-DNCURSES_STATIC")

add_executable(minesweeper main.cpp
        minesweeper/Tile.cpp
        minesweeper/Tile.h
        minesweeper/Minesweeper.cpp
        minesweeper/Minesweeper.h
        minesweeper/Grid.cpp
        minesweeper/Grid.h
        Game.cpp
        Game.h
        utils.cpp
        utils.h)

target_link_libraries(minesweeper ${CURSES_LIBRARY})