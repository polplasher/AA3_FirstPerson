#include "Game.h"

int main(int argc, char** argv) {
    Game game;
    game.Initialize(argc, argv);
    game.Run();
    return 0;
}