#ifndef CONFIG_LOADER_H
# define CONFIG_LOADER_H

# include <nlohmann/json.hpp>

# include "GameConfig.h"

using json = nlohmann::json;

class ConfigLoader
{
	public:
		static GameConfig	loadFromFile(const std::string& configPath);

	private:
		static void			loadWindowConfig(WindowConfig& windowConfig, const json& window);
		static void 		loadPlayerConfig(PlayerConfig& playerConfig, const json& player);
		static void			loadEnemyConfig(EnemyConfig& enemyConfig, const json& enemy);
		static void			loadBulletConfig(BulletConfig& bulletConfig, const json& bullet);
		static void			loadUIConfig(UIConfig& uiConfig, const json& ui);
		static void			loadFont(Font& font, const json& ui);
};

#endif