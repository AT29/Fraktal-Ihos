
#include "Mandelbrot.h"

using namespace m;
using namespace std;

// Mandelbrot

m::Mandelbrot::Mandelbrot()
	:m_firstRun(true)
{
	m_attributes.resetToDefault();
	m_fractalInfo.m_ptr = &m_attributes;
	m_spriteInfo.m_ptr = &m_attributes;
}

m::Mandelbrot::Mandelbrot(sf::Vector2u size, sf::Vector2u resolution)
	: m_firstRun(true)
{
	m_attributes.resetToDefault();

	m_fractalInfo.m_ptr = &m_attributes;
	m_spriteInfo.m_ptr = &m_attributes;

	this->init(size, resolution, true);

	m_firstRun = false;
}

void m::Mandelbrot::create(sf::Vector2u size, sf::Vector2u resolution, bool reset)
{
	if (!m_firstRun)
	{
		m_attributes.m_scale *= mType(m_attributes.m_resolution.x) / mType(resolution.x);
	}

	this->init(size, resolution, reset);
	m_firstRun = false;
}

void m::Mandelbrot::init(sf::Vector2u &size, sf::Vector2u&resolution, bool reset)
{
	// Assigning variables:

	if(reset) m_attributes.resetToDefault();

	m_attributes.m_size = size;
	m_attributes.m_resolution = resolution;

	// Other.

	m_attributes.m_renderTexture.create(m_attributes.m_resolution.x, m_attributes.m_resolution.y);
	m_attributes.m_texture.create(m_attributes.m_resolution.x, m_attributes.m_resolution.y);
	m_attributes.m_sprite.setTexture(m_attributes.m_texture);

	float xSpriteScale = m_attributes.sizeProportionX();
	float ySpriteScale = m_attributes.sizeProportionY();
	m_attributes.m_sprite.setScale(xSpriteScale, ySpriteScale);

	m_attributes.m_shader.loadFromFile(default_shader_path,sf::Shader::Fragment);
	//m_attributes.m_shader.

	// If one forgets to use 'update' method, one sees a static image of the fractal.
	// That is why the code beneath exists.

	m_attributes.m_shader.setUniform("sh_scale", (float)m_attributes.m_scale);
	m_attributes.m_shader.setUniform("sh_time", m_clock.getElapsedTime().asSeconds() );
	m_attributes.m_shader.setUniform("sh_music", 0.f);
	m_attributes.m_shader.setUniform("sh_precision", (float)m_attributes.m_precision);
	m_attributes.m_shader.setUniform("sh_relativeCenter",
		sf::Glsl::Vec2(m_attributes.m_relativeCenter)
	);
	m_attributes.m_shader.setUniform("sh_resolutionCenter",
		sf::Glsl::Vec2((float)m_attributes.m_resolution.x / 2, (float)m_attributes.m_resolution.y / 2)
	);
	m_attributes.m_shader.setUniform("sh_displacement",
		sf::Glsl::Vec2(m_attributes.m_displacement)
	);

}

void m::Mandelbrot::update(float intensity, float accum)
{
	// If you're going to change shader parameters, you would likely want to use this function.

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) m_attributes.m_shClockMultiplier -= 0.01;
	if (m_attributes.m_shClockMultiplier < -4.f)m_attributes.m_shClockMultiplier = -4.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) m_attributes.m_shClockMultiplier += 0.01;
	if (m_attributes.m_shClockMultiplier > 4.f)m_attributes.m_shClockMultiplier = 4.f;

	m_attributes.m_shClockAccum += 0.01666 * m_attributes.m_shClockMultiplier;
	
	if (m_attributes.m_scale > 0.0001)
	{
		if (m_attributes.m_precision > m_attributes.m_precision2)
			m_attributes.m_precision--;
	}
	else
	{
		if (m_attributes.m_precision < m_attributes.m_precision2*3)
			m_attributes.m_precision++;
	}

	m_attributes.m_shader.setUniform("sh_scale", (float)m_attributes.m_scale);
	m_attributes.m_shader.setUniform("sh_time", m_attributes.m_shClockAccum);
	m_attributes.m_shader.setUniform("sh_music", intensity);
	m_attributes.m_shader.setUniform("sh_musicAccumulator", accum);
	m_attributes.m_shader.setUniform("sh_precision", (float)m_attributes.m_precision);
	m_attributes.m_shader.setUniform("sh_relativeCenter",
		sf::Glsl::Vec2(m_attributes.m_relativeCenter)
	);
	m_attributes.m_shader.setUniform("sh_resolutionCenter",
		sf::Glsl::Vec2((float)m_attributes.m_resolution.x / 2., (float)m_attributes.m_resolution.y / 2.)
	);
	m_attributes.m_shader.setUniform("sh_displacement",
		sf::Glsl::Vec2(m_attributes.m_displacement)
	);
	m_attributes.m_shader.setUniform("sh_windowResolution", sf::Glsl::Vec2(m_attributes.m_resolution));
	m_attributes.m_renderTexture.clear();
	m_attributes.m_renderTexture.draw(m_attributes.m_sprite, &m_attributes.m_shader);
	m_attributes.m_renderTexture.display();
	m_attributes.m_sprite.setTexture(m_attributes.m_renderTexture.getTexture());
}

SpriteInfo& m::Mandelbrot::GetSprite()
{
	return m_spriteInfo;
}

FractalInfo& m::Mandelbrot::GetParams()
{
	return m_fractalInfo;
}

void m::Mandelbrot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_attributes.m_sprite, states);
}

//
// Other helper classes and structures.
//

// FractalAtb

void m::FractalAtb::resetToDefault()
{
	m_resolution = { default_screen_width, default_screen_height };
	m_size = { default_screen_width, default_screen_height };

	m_precision = default_precision;
	m_precision2 = default_precision/3;
	m_scale = default_scale;

	m_relativeCenter = default_center;
	m_displacement = sf::Vector2m(0,0);

	m_shClockMultiplier = 1;
	m_shClockAccum = 0;

	m_shaderPath = default_shader_path;
	m_shader.loadFromFile(default_shader_path,sf::Shader::Fragment);
}

float m::FractalAtb::sizeProportionX()
{
	return float(m_size.x) / float(m_resolution.x);
}

float m::FractalAtb::sizeProportionY()
{
	return float(m_size.y) / float(m_resolution.y);
}

// FractalInfo

bool m::FractalInfo::checkIfBelongs(sf::Vector2m point)
{

	sf::Vector2m translatedPoint{
		point.x / m_ptr->sizeProportionX(),
		point.y / m_ptr->sizeProportionY()
	};

	// Complex numbers 'z' and 'c'
	sf::Vector2m z{ 0 , 0 };

	sf::Vector2m c{
			m_ptr->m_scale * translatedPoint.x - m_ptr->m_relativeCenter.x,
			m_ptr->m_scale * translatedPoint.y - m_ptr->m_relativeCenter.y
	};

	size_t iteration = 0;
	mType tempRe;

	while (z.y * z.y + z.x * z.x < 4.0 && ++iteration < m_ptr->m_precision)
	{
		tempRe = (z.x * z.x - z.y * z.y) + c.x;
		z.y = (2.0 * z.x * z.y) + c.y;
		z.x = tempRe;
	}

	return iteration == m_ptr->m_precision;
}

// Resolution and size.

sf::Vector2u m::FractalInfo::resolution()
{
	return m_ptr->m_resolution;
}

sf::Vector2u m::FractalInfo::resolution(sf::Vector2u resolution)
{
	m_ptr->m_resolution = resolution;
	return m_ptr->m_resolution;
}

sf::Vector2u m::FractalInfo::resolution(unsigned int x, unsigned int y)
{
	m_ptr->m_resolution = { x,y };
	return m_ptr->m_resolution;
}

sf::Vector2u m::FractalInfo::size()
{
	return m_ptr->m_size;
}

sf::Vector2u m::FractalInfo::size(sf::Vector2u size)
{
	m_ptr->m_size = size;
	return m_ptr->m_size;
}

sf::Vector2u m::FractalInfo::size(unsigned int x, unsigned int y)
{
	m_ptr->m_size = { x,y };
	return m_ptr->m_size;
}

// displacement

sf::Vector2m m::FractalInfo::displacement()
{
	return m_ptr->m_displacement;
}

sf::Vector2m m::FractalInfo::displacement(sf::Vector2m position, bool scale)
{
	if (scale)
	{
		m_ptr->m_displacement = position * m_ptr->m_scale;
	}
	else
	{
		m_ptr->m_displacement = position;
	}

	return m_ptr->m_displacement;
}

sf::Vector2m m::FractalInfo::displacement(mType x, mType y, bool scale)
{
	if (scale)
	{
		m_ptr->m_displacement = sf::Vector2m(x,y) * m_ptr->m_scale;
	}
	else
	{
		m_ptr->m_displacement = { x, y };
	}
	
	return m_ptr->m_displacement;
}

sf::Vector2m m::FractalInfo::displacementAdd(sf::Vector2m offset, bool scale)
{
	if (scale)
	{
		offset *= m_ptr->m_scale;
	}

	offset.x *= mType(m_ptr->m_resolution.x) / mType(m_ptr->m_size.x);
	offset.y *= mType(m_ptr->m_resolution.y) / mType(m_ptr->m_size.y);

	m_ptr->m_displacement += sf::Vector2m(offset);

	return m_ptr->m_displacement;
}

sf::Vector2m m::FractalInfo::displacementAdd(mType x, mType y, bool scale)
{
	if (scale)
	{
		x *= m_ptr->m_scale;
		y *= m_ptr->m_scale;
	}

	x *= static_cast<mType>(m_ptr->m_resolution.x) / static_cast<mType>(m_ptr->m_size.x);
	y *= static_cast<mType>(m_ptr->m_resolution.y) / static_cast<mType>(m_ptr->m_size.y);

	m_ptr->m_displacement += sf::Vector2m(x, y);
	return m_ptr->m_displacement;
}

// precision

sf::Uint16 m::FractalInfo::precision()
{
	return m_ptr->m_precision;
}

sf::Uint16 m::FractalInfo::precision(sf::Uint16 precision)
{
	m_ptr->m_precision = precision;
	m_ptr->m_precision2 = precision;
	return m_ptr->m_precision;
}

sf::Uint16 m::FractalInfo::precisionAdd(sf::Uint16 value)
{
	m_ptr->m_precision += value;
	if (m_ptr->m_precision < 1) m_ptr->m_precision = 1;
	return m_ptr->m_precision;
}

// scale

mType m::FractalInfo::scale()
{
	return m_ptr->m_scale;
}

mType m::FractalInfo::scale(mType scale)
{
	m_ptr->m_scale = scale;
	return m_ptr->m_scale;
}

mType m::FractalInfo::scaleMultiply(mType value)
{
	m_ptr->m_scale *= value;
	return m_ptr->m_scale;
}

// New center of Mandelbrot coordinate system.

sf::Vector2m m::FractalInfo::center()
{
	return m_ptr->m_relativeCenter;
}

sf::Vector2m m::FractalInfo::center(sf::Vector2m center)
{
	m_ptr->m_relativeCenter = center;
	return m_ptr->m_relativeCenter;
}

sf::Vector2m m::FractalInfo::moveCenter(sf::Vector2m offset)
{
	m_ptr->m_relativeCenter += offset;
	return m_ptr->m_relativeCenter;
}

std::string m::FractalInfo::path()
{
	return m_ptr->m_shaderPath;
}

std::string m::FractalInfo::path(const char* path)
{
	m_ptr->m_shaderPath = path;
	m_ptr->m_shader.loadFromFile(path, sf::Shader::Fragment);
	return m_ptr->m_shaderPath;
}

// SpriteInfo

// Sprite position.

sf::Vector2f m::SpriteInfo::position()
{
	return m_ptr->m_sprite.getPosition();
}

sf::Vector2f m::SpriteInfo::position(sf::Vector2f position)
{
	m_ptr->m_sprite.setPosition(position);
	return m_ptr->m_sprite.getPosition();
}

sf::Vector2f m::SpriteInfo::position(float x, float y)
{
	m_ptr->m_sprite.setPosition(x,y);
	return m_ptr->m_sprite.getPosition();
}

sf::Vector2f m::SpriteInfo::move(sf::Vector2f offset)
{
	m_ptr->m_sprite.move(offset);
	return m_ptr->m_sprite.getPosition();
}

sf::Vector2f m::SpriteInfo::move(float x, float y)
{
	m_ptr->m_sprite.move(x,y);
	return m_ptr->m_sprite.getPosition();
}

// Sprite scale.

sf::Vector2f m::SpriteInfo::scale()
{
	return m_ptr->m_sprite.getScale();
}

sf::Vector2f m::SpriteInfo::scale(sf::Vector2f scale)
{
	m_ptr->m_sprite.setScale(scale);
	return m_ptr->m_sprite.getScale();
}

sf::Vector2f m::SpriteInfo::scale(float x, float y)
{
	m_ptr->m_sprite.setScale(x,y);
	return m_ptr->m_sprite.getScale();
}

sf::Vector2f m::SpriteInfo::scaleMultiply(sf::Vector2f factor)
{
	m_ptr->m_sprite.scale(factor);
	return m_ptr->m_sprite.getScale();
}

sf::Vector2f m::SpriteInfo::scaleMultiply(float x, float y)
{
	m_ptr->m_sprite.scale(x, y);
	return m_ptr->m_sprite.getScale();
}