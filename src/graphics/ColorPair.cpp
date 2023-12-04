//
// Created by beerpsi on 12/4/2023.
//

#include "ColorPair.h"

#include <unordered_map>

int graphics::ColorPair::get(graphics::Color a, graphics::Color b) {
    static std::unordered_map<uint8_t, int> pairs;
    static short counter = 1;

    uint8_t key = a | (b << 4u);
    auto pair = pairs.find(key);

    if (pair == pairs.end()) {
        short id = counter++;
        init_pair(id, a, b);
        pair = pairs.insert({ key, id }).first;
    }

    return pair->second;
}
