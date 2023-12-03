//
// Created by beerpsi on 12/3/2023.
//

#ifndef MINESWEEPER_UTILS_H
#define MINESWEEPER_UTILS_H

#include <string>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args);

time_t time_since_epoch();
#endif //MINESWEEPER_UTILS_H
