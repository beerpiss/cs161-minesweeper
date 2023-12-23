//
// Created by beerpsi on 12/23/2023.
//

#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <string>
#include <vector>

class Leaderboard {
public:
    explicit Leaderboard();

    void load();
    void save();

    // basic, advanced, expert, master, remaster
    std::vector<time_t> pbs[5];
};

#endif //LEADERBOARD_H
