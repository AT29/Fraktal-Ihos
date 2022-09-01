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
#include <algorithm>
#include <vector>

// Simple gui system

#define BG_STD_COLOR sf::Color(196,196,196)
#define BG_HOVER_COLOR sf::Color(128,128,128)
#define BG_CLICK_COLOR sf::Color(64,64,64)

#define DEFAULT_GUI_FONT_PATH "./resources/gui font.ttf"

namespace GUI
{
	// singleton

	class Font
	{

	public:

		Font(const Font&) = delete;

		static Font& Get();

		static void load(std::string path = DEFAULT_GUI_FONT_PATH);

		sf::Font font;

	private:

		Font();

		void ILoad(std::string &path);

	private:

		bool fontLoaded;

	};

	/////////////////////////////////////////////////////////////////////////////

	class Button : public sf::Drawable
	{

	public:
		
		Button();
		void create(std::string title, sf::Vector2f position, sf::Vector2f size);

		void setTitle(std::string title);
		void setPosition(sf::Vector2f position);
		sf::Vector2f getPosition();
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize();

		std::string update(sf::RenderWindow &window);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		
	private:

		sf::Text m_text;
		sf::RectangleShape m_rect;

		struct MouseAttributes
		{
			bool clickedInside = false;
			bool clickedOutside = false;
			bool onceOut = false;
			bool hovers = false;
			bool pressed = false;
			bool released = false;
			bool once = false;
		} mouse;
	};

	/////////////////////////////////////////////////////////////////////////////

	class Button_List : public sf::Drawable
	{

	public:

		Button_List();

		void create(sf::Vector2f position);

		void setPosition(sf::Vector2f position);

		void push_back(GUI::Button button);
		void pop_back();
		void clear();

		std::vector<Button>& get(); // don't mess with that if you can.

		std::string update(sf::RenderWindow& window);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:

		std::vector<Button> m_buttons;
		sf::Vector2f m_position;

	};
}