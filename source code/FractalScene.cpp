#include "Scenes.h"
#include "Mandelbrot.h"
#include "Sampler.h"
#include <string>
#include <math.h>

#define WINDOW_RESOLUTION {1280,720}
#define WINDOW_FRACTAL_RESOLUTION {1280,720}

#define FULLSCREEN_RESOLUTION {1920,1080}
#define FULLSCREEN_FRACTAL_RESOLUTION {1920,1080}

// Signum / sgn

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// Quick fix: Returns maximum value of the given soundwave. Future version will replace it with FFT algorithm.

float specMax(std::vector<sf::Int16>& buff)
{
    sf::Int16 temp = 0;
    for (sf::Uint64 i = 0; i < buff.size(); i++)
    {
        temp = std::max(temp, static_cast<sf::Int16>(abs(buff[i])) );
    }

    return float(temp) / float(0x8000);
}


// Events

void eventFullscreen(sf::RenderWindow& window, m::Mandelbrot& fractal, sf::VideoMode& mode, sf::VideoMode& fractalResolution, bool fullscreen);

bool eventPause(SoundVisualisalizer & vis, float &pausePitch, bool songPause, m::Mandelbrot& fractal);

void eventEscapePressed(sf::RenderWindow& window, SoundVisualisalizer& vis, sf::Color& color, float& escapePitch);

void eventEscapeNotPressed(sf::RenderWindow& window, SoundVisualisalizer& vis, sf::Color& color, float& escapePitch);


// Main scene #######################################################################################################


void Scenes::fractal(std::string& songName, std::string& shaderPath)
{

    // Called once.

    // Flags and additional variables for nice effects

    bool fullscreen = true;
    bool screen_mode_flip = true;
    bool screen_mode_flip_2 = false;

    bool songPause = false;
    bool songPause_flip = true;
    bool songPause_flip_2 = false;

    bool escape = false;

    float pausePitch = 1.f;
    float escapePitch = 1.f;

    float songIntensityAccumulator = 0;


    sf::Color color = sf::Color::Transparent;

    sf::Clock shaderClock;

    // Music

    sf::SoundBuffer buffer;
    buffer.loadFromFile("./music/"+songName+".wav");
    SoundVisualisalizer vis(buffer);

    // Window

    sf::VideoMode mode( FULLSCREEN_RESOLUTION );
    window.create(mode, "Fraktal Ihos", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Fractal
    sf::VideoMode fractalResolution; // This changes when the user wants to switch "fullscreen / window" mode.
    m::Mandelbrot fractal;
    fractal.create(sf::Vector2u(FULLSCREEN_RESOLUTION), sf::Vector2u(FULLSCREEN_FRACTAL_RESOLUTION));
    fractal.GetParams().path(shaderPath.c_str());

    // other

    sf::Vector2i lastMousePos;
    sf::Vector2i lastMousePivot;
    sf::Cursor cursor;
    bool grabbed=false;
    bool lmb_released_flip=false;

    sf::Vector2m displacement;
    mType zoom = 0;

    bool firstRun = true;

    sf::Clock firstRunTimer;

    // Scene loop:

    while (window.isOpen())
    {
        if (firstRun)
        {
            // stupidity
            if (firstRunTimer.getElapsedTime().asSeconds() > 0.5f)
            { 
                firstRun = false; vis.play();
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta < 0)
                {
                    if (fractal.GetParams().scale() < 0.1)
                        zoom = -float(event.mouseWheelScroll.delta) / 25.;
                }
                else
                {
                    if (fractal.GetParams().scale() > 2.26006e-07)
                        zoom = -float(event.mouseWheelScroll.delta) / 25.;
                }
                
            }
        }

        sf::Vector2m delta;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            delta = sf::Vector2m(sf::Mouse::getPosition(window) - lastMousePos);
            delta.y *= -1;

            fractal.GetParams().displacementAdd(delta);

            lastMousePos = sf::Mouse::getPosition(window);
            displacement = sf::Vector2m(0, 0);
            cursor.loadFromSystem(sf::Cursor::Type::Cross);
            grabbed = true;
            lmb_released_flip = true;
        }
        else
        {
            if (lmb_released_flip) 
            {
                cursor.loadFromSystem(sf::Cursor::Type::Arrow);
                sf::Vector2m d = sf::Vector2m(sf::Mouse::getPosition(window)-lastMousePos); d.y *= -1;
                displacement += sf::Vector2m(d);
                grabbed = true;
                lmb_released_flip = false;
            }
            lastMousePos = sf::Mouse::getPosition(window);
            lastMousePivot = lastMousePos;
        }

        if (grabbed)
        {
            window.setMouseCursor(cursor);
            grabbed = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && window.hasFocus()) fractal.GetParams().displacement(0,0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && window.hasFocus())
        {
            if (screen_mode_flip)
            {
                fullscreen = !fullscreen;
                screen_mode_flip_2 = true;
            }
            screen_mode_flip = false;
        }
        else screen_mode_flip = true;

        if (screen_mode_flip_2)
        {
            screen_mode_flip_2 = false;
            
            if (fullscreen)
            {
                mode = sf::VideoMode(FULLSCREEN_RESOLUTION);
                fractalResolution = sf::VideoMode(FULLSCREEN_FRACTAL_RESOLUTION);
            }
            else
            {
                mode = sf::VideoMode(WINDOW_RESOLUTION);
                fractalResolution = sf::VideoMode(WINDOW_FRACTAL_RESOLUTION);
            }
            eventFullscreen(window, fractal, mode, fractalResolution, fullscreen);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && window.hasFocus())displacement.y -= 0.5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && window.hasFocus())displacement.y += 0.5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && window.hasFocus())displacement.x += 0.5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && window.hasFocus())displacement.x -= 0.5;
        fractal.GetParams().displacementAdd(displacement);
        displacement.x *= 0.95;
        displacement.y *= 0.95;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && window.hasFocus())
        {
            if (songPause_flip)
            {
                songPause = !songPause;
                songPause_flip_2 = true;
            }
            songPause_flip = false;
        }
        else songPause_flip = true;
            
        if (songPause_flip_2 && !escape)
        {
            songPause_flip_2 = eventPause(vis, pausePitch, songPause, fractal);
        }
        
        if (fractal.GetParams().scale() > 2.26006e-07)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && window.hasFocus()) zoom -= 0.001;
        }
        else if(fractal.GetParams().scale() < 2.26006e-07) fractal.GetParams().scaleMultiply(1.001);

        if (fractal.GetParams().scale() < 0.1)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && window.hasFocus()) zoom += 0.001;
        }
        else if (fractal.GetParams().scale() > 0.1) fractal.GetParams().scaleMultiply(0.999);

        fractal.GetParams().scaleMultiply(1+zoom);
        zoom *= 0.95;

        vis.calculateElapsedSamples();
        float tempIntensity = specMax(vis.getSampleArray());
        songIntensityAccumulator += tempIntensity;
        fractal.update(tempIntensity, songIntensityAccumulator);

        window.clear(sf::Color::Black);
        window.draw(fractal);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && window.hasFocus())
        {
            escape = true;
            eventEscapePressed(window, vis, color, escapePitch);
        }
        else
        {
            escape = false;
            eventEscapeNotPressed(window, vis, color, escapePitch);
        }

        window.display();

    }
    vis.stop();
}



void eventFullscreen(sf::RenderWindow& window, m::Mandelbrot& fractal, sf::VideoMode& mode, sf::VideoMode& fractalResolution, bool fullscreen)
{
    sf::Uint32 settings;

    if (fullscreen) settings = sf::Style::Fullscreen;
    else settings = sf::Style::Titlebar | sf::Style::Close;
    window.create(mode, "Math is art", settings);
    window.setFramerateLimit(60);

    fractal.create(
        sf::Vector2u(mode.width, mode.height),
        sf::Vector2u(fractalResolution.width, fractalResolution.height),
        false
    );
}

bool eventPause(SoundVisualisalizer& vis, float& pausePitch, bool songPause,m::Mandelbrot &fractal)
{
    if (songPause)
    {
        if (pausePitch == 0.f)
        {
            if (vis.getStatus() == sf::Sound::Playing)
            {
                vis.pause();
            }

            return false;
        }
        else if (pausePitch != 0)
        {
            pausePitch -= 0.01f;
            pausePitch = pausePitch < 0.f ? 0.f : pausePitch;
            vis.setPitch(pausePitch);
        }
    }
    else
    {
        if (pausePitch == 1.f)
        {
            return false;
        }
        else if (pausePitch != 1)
        {
            if (vis.getStatus() == sf::Sound::Paused)
            {
                vis.play();
            }
            
            pausePitch += 0.01f;
            pausePitch = pausePitch > 1.f ? 1.f : pausePitch;
            vis.setPitch(pausePitch);
        }
    }

    return true;
}

void eventEscapePressed(sf::RenderWindow& window, SoundVisualisalizer& vis, sf::Color& color, float& escapePitch)
{
    sf::RectangleShape rect(sf::Vector2f(window.getSize()));
    if (!(color.a == 255))
    {
        int temp = int(color.a) + 3;
        color.a = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;
        rect.setFillColor(color);
        window.draw(rect);
    }
    else window.close();

    if (escapePitch != 0)
    {
        escapePitch -= 0.0196f;
        escapePitch = escapePitch < 0.f ? 0.f : escapePitch;
        vis.setPitch(escapePitch);
    }
}

void eventEscapeNotPressed(sf::RenderWindow& window, SoundVisualisalizer& vis, sf::Color &color, float& escapePitch)
{
    sf::RectangleShape rect(sf::Vector2f(window.getSize()));
    if (!(color.a == 0))
    {
        int temp = int(color.a) - 5;
        color.a = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;
        rect.setFillColor(color);
        window.draw(rect);
    }

    if (escapePitch != 1)
    {
        escapePitch += 0.0196f;
        escapePitch = escapePitch > 1.f ? 1.f : escapePitch;
        vis.setPitch(escapePitch);
    }
}