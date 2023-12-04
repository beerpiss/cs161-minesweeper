//
// Created by beerpsi on 12/4/2023.
//

#include "Graphics.h"

#include "../curses.h"

namespace graphics {
    Graphics::Graphics() {
        initscr();
        noecho();
        cbreak();
        curs_set(0);
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED, nullptr);

        if (has_colors()) {
            start_color();
        }
    }

    Graphics::~Graphics() {
        endwin();
    }
}
