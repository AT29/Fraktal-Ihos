#pragma once

#include <SFML/Graphics.hpp>
#include <string>

typedef double mType;

namespace sf
{
	typedef Vector2<mType> Vector2m;
}

namespace m
{
	// Global constants.

	const mType			default_scale			{ 0.004 };
	const unsigned int  default_precision		{ 150 };
	const sf::Vector2m	default_center			{ 0,0 };

	const unsigned int  default_screen_width	{ 800 };
	const unsigned int  default_screen_height	{ 600 };

	const std::string	default_shader_path		{ "./resources/MandelbrotPlay.frag" };

	// declarations

	class Mandelbrot;

	// Fractal Attributes /////////////////////////////////////////////////////////////

	class FractalAtb
	{
	public:

		void				resetToDefault	();
		float				sizeProportionX	();
		float				sizeProportionY	();

		sf::Vector2u		m_resolution;
		sf::Vector2u		m_size;
		sf::Uint16			m_precision;
		sf::Uint16			m_precision2;
		sf::Vector2m		m_relativeCenter;
		sf::Vector2m		m_displacement;
		mType				m_scale;

		sf::Sprite			m_sprite;
		sf::RenderTexture	m_renderTexture;
		sf::Texture			m_texture;
		sf::Shader			m_shader;
		std::string			m_shaderPath;

		float				m_musicIntensity;
		float				m_shClockMultiplier;
		float				m_shClockAccum;
	};

	// FractalInfo ////////////////////////////////////////////////////////////////////

	class FractalInfo
	{
	public:

		/// <summary>
		/// Returns true if the point (in window coordinates) belongs to the Mandelbrot set.
		/// The point must be presented in window coordinates.
		/// </summary>
		bool			checkIfBelongs	(sf::Vector2m point);

		sf::Vector2u	resolution		();
		sf::Vector2u	resolution		(sf::Vector2u resolution);
		sf::Vector2u	resolution		(unsigned int x, unsigned int y);

		sf::Vector2u	size			();
		sf::Vector2u	size			(sf::Vector2u size);
		sf::Vector2u	size			(unsigned int x, unsigned int y);

		sf::Vector2m	displacement	();
		sf::Vector2m	displacement	(sf::Vector2m position, bool scale = true);
		sf::Vector2m	displacement	(mType x, mType y, bool scale = true);

		sf::Vector2m	displacementAdd	(sf::Vector2m offset, bool scale = true);
		sf::Vector2m	displacementAdd	(mType x, mType y, bool scale = true);

		sf::Uint16		precision		();
		sf::Uint16		precision		(sf::Uint16 precision);
		sf::Uint16		precisionAdd	(sf::Uint16 value);

		mType			scale			();
		mType			scale			(mType scale);
		mType			scaleMultiply	(mType value);

		sf::Vector2m	center			();
		sf::Vector2m	center			(sf::Vector2m center);
		sf::Vector2m	moveCenter		(sf::Vector2m offset);

		std::string		path			();
		std::string		path			(const char* path);

	private:

		friend class Mandelbrot;

		FractalAtb*		m_ptr;

	};

	// SpriteInfo /////////////////////////////////////////////////////////////////////

	class SpriteInfo
	{
	public:

		sf::Vector2f	position		();
		sf::Vector2f	position		(sf::Vector2f position);
		sf::Vector2f	position		(float x, float y);

		sf::Vector2f	move			(sf::Vector2f offset);
		sf::Vector2f	move			(float x, float y);

		sf::Vector2f	scale			();
		sf::Vector2f	scale			(sf::Vector2f scale);
		sf::Vector2f	scale			(float x, float y);
		sf::Vector2f	scaleMultiply	(sf::Vector2f factor);
		sf::Vector2f	scaleMultiply	(float x, float y);

	private:

		friend class m::Mandelbrot;

		FractalAtb*		m_ptr;

	};

	// Mandelbrot /////////////////////////////////////////////////////////////////////

	class Mandelbrot :public sf::Drawable
	{
	public:

		/*Constructor*/	Mandelbrot		();
		/*Constructor*/	Mandelbrot		(sf::Vector2u size, sf::Vector2u resolution);

		void			create			(sf::Vector2u size, sf::Vector2u resolution, bool reset=true);

		// Common methods

		void			update			(float intensity, float accum);

		SpriteInfo&		GetSprite		();
		FractalInfo&	GetParams		();

	private:

		void			init			(sf::Vector2u& size, sf::Vector2u& resolution, bool reset);

		virtual void	draw			(sf::RenderTarget& target, sf::RenderStates states) const;

	private:

		sf::Clock		m_clock;
		sf::Clock		m_shaderClock;

		FractalAtb		m_attributes;
		SpriteInfo		m_spriteInfo;
		FractalInfo		m_fractalInfo;

		bool			m_firstRun;

	};
}