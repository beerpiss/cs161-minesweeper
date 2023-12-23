//
// Created by beerpsi on 12/3/2023.
//

#ifndef MINESWEEPER_UTILS_H
#define MINESWEEPER_UTILS_H

#include <string>
#include <vector>

template <typename T>
std::vector<T> slicing(std::vector<T> const& v,
                       int X, int Y)
{

    // Begin and End iterator
    auto first = v.begin() + X;
    auto last = Y >= v.size() ? v.end() : v.begin() + Y + 1;

    // Copy the element
    std::vector<T> vector(first, last);

    // Return the results
    return vector;
}

template <typename T>
std::string string_join(const std::vector<T> &lst, const std::string &delim)
{
    std::string ret;
    for(const auto &s : lst) {
        if(!ret.empty())
            ret += delim;
        ret += std::to_string(s);
    }
    return ret;
}

time_t time_since_epoch();
#endif //MINESWEEPER_UTILS_H
