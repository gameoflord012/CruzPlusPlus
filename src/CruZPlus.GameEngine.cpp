﻿#include <SFML/Window.hpp>

int main()
{
    sf::Window window(sf::VideoMode(sf::Vector2u(800, 600)), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
        }
    }

    return 0;
}