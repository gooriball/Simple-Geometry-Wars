#ifndef RANDOM_GENERATOR_H
# define RANDOM_GENERATOR_H

# include <random>

# include "Vec2.h"
# include "GameConfig.h"

class RandomGenerator
{
	public:
		static Vec2f	getRandomEnemyPos(const GameConfig& gameConfig, const Vec2f playerPos)
		{
			auto& gen = getGenerator();

			float enemyRadius = static_cast<float>(gameConfig.enemyConfig_.shapeRadius_);

			std::uniform_real_distribution<float> posX(enemyRadius + 1.0f, gameConfig.windowConfig_.width_ - enemyRadius - 1.0f);
			std::uniform_real_distribution<float> posY(enemyRadius + 1.0f, gameConfig.windowConfig_.height_ - enemyRadius - 1.0f);

			float	playerRadius = static_cast<float>(gameConfig.playerConfig_.shapeRadius_);
			float	x;
			float	y;
			while (true)
			{
				x = posX(gen);
				y = posY(gen);

				if (std::abs(playerPos.x_ - x) > playerRadius * 2.0f + enemyRadius ||
					std::abs(playerPos.y_ - y) > playerRadius * 2.0f + enemyRadius)
				{
					break ;
				}
			}

			return (Vec2f{x, y});
		}

		static Vec2f	getRandomEnemySpeed(const GameConfig& gameConfig)
		{
			auto& gen = getGenerator();

			float	speedMin = gameConfig.enemyConfig_.speedRange_.min_;
			float	speedMax = gameConfig.enemyConfig_.speedRange_.max_;

			std::uniform_real_distribution<float> speed(speedMin, speedMax);

			return (Vec2f{speed(gen), speed(gen)});
		}

		static sf::Color	getRandomEnemyColor()
		{
			auto& gen = getGenerator();

			std::uniform_int_distribution<int> color(0, 255);

			return (sf::Color{static_cast<sf::Uint8>(color(gen)),
								static_cast<sf::Uint8>(color(gen)),
								static_cast<sf::Uint8>(color(gen))});
		}

		static size_t	getRandomEnemyPointCount(const GameConfig& gameConfig)
		{
			auto& gen = getGenerator();

			size_t pointCountMin = gameConfig.enemyConfig_.verticeRange_.min_;
			size_t pointCountMax = gameConfig.enemyConfig_.verticeRange_.max_;

			std::uniform_int_distribution<size_t> pointCount(pointCountMin, pointCountMax);

			return (pointCount(gen));
		}

	private:
		static std::mt19937&	getGenerator()
		{
			static std::random_device rd;
			static std::mt19937 gen(rd());

			return (gen);
		}
};

#endif