//
// Created by beerpsi on 12/23/2023.
//

#include "Leaderboard.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

Leaderboard::Leaderboard() = default;

void Leaderboard::load() {
    if (!std::filesystem::exists("lb.mine")) {
        return;
    }

    std::ifstream is("lb.mine");
    std::string buf;

    if (!std::getline(is, buf)) {
        throw std::runtime_error("Truncated stream: could not get magic bytes");
    }

    if (buf != "Minesweeper Leaderboard Format Version 2.00") {
        throw std::runtime_error("Invalid save file.");
    }

    for (auto & pb : pbs) {
        int scoresCount;
        is >> scoresCount;

        for (int j = 0; j < scoresCount; j++) {
            time_t score;
            is >> score;
            pb.push_back(score);
        }

        std::sort(pb.begin(), pb.end());
        std::getline(is, buf);
    }

    is.close();
}

void Leaderboard::save() {
    std::ofstream os("lb.mine");
    os << "Minesweeper Leaderboard Format Version 2.00\n";

    for (auto & pb : pbs) {
        os << pb.size() << ' ';

        for (const auto score : pb) {
            os << score << ' ';
        }

        os << '\n';
    }

    os.close();
}
