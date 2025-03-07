
#include <fstream>

void ReadConfig()
{
	try
	{
		const std::string config_path = AsaApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/" + PROJECT_NAME + "/config.json";
		std::ifstream file{config_path};
		if (!file.is_open())
		{
			throw std::runtime_error("Can't open config file.");
		}
		file >> PluginTemplate::config;

		Log::GetLog()->info("{} config file loaded.", PROJECT_NAME);

		PluginTemplate::isDebug = PluginTemplate::config["General"]["Debug"].get<bool>();

		Log::GetLog()->warn("Debug {}", PluginTemplate::isDebug);

	}
	catch(const std::exception& error)
	{
		Log::GetLog()->error("Config load failed. ERROR: {}", error.what());
		throw;
	}
}

void LoadDatabase()
{
	PluginTemplate::pluginTemplateDB = DatabaseFactory::createConnector(PluginTemplate::config["PluginDBSettings"]);

	nlohmann::ordered_json tableDefinition = {};
	if (PluginTemplate::config["PluginDBSettings"].value("UseMySQL", true))
	{
		tableDefinition = {
			{"Id", "INT NOT NULL AUTO_INCREAMENT"},
			{"EosId", "VARCHAR(50) NOT NULL"},
			{"PlayerId", "VARCHAR(50) NOT NULL"},
			{"PlayerName", "VARCHAR(50) NOT NULL"},
			{"CreateAt", "DATETIME DEFAULT CURRENT_TIMESTAMP"},
			{"PRIMARY", "KEY(Id)"},
			{"UNIQUE", "INDEX EosId_UNIQUE (EosId ASC)"}
		};
	}
	else
	{
		// SQLite
	}

	PluginTemplate::pluginTemplateDB->createTableIfNotExist(PluginTemplate::config["PluginDBSettings"].value("TableName", ""), tableDefinition);

	
}