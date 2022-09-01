/////////////////////////////////////////////////////////////////////////////////////
//
//  MIT License
//  
//  Fraktal Ihos - Copyright(c) 2022 Antoni Tretiakov - antonitre29@gmail.com
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this softwareand associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//  
//  The above copyright noticeand this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////

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

void initText(sf::Text& text, const std::string& title = "text", unsigned int size = 20, float x = 0, float y = 0)
{
    text.setFont(GUI::Font::Get().font);
    text.setCharacterSize(size);
    text.setOutlineColor(sf::Color(25, 25, 25));
    text.setOutlineThickness(20);

    text.setString(title);

    text.setOrigin(text.getGlobalBounds().width / 2., text.getGlobalBounds().height / 2.);
    text.setPosition(x, y);
}

// The program menu is handled in the function below.

void themeSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal);
void resolutionSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal);
void graphicsSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal);
void settings(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal);
void songSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal);

void Scenes::menu()
{
    sf::Text text;
    initText(text, "  Fraktal Ihos", 50, 400, 100);

    m::Mandelbrot fractal;
    fractal.create(sf::Vector2u(800, 800), sf::Vector2u(800, 800));
    fractal.GetParams().path(options.path1.c_str());
    fractal.GetParams().center(sf::Vector2m(0, -1.7482));
    fractal.GetParams().scale(2.79514e-06);

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100,150));

    std::string names[] = { "Begin", "Settings"};
    for (int i = 0; i < 2; i++)
    {
        GUI::Button button;
        button.create(names[i], sf::Vector2f(0, 0), sf::Vector2f(600, 50));
        buttons.push_back(button);
    }

    std::string selected;

    window.create(sf::VideoMode(800, 800), "Fraktal Ihos", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30);

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        selected = buttons.update(window);
        if (!selected.empty())
        {
            if (selected == "Begin") songSelection(window, options, fractal);
            else if (selected == "Settings") settings(window, options, fractal);
        }

        fractal.update(0, 0);

        window.clear();
        window.draw(fractal);
        window.draw(text);
        window.draw(buttons);
        window.display();

    }

}

void songSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal)
{
    sf::Text text;
    initText(text, "  Put your songs in the \"music\" folder, and they will appear right here:", 25, 400, 50);
    //Polish translation:
    //initText(text,L"Wrzuæ swoje piosenki do folderu \"music\", a pojawi¹ siê tu:", 35, 400, 50);

    sf::Text text2;
    initText(text2, "You don't know what's going on? Read the \"README.md\" file.", 20, 400, 100);
    //Polish translation:
    //initText(text2,L"Nie wiesz o co chodzi? Przeczytaj plik \"README.md\".", 20, 400, 100);

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100, 150));

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
        window.draw(text);
        window.draw(text2);
        window.draw(buttons);
        window.display();

    }

    options.songName = song;

}

void themeSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal)
{
    sf::Text text;
    initText(text, "  Select a theme:", 35, 400, 50);
    //Polish translation:
    //initText(text,L"  Wybierz motyw:", 35, 400, 50);

    sf::Text text2;
    initText(text2, "  Currently selected: " + options.themeName, 20, 400, 100);
    //Polish translation:
    //initText(text2,L"  Aktualnie wybrano: "+ options.themeName, 20, 400, 100);

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100, 150));

    std::string names[] = { "Blue Palm Trees", "Cosmic Blobs", "Sunshine Tiles" };
    for (int i = 0; i < 3; i++)
    {
        GUI::Button button;
        button.create(names[i], sf::Vector2f(0, 0), sf::Vector2f(600, 50));
        buttons.push_back(button);
    }

    std::string selectedTheme;

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

        fractal.update(0, 0);

        window.clear(sf::Color(25, 25, 25));
        window.draw(fractal);
        window.draw(text);
        window.draw(text2);
        window.draw(buttons);
        window.display();

    }

    if (selectedTheme == "Blue Palm Trees")
    {
        options.themeName = "Blue Palm Trees";
        options.path1 = "./resources/palms/MandelbrotMenu.frag";
        options.path2 = "./resources/palms/MandelbrotPlay.frag";
    }
    else if (selectedTheme == "Cosmic Blobs")
    {
        options.themeName = "Cosmic Blobs";
        options.path1 = "./resources/blobs/MandelbrotMenu.frag";
        options.path2 = "./resources/blobs/MandelbrotPlay.frag";
    }
    else if (selectedTheme == "Sunshine Tiles")
    {
        options.themeName = "Sunshine Tiles";
        options.path1 = "./resources/tiles/MandelbrotMenu.frag";
        options.path2 = "./resources/tiles/MandelbrotPlay.frag";
    }

    fractal.GetParams().path(options.path1.c_str());
}

void resolutionSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal)
{
    sf::Text text;
    initText(text, "  Select your screen resolution:", 35, 400, 50);
    //Polish translation:
    //initText(text,L"  Wybierz rozdzielczoœæ ekranu:", 35, 400, 50);
    sf::Text text2;
    std::string currentResolution = std::to_string(options.resolutionX) + "x" + std::to_string(options.resolutionY);
    initText(text2, "  Currently selected: " + currentResolution, 20, 400, 100);
    //Polish translation:
    //initText(text2,L"  Aktualnie wybrano: " + currentResolution, 20, 400, 100);

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100, 150));
    std::string names[] = {
        "1920x1080", "1680x1050", "1600x900", "1440x900",
        "1400x1050", "1366x768", "1360x768", "1280x1024",
        "1280x960", "1280x800", "1280x768", "1280x720", "1280x600"
    };
    for (int i = 0; i < 13; i++)
    {
        GUI::Button button;
        button.create(names[i], sf::Vector2f(0, 0), sf::Vector2f(600, 30));
        buttons.push_back(button);
    }

    std::string selectedResolution;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        selectedResolution = buttons.update(window);
        if (!selectedResolution.empty()) break;

        fractal.update(0, 0);

        window.clear(sf::Color(25, 25, 25));
        window.draw(fractal);
        window.draw(text);
        window.draw(text2);
        window.draw(buttons);
        window.display();
    }

    std::string textResX, textResY;
    bool next = false;
    for (auto a : selectedResolution)
    {
        if (a >= '0' && a <= '9') next ? textResY.push_back(a) : textResX.push_back(a);
        else next = true;
    }

    options.resolutionX = atoi(textResX.c_str());
    options.resolutionY = atoi(textResY.c_str());
}

void graphicsSelection(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal)
{
    sf::Text text;
    initText(text, "  Select graphics quality:", 35, 400, 50);
    //Polish translation:
    //initText(text,L"  Wybierz jakoœæ grafiki:", 35, 400, 50);

    std::string currentQuality;
    switch (options.graphicsLevel)
    {
    case 1: currentQuality = "low"; break;
    case 2: currentQuality = "medium"; break;
    case 3: currentQuality = "high"; break;
    }

    sf::Text text2;
    initText(text2, "  Currently selected: " + currentQuality, 20, 400, 100);
    //Polish translation:
    //initText(text2,L"  Aktualnie wybrano: " + currentQuality, 20, 400, 100);

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100, 150));
    std::string names[] = { "low", "medium", "high" };
    for (int i = 0; i < 3; i++)
    {
        GUI::Button button;
        button.create(names[i], sf::Vector2f(0, 0), sf::Vector2f(600, 50));
        buttons.push_back(button);
    }

    std::string selectedQuality;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        selectedQuality = buttons.update(window);
        if (!selectedQuality.empty()) break;

        fractal.update(0,0);

        window.clear(sf::Color(25, 25, 25));
        window.draw(fractal);
        window.draw(text);
        window.draw(text2);
        window.draw(buttons);
        window.display();
    }

    if (selectedQuality == "low") options.graphicsLevel = 1;
    else if (selectedQuality == "medium") options.graphicsLevel = 2;
    else if (selectedQuality == "high") options.graphicsLevel = 3;
}

void settings(sf::RenderWindow& window, Scenes::Options& options, m::Mandelbrot& fractal)
{
    sf::Text text;
    initText(text, "  Settings:", 35, 400, 50);
    //Polish translation:
    //initText(text,L"  Ustawienia:", 35, 400, 50);

    GUI::Button_List buttons;
    buttons.setPosition(sf::Vector2f(100, 150));
    std::string names[] = { "Themes", "Screen Resolution", "Graphics Quality", "<< MAIN MENU <<" };
    for (int i = 0; i < 4; i++)
    {
        GUI::Button button;
        button.create(names[i], sf::Vector2f(0, 0), sf::Vector2f(600, 50));
        buttons.push_back(button);
    }

    std::string selected;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        selected = buttons.update(window);
        if (!selected.empty())
        {
            if (selected == "Themes") themeSelection(window, options, fractal);
            else if (selected == "Screen Resolution") resolutionSelection(window, options, fractal);
            else if (selected == "Graphics Quality") graphicsSelection(window, options, fractal);
            else if (selected == "<< MAIN MENU <<") break;
        }

        fractal.update(0, 0);

        window.clear(sf::Color(25, 25, 25));
        window.draw(fractal);
        window.draw(text);
        window.draw(buttons);
        window.display();
    }

}