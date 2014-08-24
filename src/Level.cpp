
#include "Level.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Level::Level(Context context, const std::string& name) :
    m_context(context),
    m_name   {name}
{
    m_context.player.addToGui();
    m_context.player.setPosition({0, 0});

    if (!m_jumpSoundBuffer.loadFromFile("resources/jump.wav"))
        throw std::runtime_error{"Failed to load resources/jump.wav"};
    else
        m_jumpSound.setBuffer(m_jumpSoundBuffer);

    if (!m_pickupSoundBuffer.loadFromFile("resources/pickup.wav"))
        throw std::runtime_error{"Failed to load resources/pickup.wav"};
    else
        m_pickupSound.setBuffer(m_pickupSoundBuffer);

    std::ifstream configFile{"resources/" + name + ".conf"};
    if (!configFile.good())
        throw std::runtime_error{"Failed to load level '" + name + "'."};

    while (configFile.good())
    {
        std::string line;
        std::getline(configFile, line);

        if (!line.empty())
        {
            auto equalCharPos = line.find('=');
            if (equalCharPos == std::string::npos)
                throw std::runtime_error{"Error parsing config file for level '" + name + "'."};

            std::string property = line.substr(0, equalCharPos);
            std::string value = line.substr(equalCharPos + 1);

            if (property == "HorizontalSpeed")
                m_horizontalSpeed = std::stof(value);
            else if (property == "VerticalSpeed")
                m_verticalSpeed = std::stof(value);
            else if (property == "Gravity")
                m_gravity = std::stof(value);
            else if (property == "Left")
                m_context.player.setPosition({std::stof(value), m_context.player.getPosition().y});
            else if (property == "Top")
                m_context.player.setPosition({m_context.player.getPosition().x, std::stof(value)});
            else
                throw std::runtime_error{"Error parsing config file for level '" + name + "'."};
        }
    }

    if (name == "Level2")
    {
        if ((!m_background.loadFromFile("resources/" + name + "BW.png")) || (!m_backgroundTexture.loadFromFile("resources/" + name + ".png")))
            throw std::runtime_error{"Failed to load level '" + name + "'."};
    }
    else
    {
        if ((!m_background.loadFromFile("resources/" + name + "BW.png")) || (!m_backgroundTexture.loadFromFile("resources/" + name + ".jpg")))
            throw std::runtime_error{"Failed to load level '" + name + "'."};
    }

    m_backgroundSprite.setTexture(m_backgroundTexture);

    if (name == "Level1")
    {
        auto portalPart = tgui::ext::AnimatedPicture::create();
        portalPart->addFrame("resources/Portal/1.png", sf::milliseconds(200));
        portalPart->addFrame("resources/Portal/2.png", sf::milliseconds(200));
        portalPart->addFrame("resources/Portal/3.png", sf::milliseconds(200));
        portalPart->addFrame("resources/Portal/4.png", sf::milliseconds(200));
        portalPart->setLooping(true);
        portalPart->play();

        portalPart->setPosition(210, 355);
        m_collectibles.push_back(tgui::ext::AnimatedPicture::copy(portalPart));

        portalPart->setPosition(100, 1490);
        m_collectibles.push_back(tgui::ext::AnimatedPicture::copy(portalPart));

        portalPart->setPosition(3330, 1050);
        m_collectibles.push_back(tgui::ext::AnimatedPicture::copy(portalPart));

        for (auto& collectible : m_collectibles)
            m_context.gui.add(collectible);

        float posX = 2540;
        for (unsigned int i = 1; i <= 5; ++i)
        {
            auto top = tgui::Picture::create("resources/Level1Trap/Top" + std::to_string(i) + ".png");
            auto middle = tgui::Picture::create("resources/Level1Trap/Middle" + std::to_string(i) + ".png");
            auto bottom = tgui::Picture::create("resources/Level1Trap/Bottom" + std::to_string(i) + ".png");

            top->setPosition({posX, 1111});
            middle->setPosition({posX, 1236});
            bottom->setPosition({posX, 1360});

            m_context.gui.add(top);
            m_context.gui.add(middle);
            m_context.gui.add(bottom);

            m_traps["GroundTrap"].push_back(top);
            m_traps["GroundTrap"].push_back(middle);
            m_traps["GroundTrap"].push_back(bottom);

            posX += top->getSize().x;
        }

        auto trampolineUp = tgui::Picture::create("resources/TrampolineUp.png");
        auto trampolineDown = tgui::Picture::create("resources/TrampolineDown.png");

        trampolineDown->hide();

        trampolineUp->setPosition({2452, 1099});
        trampolineDown->setPosition({2452, 1099});
        m_traps["Trampoline"].push_back(tgui::Picture::copy(trampolineUp));
        m_traps["Trampoline"].push_back(tgui::Picture::copy(trampolineDown));

        trampolineUp->setPosition({3185, 1081});
        trampolineDown->setPosition({3185, 1081});
        m_traps["Trampoline"].push_back(tgui::Picture::copy(trampolineUp));
        m_traps["Trampoline"].push_back(tgui::Picture::copy(trampolineDown));

        for (auto& trampoline : m_traps["Trampoline"])
            m_context.gui.add(trampoline);
    }
    else if (name == "Level2")
    {
        // Key
        {
            auto key = tgui::ext::AnimatedPicture::create();
            m_context.gui.add(key);

            key->addFrame("resources/Key/1.png", sf::milliseconds(200));
            key->addFrame("resources/Key/2.png", sf::milliseconds(200));
            key->addFrame("resources/Key/3.png", sf::milliseconds(200));
            key->addFrame("resources/Key/4.png", sf::milliseconds(200));
            key->setPosition(38, 211);
            key->setLooping(true);
            key->play();

            m_collectibles.push_back(key);
        }

        // Fire
        {
            auto fireUp = tgui::Picture::create("resources/FireUp.png");
            auto fireDown = tgui::Picture::create("resources/FireDown.png");

            fireUp->setPosition({3569, 38});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));

            fireDown->setPosition({3516, 41});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireDown));

            fireUp->hide();

            fireUp->setPosition({3295, 38});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));

            fireUp->setPosition({3250, 38});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));

            fireUp->setPosition({3205, 38});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));

            fireDown->hide();
            fireUp->show();

            fireDown->setPosition({3160, 41});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireDown));

            fireDown->setPosition({3115, 41});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireDown));

            fireDown->setPosition({3070, 41});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireDown));

            fireUp->setPosition({3025, 38});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));

            fireUp->setPosition({2980, 38});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));

            fireUp->setPosition({2935, 38});
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));

            for (auto& fire : m_traps["Fire"])
                m_context.gui.add(fire);

            fireUp->hide();
            m_traps["Fire"].push_back(tgui::Picture::copy(fireUp));
        }

        // Crusher
        {
            auto crusher = tgui::Picture::create("resources/Crusher.png");

            crusher->setPosition({2078, 337});
            m_traps["Crusher"].push_back(tgui::Picture::copy(crusher));

            crusher->move({124, 0});
            m_traps["Crusher"].push_back(tgui::Picture::copy(crusher));

            crusher->move({124, 106});
            m_traps["Crusher"].push_back(tgui::Picture::copy(crusher));

            crusher->move({124, 0});
            m_traps["Crusher"].push_back(tgui::Picture::copy(crusher));

            crusher->move({124, 0});
            m_traps["Crusher"].push_back(tgui::Picture::copy(crusher));

            crusher->move({124, 0});
            m_traps["Crusher"].push_back(tgui::Picture::copy(crusher));

            for (auto& crusherPicture : m_traps["Crusher"])
                m_context.gui.add(crusherPicture);

            crusher->hide();
            m_traps["Crusher"].push_back(tgui::Picture::copy(crusher));
        }

        // Spikes
        {
            auto spikes = tgui::Picture::create("resources/Spikes.png");
            auto spikesHidden = tgui::Picture::create("resources/SpikesHidden.png");

            float space = (488 - (7 * spikes->getSize().x)) / 8.0f;

            for (unsigned int i = 0; i < 7; ++i)
            {
                spikes->setPosition({802 + ((i+1) * space) + (i * spikes->getSize().x), 291 - spikes->getSize().y});
                m_traps["Spikes"].push_back(tgui::Picture::copy(spikes));

                spikesHidden->setPosition(spikes->getPosition());
                m_context.gui.add(tgui::Picture::copy(spikesHidden));
            }

            for (unsigned int i = 0; i < 3; ++i)
                m_traps["Spikes"][i]->hide();

            for (auto& spikesPicture : m_traps["Spikes"])
                m_context.gui.add(spikesPicture);
        }

        // Fireballs
        {
            auto fireBall = tgui::Picture::create("resources/FireBall.png");

            fireBall->setPosition({837, 569});
            m_traps["FireBall"].push_back(tgui::Picture::copy(fireBall));

            fireBall->setPosition({1032, 480});
            m_traps["FireBall"].push_back(tgui::Picture::copy(fireBall));

            fireBall->disable();
            fireBall->setPosition({1228, 391});
            m_traps["FireBall"].push_back(tgui::Picture::copy(fireBall));

            for (auto& fireBallPicture : m_traps["FireBall"])
                m_context.gui.add(fireBallPicture);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Level::play()
{
    while (m_playing)
    {
        handleEvents();
        update();
        draw();

        sf::sleep(sf::milliseconds(1));
    }

    m_context.gui.setView(m_context.window.getDefaultView());
    m_context.gui.removeAllWidgets();
    return m_finished;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::handleEvents()
{
    sf::Event event;
    while (m_context.window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            throw std::runtime_error{"Window closed while playing"};

        m_context.gui.handleEvent(event);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::update()
{
    const sf::Time elapsedTime = sf::seconds(1 / FRAME_RATE);

    m_elapsedTime += clock.restart();

    while (m_elapsedTime > elapsedTime)
    {
        m_elapsedTime -= elapsedTime;

        movePlayer(elapsedTime);
        m_context.player.update();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::movePlayer(sf::Time elapsedTime)
{
    // Let the player move left and right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_context.player.setSpeed({-m_horizontalSpeed, m_context.player.getSpeed().y});
        m_context.player.setMoving(true);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_context.player.setSpeed({m_horizontalSpeed, m_context.player.getSpeed().y});
        m_context.player.setMoving(true);
    }
    else
    {
        m_context.player.setSpeed({0, m_context.player.getSpeed().y});
        m_context.player.setMoving(false);
    }

    // Find out if the player is touching the ground
    bool touchingGround = false;
    for (unsigned int i = m_context.player.getSize().x / 4; i < m_context.player.getSize().x * 3/4; ++i)
    {
        if (m_background.getPixel(m_context.player.getPosition().x + i, m_context.player.getPosition().y + m_context.player.getSize().y + 1) == sf::Color::Black)
        {
            touchingGround = true;
            break;
        }
    }

    // Apply gravity if the player is not touching the ground
    if (!touchingGround)
    {
        m_context.player.setSpeed({m_context.player.getSpeed().x, std::min(-m_verticalSpeed, m_context.player.getSpeed().y - m_gravity * elapsedTime.asSeconds())});
    }
    else // The player is touching the ground, he can jump
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            // You will jump much higher on top of a trampoline
            bool trampoline = false;
            for (auto it = m_traps["Trampoline"].begin(); it != m_traps["Trampoline"].end(); ++it)
            {
                if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                    sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
                {
                    trampoline = true;
                    break;
                }
            }

            if (trampoline)
                m_context.player.setSpeed({m_context.player.getSpeed().x, 2 * m_verticalSpeed});
            else
                m_context.player.setSpeed({m_context.player.getSpeed().x, m_verticalSpeed});

            m_jumpSound.play();
        }
        else
            m_context.player.setSpeed({m_context.player.getSpeed().x, 0});
    }

    // Move the player
    m_context.player.move(m_context.player.getSpeed() * elapsedTime.asSeconds());

    if (m_context.player.getPosition().x < 0)
        m_context.player.setPosition(0, m_context.player.getPosition().y);
    else if (m_context.player.getPosition().x + m_context.player.getSize().x > m_background.getSize().x)
        m_context.player.setPosition(m_background.getSize().x - m_context.player.getSize().x, m_context.player.getPosition().y);

    if (m_context.player.getPosition().y < 0)
    {
        m_context.player.setPosition(m_context.player.getPosition().x, 0);
        m_context.player.setSpeed({m_context.player.getSpeed().x, 0});
    }

    // Watch out for the left side of your body
    touchingGround = true;
    while (touchingGround)
    {
        touchingGround = false;

        for (unsigned int i = m_context.player.getSize().y / 4; i < m_context.player.getSize().y * 3/4; ++i)
        {
            auto pixelColor = m_background.getPixel(m_context.player.getPosition().x, m_context.player.getPosition().y + i);

            if (pixelColor == sf::Color::Black)
            {
                touchingGround = true;
                break;
            }
            else if (pixelColor == sf::Color::Red)
            {
                m_playing = false;
                m_finished = false;
            }
        }

        if (touchingGround)
            m_context.player.setPosition({m_context.player.getPosition().x + 1, m_context.player.getPosition().y});
    }

    // Watch out for the right side of your body
    touchingGround = true;
    while (touchingGround)
    {
        touchingGround = false;

        for (unsigned int i = m_context.player.getSize().y / 4; i < m_context.player.getSize().y * 3/4; ++i)
        {
            auto pixelColor = m_background.getPixel(m_context.player.getPosition().x + m_context.player.getSize().x, m_context.player.getPosition().y + i);

            if (pixelColor == sf::Color::Black)
            {
                touchingGround = true;
                break;
            }
            else if (pixelColor == sf::Color::Red)
            {
                m_playing = false;
                m_finished = false;
            }
        }

        if (touchingGround)
            m_context.player.setPosition({m_context.player.getPosition().x - 1, m_context.player.getPosition().y});
    }

    // Watch out for your head
    touchingGround = true;
    while (touchingGround)
    {
        touchingGround = false;

        for (unsigned int i = m_context.player.getSize().x / 4; i < m_context.player.getSize().x * 3/4; ++i)
        {
            auto pixelColor = m_background.getPixel(m_context.player.getPosition().x + i, m_context.player.getPosition().y);

            if (pixelColor == sf::Color::Black)
            {
                touchingGround = true;
                break;
            }
            else if (pixelColor == sf::Color::Red)
            {
                m_playing = false;
                m_finished = false;
            }
        }

        if (touchingGround)
        {
            m_context.player.setPosition({m_context.player.getPosition().x, m_context.player.getPosition().y + 1});
            m_context.player.setSpeed({m_context.player.getSpeed().x, 0});
        }
    }

    // The player cannot be beneath the ground
    touchingGround = true;
    while (touchingGround)
    {
        touchingGround = false;

        for (unsigned int i = m_context.player.getSize().x / 4; i < m_context.player.getSize().x * 3/4; ++i)
        {
            auto pixelColor = m_background.getPixel(m_context.player.getPosition().x + i, m_context.player.getPosition().y + m_context.player.getSize().y);

            if (pixelColor == sf::Color::Black)
            {
                touchingGround = true;
                break;
            }
            else if (pixelColor == sf::Color::Red)
            {
                m_playing = false;
                m_finished = false;
            }
        }

        if (touchingGround)
            m_context.player.setPosition({m_context.player.getPosition().x, m_context.player.getPosition().y - 1});
    }

    // Check for overlapping with collectibles
    for (auto it = m_collectibles.begin(); it != m_collectibles.end(); ++it)
    {
        if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
            sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
        {
            m_context.gui.remove(*it);
            m_collectibles.erase(it);
            m_pickupSound.play();

            if (m_collectibles.empty())
            {
                m_playing = false;
                m_finished = true;
            }

            break;
        }
    }

    // Update the traps if needed
    if (m_name == "Level1")
    {
        std::for_each(m_traps["Stalactite"].begin(), m_traps["Stalactite"].end(), [=](tgui::Picture::Ptr pic) { pic->move({0, 500 * elapsedTime.asSeconds()}); });

        for (auto it = m_traps["Stalactite"].begin(); it != m_traps["Stalactite"].end();)
        {
            if ((*it)->getPosition().y > 1590)
            {
                m_context.gui.remove(*it);
                it = m_traps["Stalactite"].erase(it);
            }
            else
            {
                // You are dead when you hit one of these things
                if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                    sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
                {
                    m_playing = false;
                    m_finished = false;
                }

                ++it;
            }
        }

        if (m_trapsClock["Stalactite"].getElapsedTime().asMilliseconds() >= 800)
        {
            m_trapsClock["Stalactite"].restart();

            auto pic = tgui::Picture::create("resources/Level1Stalactite.png");
            pic->setPosition(460 + std::rand() % 460, 880);
            m_context.gui.add(pic);

            m_traps["Stalactite"].push_back(pic);
        }

        for (auto it = m_traps["GroundTrap"].begin(); it != m_traps["GroundTrap"].end();)
        {
            // The ground will disappear when you touch it
            if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
            {
                m_context.gui.remove(*it);
                it = m_traps["GroundTrap"].erase(it);
            }
            else
                ++it;
        }

        for (auto it = m_traps["Trampoline"].begin(); it != m_traps["Trampoline"].end(); ++it)
        {
            if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
            {
                (*it)->hide();
                (*(++it))->show();
            }
            else
            {
                (*it)->show();
                (*(++it))->hide();
            }
        }
    }
    else if (m_name == "Level2")
    {
        for (auto it = m_traps["Fire"].begin(); it != m_traps["Fire"].end(); ++it)
        {
            if ((*it)->isVisible())
            {
                // You get fried when you hit the fire
                if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                    sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
                {
                    m_playing = false;
                    m_finished = false;
                }
            }
        }

        // You can't walk through when the crusher is down, but you should not die either when it is not moving
        for (auto it = m_traps["Crusher"].begin(); it != m_traps["Crusher"].end(); ++it)
        {
            if ((*it)->isVisible())
            {
                if ((*it)->getPosition().y != 337)
                {
                    if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                        sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
                    {
                        if ((*it)->getPosition().x < m_context.player.getPosition().x)
                            m_context.player.setPosition({(*it)->getPosition().x + (*it)->getSize().x, m_context.player.getPosition().y});
                        else
                            m_context.player.setPosition({(*it)->getPosition().x - m_context.player.getSize().x, m_context.player.getPosition().y});
                    }
                }
            }
        }

        // Spikes can be sharp
        for (auto it = m_traps["Spikes"].begin(); it != m_traps["Spikes"].end(); it++)
        {
            if ((*it)->isVisible())
            {
                if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                    sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
                {
                    m_playing = false;
                    m_finished = false;
                }
            }
        }

        // Fireballs are hot
        for (auto it = m_traps["FireBall"].begin(); it != m_traps["FireBall"].end(); it++)
        {
            if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
            {
                m_playing = false;
                m_finished = false;
            }
        }

        if (m_trapsClock["Fire"].getElapsedTime().asMilliseconds() > 1000)
        {
            m_trapsClock["Fire"].restart();

            // Play with the first two fires
            if (m_traps["Fire"][0]->isVisible())
            {
                m_traps["Fire"][0]->hide();
            }
            else if (m_traps["Fire"][1]->isVisible())
            {
                m_traps["Fire"][0]->show();
                m_traps["Fire"][1]->hide();
            }
            else
            {
                m_traps["Fire"][1]->show();
            }

            // Fun with the other fires
            bool left = m_traps["Fire"][9]->isVisible();
            bool middle = m_traps["Fire"][6]->isVisible();
            bool right = m_traps["Fire"][3]->isVisible();
            bool special = m_traps["Fire"][11]->isVisible();

            if (left && middle && right && special)
            {
                m_traps["Fire"][5]->hide();
                m_traps["Fire"][6]->hide();
                m_traps["Fire"][7]->hide();
            }
            else if (left && !middle && right && special)
            {
                m_traps["Fire"][5]->show();
                m_traps["Fire"][6]->show();
                m_traps["Fire"][7]->show();

                m_traps["Fire"][8]->hide();
                m_traps["Fire"][9]->hide();
                m_traps["Fire"][10]->hide();
            }
            else if (!left && middle && right && special)
            {
                m_traps["Fire"][5]->hide();
                m_traps["Fire"][6]->hide();
                m_traps["Fire"][7]->hide();
            }
            else if (!left && !middle && right && special)
            {
                m_traps["Fire"][2]->hide();
                m_traps["Fire"][3]->hide();
                m_traps["Fire"][4]->hide();

                m_traps["Fire"][5]->show();
                m_traps["Fire"][6]->show();
                m_traps["Fire"][7]->show();

                m_traps["Fire"][8]->show();
                m_traps["Fire"][9]->show();
                m_traps["Fire"][10]->show();
            }
            else if (left && middle && !right && special)
            {
                m_traps["Fire"][5]->hide();
                m_traps["Fire"][6]->hide();
                m_traps["Fire"][7]->hide();

                m_traps["Fire"][11]->hide();
            }
            else if (left && !middle && !right && !special)
            {
                m_traps["Fire"][2]->show();
                m_traps["Fire"][3]->show();
                m_traps["Fire"][4]->show();
            }
            else if (left && !middle && right && !special)
            {
                m_traps["Fire"][8]->hide();
                m_traps["Fire"][9]->hide();
                m_traps["Fire"][10]->hide();
            }
            else if (!left && !middle && right && !special)
            {
                m_traps["Fire"][5]->show();
                m_traps["Fire"][6]->show();
                m_traps["Fire"][7]->show();
            }
            else if (!left && middle && right && !special)
            {
                m_traps["Fire"][8]->show();
                m_traps["Fire"][9]->show();
                m_traps["Fire"][10]->show();

                m_traps["Fire"][11]->show();
            }
        }

        // Fun with crushers
        if (m_trapsClock["Crusher"].getElapsedTime().asMilliseconds() > 800)
        {
            m_trapsClock["Crusher"].restart();

            if (m_traps["Crusher"][6]->getPosition().y == 337)
            {
                if (m_traps["Crusher"][5]->getPosition().y == 337)
                    m_traps["Crusher"][5]->setPosition({m_traps["Crusher"][5]->getPosition().x, 443});
                else
                {
                    m_traps["Crusher"][5]->setPosition({m_traps["Crusher"][5]->getPosition().x, 337});
                    m_traps["Crusher"][6]->setPosition({m_traps["Crusher"][6]->getPosition().x, 443});
                }
            }
            else
            {
                if (m_traps["Crusher"][0]->getPosition().y == 337 && m_traps["Crusher"][1]->getPosition().y == 337)
                {
                    m_traps["Crusher"][0]->setPosition({m_traps["Crusher"][0]->getPosition().x, 443});
                    m_traps["Crusher"][2]->setPosition({m_traps["Crusher"][2]->getPosition().x, 337});
                }
                else if (m_traps["Crusher"][1]->getPosition().y == 337 && m_traps["Crusher"][2]->getPosition().y == 337)
                {
                    m_traps["Crusher"][1]->setPosition({m_traps["Crusher"][1]->getPosition().x, 443});
                    m_traps["Crusher"][3]->setPosition({m_traps["Crusher"][3]->getPosition().x, 337});
                }
                else if (m_traps["Crusher"][2]->getPosition().y == 337 && m_traps["Crusher"][3]->getPosition().y == 337)
                {
                    m_traps["Crusher"][2]->setPosition({m_traps["Crusher"][2]->getPosition().x, 443});
                    m_traps["Crusher"][4]->setPosition({m_traps["Crusher"][4]->getPosition().x, 337});
                }
                else if (m_traps["Crusher"][3]->getPosition().y == 337 && m_traps["Crusher"][4]->getPosition().y == 337)
                {
                    m_traps["Crusher"][3]->setPosition({m_traps["Crusher"][3]->getPosition().x, 443});
                    m_traps["Crusher"][0]->setPosition({m_traps["Crusher"][0]->getPosition().x, 337});
                }
                else if (m_traps["Crusher"][4]->getPosition().y == 337 && m_traps["Crusher"][0]->getPosition().y == 337)
                {
                    m_traps["Crusher"][4]->setPosition({m_traps["Crusher"][4]->getPosition().x, 443});
                    m_traps["Crusher"][1]->setPosition({m_traps["Crusher"][1]->getPosition().x, 337});
                }

                if (m_traps["Crusher"][5]->getPosition().y == 337)
                {
                    m_traps["Crusher"][6]->setPosition({m_traps["Crusher"][6]->getPosition().x, 337});
                    m_traps["Crusher"][5]->setPosition({m_traps["Crusher"][5]->getPosition().x, 443});
                }
                else
                {
                    m_traps["Crusher"][5]->setPosition({m_traps["Crusher"][5]->getPosition().x, 337});
                }
            }

            // Hit one of these crushers and your 2D character will become 1D
            for (auto it = m_traps["Crusher"].begin(); it != m_traps["Crusher"].end(); ++it)
            {
                if ((*it)->isVisible())
                {
                    if ((*it)->getPosition().y != 337)
                    {
                        if (sf::FloatRect{(*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y}.intersects(
                            sf::FloatRect{m_context.player.getPosition().x, m_context.player.getPosition().y, m_context.player.getSize().x, m_context.player.getSize().y}))
                        {
                            m_playing = false;
                            m_finished = false;
                        }
                    }
                }
            }
        }

        // Fun with spikes
        if (m_trapsClock["Spikes"].getElapsedTime().asMilliseconds() > 235)
        {
            m_trapsClock["Spikes"].restart();

            if (m_traps["Spikes"][0]->isVisible() && !m_traps["Spikes"][6]->isVisible())
            {
                m_traps["Spikes"][6]->show();
                m_traps["Spikes"][3]->hide();
            }
            else if (m_traps["Spikes"][6]->isVisible() && !m_traps["Spikes"][5]->isVisible())
            {
                m_traps["Spikes"][5]->show();
                m_traps["Spikes"][2]->hide();
            }
            else if (m_traps["Spikes"][5]->isVisible() && !m_traps["Spikes"][4]->isVisible())
            {
                m_traps["Spikes"][4]->show();
                m_traps["Spikes"][1]->hide();
            }
            else if (m_traps["Spikes"][4]->isVisible() && !m_traps["Spikes"][3]->isVisible())
            {
                m_traps["Spikes"][3]->show();
                m_traps["Spikes"][0]->hide();
            }
            else if (m_traps["Spikes"][3]->isVisible() && !m_traps["Spikes"][2]->isVisible())
            {
                m_traps["Spikes"][2]->show();
                m_traps["Spikes"][6]->hide();
            }
            else if (m_traps["Spikes"][2]->isVisible() && !m_traps["Spikes"][1]->isVisible())
            {
                m_traps["Spikes"][1]->show();
                m_traps["Spikes"][5]->hide();
            }
            else if (m_traps["Spikes"][1]->isVisible() && !m_traps["Spikes"][0]->isVisible())
            {
                m_traps["Spikes"][0]->show();
                m_traps["Spikes"][4]->hide();
            }
        }

        // Fire balls just go up and down
        const float speed = 89.f / 0.2f;
        for (auto& fireBall : m_traps["FireBall"])
        {
            if ((std::abs(fireBall->getPosition().y - 569.f) < 0.000001f) && (m_trapsClock["FireBall"].getElapsedTime().asMilliseconds() > 100))
            {
                fireBall->show();
                fireBall->enable();
            }

            if (fireBall->isEnabled())
            {
                fireBall->move({0, -speed * elapsedTime.asSeconds()});

                if (fireBall->getPosition().y <= 391)
                {
                    fireBall->setPosition({fireBall->getPosition().x, 391});
                    fireBall->disable();
                }
            }
            else // Fireball is moving down
            {
                if (fireBall->isVisible())
                {
                    fireBall->move({0, speed * elapsedTime.asSeconds()});

                    if (fireBall->getPosition().y >= 569)
                    {
                        fireBall->setPosition({fireBall->getPosition().x, 569});
                        fireBall->hide();

                        m_trapsClock["FireBall"].restart();
                    }
                }
            }
        }
    }

    moveView();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::moveView()
{
    sf::View view = m_context.window.getView();
    sf::Vector2f viewPos;

    if (m_context.player.getPosition().x + m_context.player.getSize().x / 2.0f < view.getSize().x / 2.0f)
        viewPos.x = view.getSize().x / 2.0f;
    else if (m_context.player.getPosition().x + m_context.player.getSize().x / 2.0f > m_background.getSize().x - view.getSize().x / 2.0f)
        viewPos.x = m_background.getSize().x - view.getSize().x / 2.0f;
    else
        viewPos.x = m_context.player.getPosition().x + m_context.player.getSize().x / 2.0f;

    if (m_context.player.getPosition().y + m_context.player.getSize().y / 2.0f < view.getSize().y / 2.0f)
        viewPos.y = view.getSize().y / 2.0f;
    else if (m_context.player.getPosition().y + m_context.player.getSize().y / 2.0f > m_background.getSize().y - view.getSize().y / 2.0f)
        viewPos.y = m_background.getSize().y - view.getSize().y / 2.0f;
    else
        viewPos.y = m_context.player.getPosition().y + m_context.player.getSize().y / 2.0f;

    view.setCenter(viewPos);
    m_context.window.setView(view);
    m_context.gui.setView(view);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::draw()
{
    m_context.window.clear();
    m_context.window.draw(m_backgroundSprite);
    m_context.gui.draw();
    m_context.window.display();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
