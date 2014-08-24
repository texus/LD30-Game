
#include "Game.hpp"
#include "Level.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Game() :
    m_window{{800, 600}, "LD30"},
    m_gui   {m_window},
    m_player{m_gui}
{
    m_gui.setGlobalFont("resources/TGUI/DejaVuSans.ttf");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::run()
{
    m_gui.add(tgui::Picture::create("resources/Story" + m_gameScreen + ".png"));

    std::unique_ptr<sf::Clock> waitClock{new sf::Clock()};
    const sf::Time waitTime = sf::milliseconds(500);
    bool allowContinue = false;

    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();

            if (allowContinue)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    m_gui.removeAllWidgets();

                    if (m_gameScreen == "Intro")
                    {
                        m_gameScreen = "Level1";
                    }
                    else if (m_gameScreen == "GameOver")
                    {
                        m_gameScreen = m_oldGameScreen;
                    }
                    else if (m_gameScreen == "Level1")
                    {
                        Level level{{m_window, m_gui, m_player}, "Level1"};
                        if (level.play())
                            m_gameScreen = "EndLevel1";
                        else
                        {
                            m_oldGameScreen = "Level1";
                            m_gameScreen = "GameOver";
                        }
                    }
                    else if (m_gameScreen == "EndLevel1")
                    {
                        m_gameScreen = "Level2";
                    }
                    else if (m_gameScreen == "Level2")
                    {
                        Level level{{m_window, m_gui, m_player}, "Level2"};
                        if (level.play())
                            m_gameScreen = "EndLevel2";
                        else
                        {
                            m_oldGameScreen = "Level2";
                            m_gameScreen = "GameOver";
                        }
                    }
                    else if (m_gameScreen == "EndLevel2")
                    {
                        m_gameScreen = "End";
                    }
                    else if (m_gameScreen == "End")
                    {
                        m_window.close();
                    }

                    m_gui.add(tgui::Picture::create("resources/Story" + m_gameScreen + ".png"));

                    waitClock = std::unique_ptr<sf::Clock>{new sf::Clock()};
                    allowContinue = false;
                }
            }
        }

        if (waitClock != nullptr && waitClock->getElapsedTime() > waitTime)
        {
            waitClock = nullptr;
            allowContinue = true;

            auto labelContinue = tgui::Label::create();
            labelContinue->setText("Press any key to continue");
            labelContinue->setTextColor(sf::Color::White);
            labelContinue->setTextSize(18);
            labelContinue->setPosition((tgui::bindWidth(m_gui) - tgui::bindWidth(labelContinue)) / 2.0f, 520);
            m_gui.add(labelContinue);
        }

        m_window.clear();
        m_gui.draw();
        m_window.display();

        sf::sleep(sf::milliseconds(1));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
