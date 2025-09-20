#ifndef GAME_H
# define GAME_H

# include <SFML/Graphics.hpp>
# include <string>

# include "EntityManager.h"
# include "Entity.h"
# include "GameConfig.h"

class Game
{
	public:
		Game(const std::string& configPath);

		void	run();

	private:
		void					init(const std::string& configPath);
		void					initText(sf::Text& text, sf::Font& font, const Font& fontConfig, const std::string& str);

		void					spawnPlayer();
		void					spawnEnemy();
		void					spawnSmallEnemies(std::shared_ptr<Entity> entity);
		void					spawnBullet(const Vec2f& startPos, const Vec2f& targetPos);
		void					specialWeapon(const Vec2f& startPos);

		void					inputSystem();
		void					enemySpawnerSystem();
		void					movementSystem();
		void					collisionSystem();
		void					lifespanSystem();
		void					GUISystem();
		void					renderSystem();

		sf::RenderWindow		window_;
		GameConfig				gameConfig_;
		ImGuiConfig				imGuiConfig_;
		sf::Clock				deltaClock_;
		EntityManager			entities_;

		sf::Font				scoreFont_;
		sf::Text				scoreText_;

		sf::Font				highScoreFont_;
		sf::Text				highScoreText_;

		sf::Font				specialWeaponFont_;
		sf::Text				specialWeaponText_;

		sf::Font				pauseFont_;
		sf::Text				pauseText_;

		size_t					score_ = 0;
		size_t					highScore_ = 0;
		std::string				specialWeaponAvailable_ = "Special Weapon Ready!";
		std::string				specialWeaponCooldown_ = "Cooldown in Progress...";
		std::string				pause_ = "PAUSED";

		int						currentFrame_ = 0;
		int						lastEnemySpawnTime_ = 0;
		int						lastSpecialWeaponTime_ = 0;
		bool					isSpecialWeaponAvailable_ = true;
		bool					paused_ = false;
		bool					running_ = true;

		std::shared_ptr<Entity>	player();
};

#endif