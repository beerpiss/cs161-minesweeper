cmake_minimum_required(VERSION 3.26)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-DNCURSES_STATIC")

add_executable(minesweeper src/main.cpp
        src/minesweeper/Tile.cpp
        src/minesweeper/Tile.h
        src/minesweeper/Minesweeper.cpp
        src/minesweeper/Minesweeper.h
        src/minesweeper/Grid.cpp
        src/minesweeper/Grid.h
        src/Game.cpp
        src/Game.h
        src/utils.cpp
        src/utils.h)

target_link_libraries(minesweeper ${CURSES_LIBRARY})