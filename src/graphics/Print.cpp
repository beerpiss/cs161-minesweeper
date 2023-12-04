//
// Created by beerpsi on 12/4/2023.
//

#include "Print.h"

#include <string>

#include "ColorPair.h"

namespace graphics {
    void print(int x, int y, const std::string& data) {
        mvprintw(y, x, "%s", data.c_str());
    }

    void print(int x, int y, const std::string& data, Color fg, Color bg) {
        attron(COLOR_PAIR(ColorPair::get(fg, bg)));
        mvprintw(y, x, "%s", data.c_str());
        attroff(COLOR_PAIR(ColorPair::get(fg, bg)));
    }
}
