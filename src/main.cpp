#include "Game.h"
#include <curses.h>
#include <filesystem>
#include <fstream>
#include <optional>
#include <valarray>

#include "Leaderboard.h"
#include "utils.h"
#include "graphics/Print.h"

typedef struct {
    int id;
    const char* desc;
} Option;

int main() {
    std::vector<Option> options;
    graphics::Graphics graphics;
    Leaderboard leaderboard;
    const bool hasOldSave = std::filesystem::exists("save.mine");

    leaderboard.load();
    nodelay(stdscr, FALSE);

    options.push_back({ 1, "New Game" });

    if (hasOldSave) {
        options.push_back({ 2, "Continue" });
    }

    options.push_back({ 3, "Leaderboard" });
    options.push_back({ 4, "Exit" });

menu_start:
    clear();
    printw("MINESWEEPER\n\n");
    for (int i = 0; i < options.size(); i++) {
        printw("[%d] %s\n", i + 1, options[i].desc);
    }
    printw("\n");
    printw("Select an option using the number keys...");

    Option selection;
    for (;;) {
        const char option = getch();

        int conv;
        try {
            conv = std::stoi(std::string(1, option));
        } catch (std::invalid_argument& e) {
            continue;
        }

        if (conv < 1 || conv > options.size()) {
            continue;
        }

        selection = options[conv - 1];
        break;
    }

    std::optional<Game> game = std::nullopt;
    switch (selection.id) {
        case 1: {
            if (hasOldSave) {
                clear();
                printw("Delete previous save? [y/n]");

                char answer = ' ';
                for (; answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N'; answer = getch()) {}

                if (answer == 'y' || answer == 'Y') {
                    std::remove("save.mine");
                } else {
                    goto menu_start;
                }
            }

            clear();

            printw("Select a difficulty using the number keys:\n");
            printw("[1] Basic: 13x13, 16 bombs\n");
            printw("[2] Advanced: 16x16, 40 bombs\n");
            printw("[3] Expert: 30x16, 96 bombs\n");
            printw("[4] Master: 30x30, 198 bombs\n");
            printw("[5] Re:Master: 40x35, 350 bombs\n");
            printw("[6] Custom\n");
            printw("[7] Return to main menu\n");

            int selectedDifficulty;
            for (;;) {
                const char option = getch();

                try {
                    selectedDifficulty = std::stoi(std::string(1, option));
                } catch (std::invalid_argument& e) {
                    continue;
                }

                if (selectedDifficulty < 1 || selectedDifficulty > 7) {
                    continue;
                }

                break;
            }

            clear();
            nodelay(stdscr, TRUE);

            switch (selectedDifficulty) {
                case 1:
                    game.emplace(13, 13, 10, 0);
                    break;
                case 2:
                    game.emplace(16, 16, 16, 1);
                    break;
                case 3:
                    game.emplace(30, 16, 20, 2);
                    break;
                case 4:
                    game.emplace(30, 30, 22, 3);
                    break;
                case 5:
                    game.emplace(40, 35, 25, 4);
                    break;
                case 6: {
                    clear();
                    int width, height, density = -1;

                    echo();
                    printw("Enter board width: ");
                    scanw("%d", &width);

                    printw("Enter board height: ");
                    scanw("%d", &height);

                    while (density < 0 || density > 100) {
                        printw("Enter mine density in percentages [0-100]: ");
                        scanw("%d", &density);
                    }

                    game.emplace(width, height, density, 6);
                    clear();
                    noecho();
                    break;
                }
                case 7:
                    goto menu_start;
                default:
                    break;
            }

            if (!game.has_value()) {
                throw std::runtime_error("");
            }

            game->start();
            std::remove("save.mine");
            if (game->won() && game->difficultyId < 6) {
                leaderboard.pbs[game->difficultyId].push_back(game->getCurrentTime());
            }
            leaderboard.save();

            nodelay(stdscr, FALSE);
            getch();

            break;
        }
        case 2: {
            clear();
            nodelay(stdscr, TRUE);
            std::ifstream saveFile("save.mine");
            game.emplace(Game::importGameState(saveFile));
            saveFile.close();

            if (!game.has_value()) {
                throw std::runtime_error("");
            }

            game->start();
            std::remove("save.mine");
            if (game->won() && game->difficultyId < 6) {
                leaderboard.pbs[game->difficultyId].push_back(game->getCurrentTime());
            }
            leaderboard.save();

            nodelay(stdscr, FALSE);
            getch();

            break;
        }
        case 3: {
            clear();
            const int maxX = getmaxx(stdscr);
            graphics::print(maxX / 2 - 12, 0, "Leaderboard");
            printw("%d", leaderboard.pbs[0].size());
            mvprintw(1, 0, "Basic: %s", string_join(slicing(leaderboard.pbs[0], 0, 5), ", ").c_str());
            mvprintw(2, 0, "Advanced: %s", string_join(slicing(leaderboard.pbs[1], 0, 5), ", ").c_str());
            mvprintw(3, 0, "Expert: %s", string_join(slicing(leaderboard.pbs[2], 0, 5), ", ").c_str());
            mvprintw(4, 0, "Master: %s", string_join(slicing(leaderboard.pbs[3], 0, 5), ", ").c_str());
            mvprintw(5, 0, "Re:Master: %s", string_join(slicing(leaderboard.pbs[4], 0, 5), ", ").c_str());
            graphics::print(0, 6, "Press any key to return to main menu...");
            getch();
            goto menu_start;

        }
        case 4: {
            exit(0);
        }
        default:
            break;
    }
}
