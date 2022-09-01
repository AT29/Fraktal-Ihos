#include "Scenes.h"

#include "Mandelbrot.h"
#include "GUI.h"

#include <SFML/Graphics.hpp>
#include <string>

void addButtonsToList(GUI::Button_List& buttons)
{
    std::string names[] = { "blue palm trees", "cosmic blobs", "sunshine tiles" };
    for (int i = 0; i < 3; i++)
    {
        GUI::Button button;
        button.create("", sf::Vector2f(0, 0), sf::Vector2f(600, 50));
        button.setTitle(names[i]);
        buttons.push_back(button);
    }
    
}

PathContainer Scenes::theme()
{
    std::string selectedTheme;

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100, 150));
    addButtonsToList(buttons);

    sf::Text text;
    text.setFont(GUI::Font::Get().font);
    text.setCharacterSize(35);
    text.setOutlineColor(sf::Color(25, 25, 25));
    text.setOutlineThickness(20);

    //English translation:
    text.setString("  Please select a theme:");
    //Polish translation:
    //text.setString(L"Proszê wybierz motyw:");

    text.setOrigin(text.getGlobalBounds().width / 2., text.getGlobalBounds().height / 2.);
    text.setPosition(400, 50);

    window.create({ 800, 800 }, "Fraktal Ihos", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30);

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        selectedTheme = buttons.update(window);
        if (!selectedTheme.empty()) break;

        window.clear(sf::Color(25,25,25));
        window.draw(buttons);
        window.draw(text);
        window.display();

    }

    PathContainer paths;
    std::string resPath = "./resources/";
    if (selectedTheme == "blue palm trees") paths = { resPath+"palms/MandelbrotMenu.frag",resPath+"palms/MandelbrotPlay.frag" };
    else if(selectedTheme == "cosmic blobs") paths = { resPath+"blobs/MandelbrotMenu.frag",resPath+"blobs/MandelbrotPlay.frag" };
    else if(selectedTheme == "sunshine tiles") paths = { resPath+"tiles/MandelbrotMenu.frag",resPath+"tiles/MandelbrotPlay.frag" };

    return paths;
}