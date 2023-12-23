cmake_minimum_required(VERSION 3.26)
project(minesweeper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)

if (WIN32 OR MSVC)
    find_package(PDCurses REQUIRED)
    set (CURSES_FOUND ${PDCURSES_FOUND})
    set (CURSES_LIBRARY ${PDCURSES_LIBRARY})
    set (CURSES_INCLUDE_DIRS ${PDCURSES_INCLUDE_DIR})
else (WIN32 OR MSVC)
    find_package (Curses REQUIRED)
endif (WIN32 OR MSVC)

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
        src/utils.h
        src/graphics/Graphics.cpp
        src/graphics/Graphics.h
        src/graphics/Print.cpp
        src/graphics/Print.h
        src/graphics/ColorPair.cpp
        src/graphics/ColorPair.h
        src/include/curses.h
        src/Leaderboard.cpp
        src/Leaderboard.h
)

target_include_directories(minesweeper PRIVATE src/include)
target_compile_definitions(minesweeper PRIVATE NCURSES_STATIC)
target_compile_definitions(minesweeper PRIVATE PDCURSES_STATIC)
target_link_libraries(minesweeper ${CURSES_LIBRARY})