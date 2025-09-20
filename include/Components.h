#ifndef COMPONENTS_H
# define COMPONENTS_H

# include <SFML/Graphics/CircleShape.hpp>

# include "Vec2.h"

struct Component
{
	bool	exists_ = false;
};

struct TransformComponent : public Component
{
	Vec2f	pos_;
	Vec2f	velocity_;
	float	angle_;

	TransformComponent() = default;
	TransformComponent(const Vec2f& pos, const Vec2f& velocity, const float angle) :
		pos_{pos}, velocity_{velocity}, angle_{angle} {}
};

struct ShapeComponent : public Component
{
	sf::CircleShape	circle_;

	ShapeComponent() = default;
	ShapeComponent(const float radius, const size_t pointCount, const sf::Color& fillColor,
					const sf::Color& outlineColor, const float thickness) :
		circle_{radius, pointCount}
	{
		circle_.setFillColor(fillColor);
		circle_.setOutlineColor(outlineColor);
		circle_.setOutlineThickness(thickness);
		circle_.setOrigin(radius, radius);
	}
};

struct CollisionComponent : public Component
{
	float	radius_ = 0.0f;

	CollisionComponent() = default;
	CollisionComponent(const float radius) :
		radius_{radius} {}
};

struct ScoreComponent : public Component
{
	size_t	score_ = 0;

	ScoreComponent() = default;
	ScoreComponent(const size_t score) :
		score_{score} {}
};

struct LifespanComponent : public Component
{
	using FrameCount = int;

	FrameCount	lifespan_ = 0;
	FrameCount	remaining_ = 0;

	LifespanComponent() = default;
	LifespanComponent(const int lifespan) :
		lifespan_{lifespan}, remaining_{lifespan} {}
};

struct InputComponent : public Component
{
	bool	up_ = false;
	bool	down_ = false;
	bool	left_ = false;
	bool	right_ = false;
	bool	shoot_ = false;

	InputComponent() = default;
};

#endif