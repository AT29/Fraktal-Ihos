#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Scenes
{
public:

	void menu();
	void fractal();

public:

	sf::RenderWindow window;

	struct Options
	{
		std::string songName = "";

		std::string themeName = "blue palm trees";

		std::string path1 = "./resources/palms/MandelbrotMenu.frag";
		std::string path2 = "./resources/palms/MandelbrotPlay.frag";

		unsigned int resolutionX = 1920;
		unsigned int resolutionY = 1080;

		int graphicsLevel = 3; // high

	} options;

	

};