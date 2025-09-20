#ifndef GAME_CONFIG_H
# define GAME_CONFIG_H

# include <SFML/Graphics.hpp>
# include <cstdint>

# include "Vec2.h"

struct Color
{
	uint8_t	r_ = 255;
	uint8_t g_ = 255;
	uint8_t b_ = 255;
	uint8_t	a_ = 255;

	operator sf::Color() const
	{
		return (sf::Color{r_, g_, b_, a_});
	}
};

template<typename T>
struct Range
{
	T	min_;
	T	max_;
};

struct Font
{
	Vec2f		pos_ = { 0.5f, 0.5f };
	std::string	path_ = "fonts/tech.ttf";
	int			size_ = 32;
	Color		color_ = {255, 255, 255, 255};
};

struct PlayerConfig
{
	Vec2f	pos_ = { 0.5f, 0.5f };
	float	shapeRadius_ = 25.0f;
	float	collisionRadius_ = 25.0f;
	Color	fillColor_ = { 5, 5, 5, 255 };
	Color	outlineColor_ = { 255, 0, 0, 255 };
	float	outlineThickness_ = 4.0f;
	size_t	vertices_ = 8;
	Vec2f	velocity_ = { 5.0f, 5.0f };
	size_t	score_ = 0;
	size_t	highScore_ = 0;
};

struct EnemyConfig
{
	float			shapeRadius_ = 25.0f;
	float			collisionRadius_ = 25.0f;
	Color			outlineColor_ = { 255, 255, 255, 255 };
	float			outlineThickness_ = 4.0f;
	Range<size_t>	verticeRange_ = { 3, 8 };
	Range<float>	speedRange_ = { 1.0f, 5.0f };
	int				smallEnemyLifespan_ = 90;
	int				spawnInterval_ = 60;
};

struct BulletConfig
{
	float	shapeRadius_ = 10.0f;
	float	collisionRadius_ = 10.0f;
	Color	fillColor_ = { 255, 255, 255, 255 };
	Color	outlineColor_ = { 255, 255, 255, 255 };
	float	outlineThickness_ = 2.0f;
	size_t	vertices_ = 20;
	float	speed_ = 10.0f;
	int		lifespan_ = 75;
};

struct WindowConfig
{
	int			width_ = 1440;
	int 		height_ = 900;
	bool		fullscreen_ = false;
	std::string	title_ = "Geometry Wars";
	int			frameLimit_ = 60;
};

struct UIConfig
{
	Font	score_ = {
		{ 0.01f, 0.07f },
		"fonts/tech.ttf",
		32,
		{ 255, 255, 255, 255 }
	};
	Font	highScore_ = {
		{ 0.01f, 0.02f },
		"fonts/tech.ttf",
		32,
		{ 255, 255, 255, 255 }
	};
	Font	specialWeapon_ = {
		{ 0.83f, 0.03f },
		"fonts/tech.ttf",
		48,
		{ 255, 255, 255, 255 }
	};
	Font	pause_ = {
		{ 0.5f, 0.5f },
		"fonts/tech.ttf",
		96,
		{ 255, 255, 255, 255 }
	};
};

struct ImGuiConfig
{
	bool	movement_ = true;
	bool	lifespan_ = true;
	bool	collision_ = true;
	bool	spawning_ = true;
	bool	rendering_ = true;
};

struct GameConfig
{
	PlayerConfig	playerConfig_;
	EnemyConfig		enemyConfig_;
	BulletConfig	bulletConfig_;
	WindowConfig	windowConfig_;
	UIConfig		uiConfig_;
};

#endif