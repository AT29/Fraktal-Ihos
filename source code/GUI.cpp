#include "GUI.h"

GUI::Font::Font() { font.loadFromFile(DEFAULT_GUI_FONT_PATH); }

void GUI::Font::ILoad(std::string &path)
{
	if (this->font.loadFromFile(path))
		this->fontLoaded = true;
}

GUI::Font& GUI::Font::Get()
{
	static GUI::Font instance;
	return instance;
}

void GUI::Font::load(std::string path)
{
	Get().ILoad(path);
}

/////////////////////////////////////////////////////////////////////////////

GUI::Button::Button()
{
	m_text.setFont(GUI::Font::Get().font);
	m_text.setFillColor(sf::Color::White);
	m_text.setOutlineThickness(3);
	m_text.setOutlineColor(sf::Color::Black);
	m_text.setCharacterSize(25);

	m_rect.setFillColor(BG_STD_COLOR);
	m_rect.setOutlineThickness(2);
	m_rect.setOutlineColor(sf::Color::Black);
	m_rect.setSize(sf::Vector2f(89.f,55.f));
}

void GUI::Button::create(std::string title, sf::Vector2f position, sf::Vector2f size)
{
	setTitle(title);
	setPosition(position);
	setSize(size);
}

void GUI::Button::setTitle(std::string title)
{
	m_text.setString(title);
	m_text.setOrigin(m_text.getGlobalBounds().width / 2.f, m_text.getGlobalBounds().height / 2.f);
}

void GUI::Button::setPosition(sf::Vector2f position)
{
	m_rect.setPosition(position);

	m_text.setPosition(
		m_rect.getPosition() +
		sf::Vector2f(m_rect.getGlobalBounds().width / 2.f, m_rect.getGlobalBounds().height / 2.f)
	);
}

sf::Vector2f GUI::Button::getPosition()
{
	return m_rect.getPosition();
}

void GUI::Button::setSize(sf::Vector2f size)
{
	m_rect.setSize(size);
	m_text.setPosition(
		m_rect.getPosition() +
		sf::Vector2f(m_rect.getGlobalBounds().width / 2.f, m_rect.getGlobalBounds().height / 2.f)
	);
}

sf::Vector2f GUI::Button::getSize()
{
	return m_rect.getSize();
}

std::string GUI::Button::update(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	bool hovers  = m_rect.getGlobalBounds().contains(sf::Vector2f(mousePos));
	bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	if (!hovers)
	{
		m_text.setFillColor(sf::Color::White);
	}
	else if(window.hasFocus())
	{
		if (pressed)
		{
			return m_text.getString();

			m_text.setFillColor(BG_CLICK_COLOR);
		}
		else
		{
			m_text.setFillColor(sf::Color(128, 128, 128));
		}
	}
	
	return "";
}

void GUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(m_rect, states);
	target.draw(m_text, states);
}

/////////////////////////////////////////////////////////////////////////////

GUI::Button_List::Button_List(){}

void GUI::Button_List::create(sf::Vector2f position)
{
	clear();
	m_position = position;
}

void GUI::Button_List::setPosition(sf::Vector2f position)
{
	m_position = position;
	double sum = 0;
	for (size_t i=0; i<m_buttons.size(); i++)
	{
		m_buttons[i].setPosition(m_position + sf::Vector2f(0.,sum) );
		sum += double(m_buttons[i].getSize().y) + 10.;
	}
}

void GUI::Button_List::push_back(GUI::Button button)
{
	if (!m_buttons.empty())
		button.setPosition(m_buttons.back().getPosition() + sf::Vector2f(0,m_buttons.back().getSize().y) );
	
	m_buttons.push_back(button);
	setPosition(m_position);
}

void GUI::Button_List::pop_back()
{
	m_buttons.pop_back();
}

void GUI::Button_List::clear()
{
	m_buttons.clear();
}

std::vector<GUI::Button>& GUI::Button_List::get()// don't mess with that if you can.
{
	return m_buttons;
} 

std::string GUI::Button_List::update(sf::RenderWindow &window)
{
	std::string result,t;
	for (auto& e : m_buttons) { t = e.update(window); if (!t.empty())result = t; }
	return result;
}

void GUI::Button_List::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto e : m_buttons) target.draw(e, states);
}