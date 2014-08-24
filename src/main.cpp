#include "Game.hpp"

int main()
{
    std::srand(time(NULL));

    try
    {
        Game game;
        game.run();
        return 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
