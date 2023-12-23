//
// Created by beerpsi on 12/3/2023.
//
#include <chrono>

#include "utils.h"

time_t time_since_epoch() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}