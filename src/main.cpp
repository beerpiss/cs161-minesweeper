#include "Game.h"
#include <fstream>

int main() {
    // Game(40, 35, 20).start();
    std::ifstream saveFile("save.mine");
    Game game = Game::importGameState(saveFile);
    game.start();
}
