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