//
// Created by beerpsi on 12/4/2023.
//

#ifndef MINESWEEPER_PRINT_H
#define MINESWEEPER_PRINT_H

#include <string>

#include "ColorPair.h"

namespace graphics {
    void print(int x, int y, const std::string& data);

    void print(int x, int y, const std::string& data, Color fg, Color bg = Color::BLACK);
}


#endif //MINESWEEPER_PRINT_H
