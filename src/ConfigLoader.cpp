#include "ConfigLoader.h"

# include <spdlog/spdlog.h>
# include <fstream>

GameConfig	ConfigLoader::loadFromFile(const std::string& configPath)
{
	std::ifstream config{configPath};
	if (!config)
	{
		SPDLOG_ERROR("Failed to open config file: {}", configPath);
		exit(1);
	}

	json data = json::parse(config, nullptr, false);
	if (data.is_discarded())
	{
		SPDLOG_ERROR("Failed to parse config file: {}", configPath);
		exit(1);
	}

	GameConfig gameConfig;
	if (data.contains("window")) { loadWindowConfig(gameConfig.windowConfig_, data["window"]); }
	if (data.contains("player")) { loadPlayerConfig(gameConfig.playerConfig_, data["player"]); }
	if (data.contains("enemy")) { loadEnemyConfig(gameConfig.enemyConfig_, data["enemy"]); }
	if (data.contains("bullet")) { loadBulletConfig(gameConfig.bulletConfig_, data["bullet"]); }
	if (data.contains("ui")) { loadUIConfig(gameConfig.uiConfig_, data["ui"]); }

	return (gameConfig);
}

void	ConfigLoader::loadWindowConfig(WindowConfig& windowConfig, const json& window)
{
	windowConfig.width_ = window.value("width", 1280);
	windowConfig.height_ = window.value("height", 720);
	windowConfig.fullscreen_ = window.value("fullscreen", false);
	windowConfig.title_ = window.value("title", "Geometry Wars");
	windowConfig.frameLimit_ = window.value("frameLimit", 60);
}

void	ConfigLoader::loadPlayerConfig(PlayerConfig& playerConfig, const json& player)
{
	if (player.contains("pos"))
	{
		const auto& pos = player["pos"];
		playerConfig.pos_.x_ = pos.value("relativeX", 0.5f);
		playerConfig.pos_.y_ = pos.value("relativeY", 0.5f);
	}
	playerConfig.shapeRadius_ = player.value("shapeRadius", 25.0f);
	playerConfig.collisionRadius_ = player.value("collisionRadius", 25.0f);
	if (player.contains("fillColor"))
	{
		const auto& fillColor = player["fillColor"];
		playerConfig.fillColor_.r_ = fillColor.value("r", 5);
		playerConfig.fillColor_.g_ = fillColor.value("g", 5);
		playerConfig.fillColor_.b_ = fillColor.value("b", 5);
		playerConfig.fillColor_.a_ = fillColor.value("a", 255);
	}
	if (player.contains("outlineColor"))
	{
		const auto& outlineColor = player["outlineColor"];
		playerConfig.outlineColor_.r_ = outlineColor.value("r", 255);
		playerConfig.outlineColor_.g_ = outlineColor.value("g", 0);
		playerConfig.outlineColor_.b_ = outlineColor.value("b", 0);
		playerConfig.outlineColor_.a_ = outlineColor.value("a", 255);
	}
	playerConfig.outlineThickness_ = player.value("outlineThickness", 4.0f);
	playerConfig.vertices_ = player.value("vertices", 8);
	if (player.contains("velocity"))
	{
		const auto& velocity = player["velocity"];
		playerConfig.velocity_.x_ = velocity.value("x", 5.0f);
		playerConfig.velocity_.y_ = velocity.value("y", 5.0f);
	}
}

void	ConfigLoader::loadEnemyConfig(EnemyConfig& enemyConfig, const json& enemy)
{
	enemyConfig.shapeRadius_ = enemy.value("shapeRadius", 25.0f);
	enemyConfig.collisionRadius_ = enemy.value("collisionRadius", 25.0f);
	if (enemy.contains("outlineColor"))
	{
		const auto& outlineColor = enemy["outlineColor"];
		enemyConfig.outlineColor_.r_ = outlineColor.value("r", 255);
		enemyConfig.outlineColor_.g_ = outlineColor.value("g", 255);
		enemyConfig.outlineColor_.b_ = outlineColor.value("b", 255);
		enemyConfig.outlineColor_.a_ = outlineColor.value("a", 255);
	}
	enemyConfig.outlineThickness_ = enemy.value("outlineThickness", 4.0f);
	if (enemy.contains("verticesRange"))
	{
		const auto& verticesRange = enemy["verticesRange"];
		enemyConfig.verticeRange_.min_ = verticesRange.value("min", 3);
		enemyConfig.verticeRange_.max_ = verticesRange.value("max", 8);
	}
	if (enemy.contains("speedRange"))
	{
		const auto& speedRange = enemy["speedRange"];
		enemyConfig.speedRange_.min_ = speedRange.value("min", 1.0f);
		enemyConfig.speedRange_.max_ = speedRange.value("max", 15.0f);
	}
	enemyConfig.smallEnemyLifespan_ = enemy.value("smallEnemyLifespan", 90);
	enemyConfig.spawnInterval_ = enemy.value("spawnInterval", 60);
}

void	ConfigLoader::loadBulletConfig(BulletConfig& bulletConfig, const json& bullet)
{
	bulletConfig.shapeRadius_ = bullet.value("shapeRadius", 10.0f);
	bulletConfig.collisionRadius_ = bullet.value("collisionRadius", 10.0f);
	if (bullet.contains("fillColor"))
	{
		const auto& fillColor = bullet["fillColor"];
		bulletConfig.fillColor_.r_ = fillColor.value("r", 255);
		bulletConfig.fillColor_.g_ = fillColor.value("g", 255);
		bulletConfig.fillColor_.b_ = fillColor.value("b", 255);
		bulletConfig.fillColor_.a_ = fillColor.value("a", 255);
	}
	if (bullet.contains("outlineColor"))
	{
		const auto& outlineColor = bullet["outlineColor"];
		bulletConfig.outlineColor_.r_ = outlineColor.value("r", 255);
		bulletConfig.outlineColor_.g_ = outlineColor.value("g", 255);
		bulletConfig.outlineColor_.b_ = outlineColor.value("b", 255);
		bulletConfig.outlineColor_.a_ = outlineColor.value("a", 255);
	}
	bulletConfig.outlineThickness_ = bullet.value("outlineThickness", 2.0f);
	bulletConfig.speed_ = bullet.value("speed", 10.0f);
	bulletConfig.vertices_ = bullet.value("vertices", 20);
	bulletConfig.lifespan_ = bullet.value("lifespan", 75);
}

void	ConfigLoader::loadUIConfig(UIConfig &uiConfig, const json &ui)
{
	std::vector<std::pair<Font&, std::string>> fontConfigs{
		{uiConfig.score_, "scoreFont"},
		{uiConfig.highScore_, "highScoreFont"},
		{uiConfig.specialWeapon_, "specialWeaponFont"},
		{uiConfig.pause_, "pauseFont"}
	};

	for (auto& fontConfig : fontConfigs)
	{
		if (ui.contains(fontConfig.second))
		{
			loadFont(fontConfig.first, ui[fontConfig.second]);
		}
	}
}

void	ConfigLoader::loadFont(Font& font, const json& fontConfig)
{
	if (fontConfig.contains("pos"))
	{
		const auto& pos = fontConfig["pos"];
		if (pos.contains("relativeX")) { font.pos_.x_ = pos["relativeX"]; }
		if (pos.contains("relativeY")) { font.pos_.y_ = pos["relativeY"]; }
	}
	if (fontConfig.contains("path")) { font.path_ = fontConfig["path"]; }
	if (fontConfig.contains("size")) { font.size_ = fontConfig["size"]; }
	if (fontConfig.contains("color"))
	{
		const auto& color = fontConfig["color"];
		if (color.contains("r")) { font.color_.r_ = color["r"]; }
		if (color.contains("g")) { font.color_.g_ = color["g"]; }
		if (color.contains("b")) { font.color_.b_ = color["b"]; }
		if (color.contains("a")) { font.color_.a_ = color["a"]; }
	}
}
