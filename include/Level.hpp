#ifndef LEVEL_HPP
#define LEVEL_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Thor/Graphics/BigSprite.hpp>
#include <Thor/Graphics/BigTexture.hpp>

#include <SFML/Audio.hpp>

#include "Global.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Level
{
public:

    Level(Context context, const std::string& name);

    bool play();


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:

    void handleEvents();
    void update();
    void draw();

    void movePlayer(sf::Time elapsedTime);
    void moveView();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:

    Context m_context;

    std::string m_name;

    bool m_playing = true;
    bool m_finished = false;

    sf::Image m_background;

    thor::BigTexture m_backgroundTexture;
    thor::BigSprite  m_backgroundSprite;

    sf::Clock clock;
    sf::Time  m_elapsedTime;

    std::vector<tgui::ext::AnimatedPicture::Ptr> m_collectibles;

    std::map<std::string, std::vector<tgui::Picture::Ptr>> m_traps;
    std::map<std::string, sf::Clock> m_trapsClock;

    sf::SoundBuffer m_jumpSoundBuffer;
    sf::Sound       m_jumpSound;

    sf::SoundBuffer m_pickupSoundBuffer;
    sf::Sound       m_pickupSound;

    float m_horizontalSpeed;
    float m_verticalSpeed;
    float m_gravity;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // LEVEL_HPP
