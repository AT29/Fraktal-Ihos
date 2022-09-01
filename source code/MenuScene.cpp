#include "Scenes.h"

#include "Mandelbrot.h"
#include "GUI.h"

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// This function creates a list of .wav files in a specific directory and represents these files as buttons.

void listFiles(GUI::Button_List &buttons)
{
    GUI::Button button;
    button.create("", sf::Vector2f(0, 0), sf::Vector2f(600, 50));
    std::string path = "./music/";
    for (const auto& entry : fs::directory_iterator(path))
    {
        std::string temp = entry.path().filename().string();

        if (temp.find(".wav") != std::string::npos)
        {
            button.setTitle(temp.substr(0, temp.size() - 4));
            buttons.push_back(button);
        }
    }
}

// The program menu is handled in the function below.

std::string Scenes::menu(std::string& shaderPath)
{
    m::Mandelbrot fractal;
    fractal.create(sf::Vector2u(800, 800), sf::Vector2u(800, 800));
    //fractal.GetParams().path("./resources/MandelbrotMenu.frag");
    fractal.GetParams().path(shaderPath.c_str());
    fractal.GetParams().center(sf::Vector2m(0, -1.7482));
    fractal.GetParams().scale(2.79514e-06);

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100,150));

    sf::Text text,text2;
    text.setFont(GUI::Font::Get().font);
    text.setCharacterSize(25);
    text.setOutlineColor(sf::Color(0,0,0));
    text.setOutlineThickness(20);

    //English translation:
    text.setString("  Put your songs in the \"music\" folder, and they will appear right here:");
    //Polish translation:
    //text.setString(L"Wrzuæ swoje piosenki do folderu \"music\", a pojawi¹ siê tu:");

    text.setOrigin(text.getGlobalBounds().width/2., text.getGlobalBounds().height / 2.);
    text.setPosition(400, 50);

    text2 = text;
    text2.setCharacterSize(20);

    //English translation:
    text2.setString("You don't know what's going on? Read the \"INSTRUCTION.txt\" file.");
    //Polish translation:
    //text2.setString(L"Nie wiesz o co chodzi? Przeczytaj plik \"INSTRUCTION.txt\".");

    text2.setOrigin(text2.getGlobalBounds().width / 2., text2.getGlobalBounds().height / 2.);
    text2.setPosition(400, 100);

    sf::Clock refresher;

    std::string song;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (refresher.getElapsedTime().asSeconds() > 0.5)
        {
            refresher.restart();
            buttons.clear();
            listFiles(buttons);
        }

        song = buttons.update(window);
        if (!song.empty()) window.close();

        fractal.update(0, 0);

        window.clear();
        window.draw(fractal);
        window.draw(buttons);
        window.draw(text);
        window.draw(text2);
        window.display();

    }

    return song;
}
