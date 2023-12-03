//
// Created by beerpsi on 12/3/2023.
//

#include <memory>
#include <string>
#include <stdexcept>
#include <chrono>

#include "utils.h"

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'

    if (size_s <= 0) {
        throw std::runtime_error( "Error during formatting." );
    }

    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);

    std::snprintf(buf.get(), size, format.c_str(), args...);

    return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

time_t time_since_epoch() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}