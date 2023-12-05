//
// Created by beerpsi on 12/4/2023.
//

#ifndef MINESWEEPER_COLORPAIR_H
#define MINESWEEPER_COLORPAIR_H

#include <cstdint>

#include "curses.h"

namespace graphics {
    enum Color : uint8_t {
        BLACK = COLOR_BLACK,
        RED = COLOR_RED,
        GREEN = COLOR_GREEN,
        YELLOW = COLOR_YELLOW,
        BLUE = COLOR_BLUE,
        MAGENTA = COLOR_MAGENTA,
        CYAN = COLOR_CYAN,
        WHITE = COLOR_WHITE,
    };

    class ColorPair {
    public:
        static int get(Color a, Color b);
    };
}


#endif //MINESWEEPER_COLORPAIR_H
