//
// Created by beerpsi on 12/4/2023.
//

#ifndef MINESWEEPER_CURSES_H
#define MINESWEEPER_CURSES_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define NCURSES_MOUSE_VERSION 2
#include <pdcurses.h>
#else
#include <ncurses/curses.h>
#endif

#endif //MINESWEEPER_CURSES_H
