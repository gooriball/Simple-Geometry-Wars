#include "Game.h"
#include "ConfigLoader.h"
#include "RandomGenerator.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <cmath>

Game::Game(const std::string& configPath)
{
	init(configPath);
}

void	Game::init(const std::string& configPath)
{
	gameConfig_ = ConfigLoader::loadFromFile(configPath);

	unsigned int windowWidth = gameConfig_.windowConfig_.width_;
	unsigned int windowHeight = gameConfig_.windowConfig_.height_;
	sf::String title = gameConfig_.windowConfig_.title_;
	bool fullscreen = gameConfig_.windowConfig_.fullscreen_;
	window_.create(sf::VideoMode{windowWidth, windowHeight}, title, fullscreen ? sf::Style::Fullscreen : sf::Style::Default);

	unsigned int frameLimit = gameConfig_.windowConfig_.frameLimit_;
	window_.setFramerateLimit(frameLimit);

	initText(scoreText_, scoreFont_, gameConfig_.uiConfig_.score_, "");
	initText(highScoreText_, highScoreFont_, gameConfig_.uiConfig_.highScore_, "");
	initText(specialWeaponText_, specialWeaponFont_, gameConfig_.uiConfig_.specialWeapon_, specialWeaponAvailable_);
	initText(pauseText_, pauseFont_, gameConfig_.uiConfig_.pause_, pause_);

	ImGui::SFML::Init(window_);

	spawnPlayer();
}

void	Game::initText(sf::Text& text, sf::Font& font, const Font& fontConfig, const std::string& str)
{
	if (!font.loadFromFile(fontConfig.path_))
	{
		SPDLOG_ERROR("Could not load font: {}", fontConfig.path_);
		exit(1);
	}
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontConfig.size_);
	text.setFillColor(fontConfig.color_);
	text.setPosition(sf::Vector2f{gameConfig_.windowConfig_.width_ * fontConfig.pos_.x_ - text.getGlobalBounds().width / 2.0f,
										gameConfig_.windowConfig_.height_ * fontConfig.pos_.y_ - text.getCharacterSize() / 2.0f});
}

void	Game::run()
{
	while (running_)
	{
		entities_.update();
		ImGui::SFML::Update(window_, deltaClock_.restart());

		inputSystem();
		enemySpawnerSystem();
		movementSystem();
		collisionSystem();
		lifespanSystem();
		GUISystem();
		renderSystem();

		if (paused_) { continue ; }
		if (++currentFrame_ - lastSpecialWeaponTime_ > 900) { isSpecialWeaponAvailable_ = true; }
	}

	window_.close();
}

void	Game::spawnPlayer()
{
	auto& player = entities_.addEntity("player");

	const auto& playerConfig = gameConfig_.playerConfig_;
	const auto& windowConfig = gameConfig_.windowConfig_;
	Vec2f playerPos{windowConfig.width_ * playerConfig.pos_.x_, windowConfig.height_ * playerConfig.pos_.y_};
	player->addComponent<TransformComponent>(playerPos, playerConfig.velocity_, 0.0f);
	player->addComponent<ShapeComponent>(playerConfig.shapeRadius_, playerConfig.vertices_, playerConfig.fillColor_,
											playerConfig.outlineColor_, playerConfig.outlineThickness_);
	player->addComponent<CollisionComponent>(playerConfig.collisionRadius_);
	player->addComponent<InputComponent>();
}

void	Game::spawnEnemy()
{
	auto& enemy = entities_.addEntity("enemy");

	const auto&	enemyConfig = gameConfig_.enemyConfig_;
	Vec2f		enemyPos = RandomGenerator::getRandomEnemyPos(gameConfig_, player()->getComponent<TransformComponent>().pos_);
	Vec2f		enemySpeed = RandomGenerator::getRandomEnemySpeed(gameConfig_);
	sf::Color	enemyColor = RandomGenerator::getRandomEnemyColor();
	size_t		enemyPointCount = RandomGenerator::getRandomEnemyPointCount(gameConfig_);
	
	enemy->addComponent<TransformComponent>(enemyPos, enemySpeed, 0.0f);
	enemy->addComponent<ShapeComponent>(enemyConfig.shapeRadius_, enemyPointCount, enemyColor, enemyConfig.outlineColor_, enemyConfig.outlineThickness_);
	enemy->addComponent<CollisionComponent>(enemyConfig.collisionRadius_);

	lastEnemySpawnTime_ = currentFrame_;
}

void	Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	const size_t vertices = entity->getComponent<ShapeComponent>().circle_.getPointCount();
	const auto pos = entity->getComponent<TransformComponent>().pos_;
	const float speed = std::max(entity->getComponent<TransformComponent>().velocity_.x_, entity->getComponent<TransformComponent>().velocity_.y_);
	const float radius = entity->getComponent<ShapeComponent>().circle_.getRadius() / 2.0f;
	const auto fillColor = entity->getComponent<ShapeComponent>().circle_.getFillColor();
	const auto outlineColor = entity->getComponent<ShapeComponent>().circle_.getOutlineColor();
	const auto thickness = entity->getComponent<ShapeComponent>().circle_.getOutlineThickness();
	const float pi = 3.1415f;
	const float degrees = 360.0f / vertices;
	const float radians = degrees * pi / 180.0f;
	for (size_t i = 0; i < vertices; ++i)
	{
		auto& smallEnemy = entities_.addEntity("smallEnemy");
		Vec2f direction{std::cosf(radians * i), std::sinf(radians * i)};
		smallEnemy->addComponent<TransformComponent>(pos, direction.normalize() * speed, 0.0f);
		smallEnemy->addComponent<ShapeComponent>(radius, vertices, fillColor,outlineColor, thickness);
		smallEnemy->addComponent<CollisionComponent>(radius);
		smallEnemy->addComponent<LifespanComponent>(gameConfig_.enemyConfig_.smallEnemyLifespan_);
	}
}

void	Game::spawnBullet(const Vec2f& startPos, const Vec2f& targetPos)
{
	if (paused_) { return ; }
	
	const auto& bulletConfig = gameConfig_.bulletConfig_;
	auto& bullet = entities_.addEntity("bullet");
	bullet->addComponent<TransformComponent>(startPos, (targetPos - startPos).normalize() * bulletConfig.speed_, 0.0f);
	bullet->addComponent<ShapeComponent>(bulletConfig.shapeRadius_, bulletConfig.vertices_, bulletConfig.fillColor_, bulletConfig.outlineColor_, bulletConfig.outlineThickness_);
	bullet->addComponent<CollisionComponent>(bulletConfig.collisionRadius_);
	bullet->addComponent<LifespanComponent>(bulletConfig.lifespan_);
}

void	Game::specialWeapon(const Vec2f& playerPos)
{
	if (paused_ || !isSpecialWeaponAvailable_ ) { return ; }

    const float pi = 3.1415f;
    const float degrees = 360.0f / 36;
    const float radians = degrees * pi / 180.0f;
    
    for (int i = 0; i < 36; ++i)
    {
        Vec2f direction{std::cosf(radians * i), std::sinf(radians * i)};
        
        for (int j = 0; j < 5; ++j)
        {
            float distance = 20.0f * (j + 1);
            Vec2f startPos = playerPos + direction * distance;
			Vec2f targetPos = playerPos + direction * 1000.0f;
			spawnBullet(startPos, targetPos);
        }
    }
	lastSpecialWeaponTime_ = currentFrame_;
	isSpecialWeaponAvailable_ = false;
}

void	Game::inputSystem()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(window_, event);
		auto& playerInput = player()->getComponent<InputComponent>();
		if (event.type == sf::Event::Closed) { running_ = false; }
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::W:
					playerInput.up_ = true;
					break ;
				case sf::Keyboard::S:
					playerInput.down_ = true;
					break ;
				case sf::Keyboard::A:
					playerInput.left_ = true;
					break ;
				case sf::Keyboard::D:
					playerInput.right_ = true;
					break ;
				case sf::Keyboard::P:
					paused_ = !paused_;
					break ;
				case sf::Keyboard::Escape:
					running_ = false;
					break ;
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::W:
					playerInput.up_ = false;
					break ;
				case sf::Keyboard::S:
					playerInput.down_ = false;
					break ;
				case sf::Keyboard::A:
					playerInput.left_ = false;
					break ;
				case sf::Keyboard::D:
					playerInput.right_ = false;
					break ;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (ImGui::GetIO().WantCaptureMouse) { continue ; }

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawnBullet(player()->getComponent<TransformComponent>().pos_, Vec2i{event.mouseButton.x, event.mouseButton.y});
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				specialWeapon(player()->getComponent<TransformComponent>().pos_);
			}
		}
	}
}

void	Game::enemySpawnerSystem()
{
	if (paused_ || !imGuiConfig_.spawning_) { return ; }

	if (gameConfig_.enemyConfig_.spawnInterval_ <= currentFrame_ - lastEnemySpawnTime_)
	{
		spawnEnemy();
	}
}

void	Game::movementSystem()
{
	auto entities = entities_.getEntities();
	for (auto& entity : entities) { entity->getComponent<TransformComponent>().angle_ += 1.0f; }

	if (paused_ || !imGuiConfig_.movement_) { return ; }

	auto& playerEntity = player();
	auto& playerVelocity = playerEntity->getComponent<TransformComponent>().velocity_;
	Vec2f movement{0.0f, 0.0f};
	for (auto& entity : entities)
	{
		if (entity->tag() == "player")
		{
			if (playerEntity->getComponent<InputComponent>().up_) { movement.y_ -= playerVelocity.y_; }
			if (playerEntity->getComponent<InputComponent>().down_) { movement.y_ += playerVelocity.y_; }
			if (playerEntity->getComponent<InputComponent>().left_) { movement.x_ -= playerVelocity.x_; }
			if (playerEntity->getComponent<InputComponent>().right_) { movement.x_ += playerVelocity.x_; }
			playerEntity->getComponent<TransformComponent>().pos_ += movement;
		}
		else if (entity->tag() == "enemy" || entity->tag() == "smallEnemy" || entity->tag() == "bullet")
		{
			entity->getComponent<TransformComponent>().pos_ += entity->getComponent<TransformComponent>().velocity_;
		}
	}
}

void	Game::collisionSystem()
{
	if (paused_) { return ; }

	const auto& windowConfig = gameConfig_.windowConfig_;

	auto& playerEntity = player();

	auto& entities = entities_.getEntities();
	for (auto& entity : entities)
	{
		auto& pos = entity->getComponent<TransformComponent>().pos_;
		auto& velocity = entity->getComponent<TransformComponent>().velocity_;
		const auto collisionRadius = entity->getComponent<CollisionComponent>().radius_;
		const auto& tag = entity->tag();
		if (tag == "player")
		{
			if (pos.x_ - collisionRadius < 0.0f) { pos.x_ = collisionRadius; }
			if (pos.x_ + collisionRadius > windowConfig.width_) { pos.x_ = windowConfig.width_ - collisionRadius; }
			if (pos.y_ - collisionRadius < 0.0f) { pos.y_ = collisionRadius; }
			if (pos.y_ + collisionRadius > windowConfig.height_) { pos.y_ = windowConfig.height_ - collisionRadius; }
		}
		else if (tag == "enemy" || tag == "smallEnemy")
		{
			if (pos.x_ - collisionRadius < 0.0f || pos.x_ + collisionRadius > windowConfig.width_) { velocity.x_ *= -1; }
			if (pos.y_ - collisionRadius < 0.0f || pos.y_ + collisionRadius > windowConfig.height_) { velocity.y_ *= -1; }
			if (!imGuiConfig_.collision_) { continue ; }
			if (playerEntity->getComponent<TransformComponent>().pos_.distSquared(pos) <
				std::pow(playerEntity->getComponent<CollisionComponent>().radius_ + collisionRadius, 2))
			{
				const auto& playerConfig = gameConfig_.playerConfig_;
				const auto& windowConfig = gameConfig_.windowConfig_;
				Vec2f playerPos{windowConfig.width_ * playerConfig.pos_.x_, windowConfig.height_ * playerConfig.pos_.y_};
				playerEntity->getComponent<TransformComponent>().pos_ = playerPos;
				score_ = 0;
				isSpecialWeaponAvailable_ = true;

				entity->destroy();
				if (tag == "enemy") { spawnSmallEnemies(entity); }
			}
		}
	}
	if (!imGuiConfig_.collision_) { return ; }
	auto& bullets = entities_.getEntities("bullet");
	for (auto& bullet : bullets)
	{
		for (auto& entity : entities)
		{
			auto& pos = entity->getComponent<TransformComponent>().pos_;
			auto& velocity = entity->getComponent<TransformComponent>().velocity_;
			const auto collisionRadius = entity->getComponent<CollisionComponent>().radius_;
			const auto& tag = entity->tag();
			if (tag == "enemy" || tag == "smallEnemy")
			{
				if (bullet->getComponent<TransformComponent>().pos_.distSquared(entity->getComponent<TransformComponent>().pos_) <
					std::pow(bullet->getComponent<CollisionComponent>().radius_ + entity->getComponent<CollisionComponent>().radius_, 2))
				{
					bullet->destroy();
					entity->destroy();
					const auto vertices = entity->getComponent<ShapeComponent>().circle_.getPointCount();
					if (tag == "enemy") { spawnSmallEnemies(entity); score_ += (vertices * 10); }
					else { score_ += (static_cast<int>(vertices) * 20); }
					score_ > highScore_ ? (highScore_ = score_) : void();
				}
			}	
		}
	}
}

void	Game::lifespanSystem()
{
	if (paused_ || !imGuiConfig_.lifespan_) { return ; }

	auto& entities = entities_.getEntities();
	for (auto& entity : entities)
	{
		if (entity->hasComponent<LifespanComponent>())
		{
			auto& entityLifespan = entity->getComponent<LifespanComponent>();
			auto& entityShape = entity->getComponent<ShapeComponent>();

			if (--entityLifespan.remaining_ < 1) { entity->destroy(); continue ; }
			auto& entityColor = entityShape.circle_.getFillColor();
			auto alpha = static_cast<sf::Uint8>((entityLifespan.remaining_ / static_cast<float>(entityLifespan.lifespan_)) * 255.0f);
			auto newColor = entityColor;
			newColor.a = alpha;
			entityShape.circle_.setFillColor(newColor);
			entityShape.circle_.setOutlineColor(newColor);
		}
	}
}

void	Game::GUISystem()
{
	ImGui::Begin("Geometry Wars");
	if (ImGui::BeginTabBar("MyTabBar"))
	{
		if (ImGui::BeginTabItem("Systems"))
		{
			ImGui::Checkbox("Movement", &imGuiConfig_.movement_);
			ImGui::Checkbox("Lifespan", &imGuiConfig_.lifespan_);
			ImGui::Checkbox("Collision", &imGuiConfig_.collision_);
			ImGui::Checkbox("Spawning", &imGuiConfig_.spawning_);
			if (imGuiConfig_.spawning_)
			{
				ImGui::Indent(30);
				ImGui::SliderInt("Spawn", &gameConfig_.enemyConfig_.spawnInterval_, 0, 180);
				if (ImGui::Button("Manual Spawn"))
				{
					spawnEnemy();
				}
				ImGui::Unindent(30);
			}
			ImGui::Checkbox("Rendering", &imGuiConfig_.rendering_);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void	Game::renderSystem()
{
	window_.clear();

	if (imGuiConfig_.rendering_)
	{
		auto entities = entities_.getEntities();
		for (auto& entity : entities)
		{
			entity->getComponent<ShapeComponent>().circle_.setRotation(entity->getComponent<TransformComponent>().angle_);
			entity->getComponent<ShapeComponent>().circle_.setPosition(entity->getComponent<TransformComponent>().pos_);
			window_.draw(entity->getComponent<ShapeComponent>().circle_);
		}
		scoreText_.setString("SCORE: " + std::to_string(score_));
		highScoreText_.setString("HIGH SCORE: " + std::to_string(highScore_));
		window_.draw(scoreText_);
		window_.draw(highScoreText_);
		isSpecialWeaponAvailable_ ? specialWeaponText_.setString(specialWeaponAvailable_) : specialWeaponText_.setString(specialWeaponCooldown_);
		window_.draw(specialWeaponText_);
		if (paused_) { window_.draw(pauseText_); }
	}

	ImGui::SFML::Render(window_);
	window_.display();
}

std::shared_ptr<Entity>	Game::player()
{
	auto& players = entities_.getEntities("player");

	if (players.size() != 1)
	{
		SPDLOG_ERROR("There must be 1 player!");
		exit(1);
	}
	return (players.front());
}
