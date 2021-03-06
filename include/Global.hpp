#ifndef GLOBAL_HPP
#define GLOBAL_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TGUI/TGUI.hpp>

#include "Player.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const float FRAME_RATE = 60;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Context
{
    sf::RenderWindow& window;
    tgui::Gui&        gui;
    Player&           player;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // GLOBAL_HPP
