#include "Game.h"

#include <cassert>
#include <cstdio>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>

#include <box2d/box2d.h>

#if CRUZ_EDITOR
#include <imgui-SFML.h>
#include <imgui.h>
#endif

#include "CruZPlus/BodyFactory.h"
#include "CruZPlus/Configure.h"
#include "CruZPlus/GameEntity/EntityWorld.h"
#include "CruZPlus/Helper/TimeHelper.h"
#include "CruZPlus/Input.h"
#include "CruZPlus/Settings.h"
#include "CruZPlus/TextureManager.h"

namespace CruZ
{
Game::Game()
{
    m_entityWorld = new EntityWorld;
    m_b2World = new b2World(b2Vec2(0, -10));

    m_textureManager = new TextureManager;
    m_bodyFactory = new BodyFactory(*m_b2World);
    m_input = new Input;

    Instances::set(this);
    Instances::set(m_bodyFactory);
    Instances::set(m_textureManager);
    Instances::set(m_input);
    Instances::set(m_entityWorld);
}

b2World *Game::getB2World()
{
    assert(m_b2World != nullptr);
    return m_b2World;
}

EntityWorld *Game::getEntityWorld()
{
    return m_entityWorld;
}

sf::View *Game::getView()
{
    return m_view;
}

void Game::run()
{
    // init window
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1920, 1080)), "My window");
#if CRUZ_EDITOR
    assert(ImGui::SFML::Init(window));
#endif

    m_view = new sf::View({0, 0}, window.getDefaultView().getSize());
    m_view->zoom(Setting::ZOOM);
    m_view->setCenter({0, 0});

    sf::Clock gameClock;
    float elapsedSeconds = 0;

    // printf("%s\n", std::filesystem::current_path().string().c_str());

    while (window.isOpen())
    {
        sf::Event event;
        while (event = window.pollEvent(), event)
        {
            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
#if CRUZ_EDITOR
            ImGui::SFML::ProcessEvent(window, event);
#endif
            m_input->processEvent(event);
        }

        elapsedSeconds += gameClock.restart().asSeconds();
        while (elapsedSeconds > Setting::UPDATE_DURATION)
        {
            elapsedSeconds -= Setting::UPDATE_DURATION;
            {
                m_input->updateInput();
#if CRUZ_EDITOR
                ImGui::SFML::Update(window, Helper::convertToTime(Setting::UPDATE_DURATION));
#endif
                m_b2World->Step(Setting::UPDATE_DURATION, 6, 2);
                m_entityWorld->updateAll(Setting::UPDATE_DURATION);
            }
        }
#if CRUZ_EDITOR
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::GetIO().IniFilename = CRUZ_BINARY_DIR "/imgui.ini";
#endif
        window.clear(sf::Color::Blue);
        {
            window.setView(*m_view);
            m_entityWorld->renderAll(window);
#if CRUZ_EDITOR
            ImGui ::SFML::Render(window);
#endif
        }
        window.display();
    }
#if CRUZ_EDITOR
    ImGui::SFML::Shutdown();
#endif
}

Game::~Game()
{
    delete m_b2World;
    delete m_entityWorld;
    delete m_view;
}
} // namespace CruZ
