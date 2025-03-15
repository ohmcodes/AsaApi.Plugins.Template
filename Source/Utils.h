
#include <fstream>

TArray<FString> GetPlayerPermissions(FString eos_id)
{
	TArray<FString> PlayerPerms = { "Default" };

	std::string escaped_eos_id = PluginTemplate::permissionsDB->escapeString(eos_id.ToString());

	std::string tablename = PluginTemplate::config["PermissionsDBSettings"].value("TableName", "Players");

	std::string wherefield = PluginTemplate::config["PermissionsDBSettings"].value("UniqueIDField", "EOS_Id");

	std::string query = fmt::format("SELECT * FROM {} WHERE {}='{}'", tablename, wherefield, escaped_eos_id);

	std::vector<std::map<std::string, std::string>> results;
	if (!PluginTemplate::permissionsDB->read(query, results))
	{
		if (PluginTemplate::config["Debug"].value("Permissions", false) == true)
		{
			Log::GetLog()->warn("Error reading permissions DB");
		}

		return PlayerPerms;
	}

	if (results.size() <= 0) return PlayerPerms;

	std::string permsfield = PluginTemplate::config["PermissionsDBSettings"].value("PermissionGroupField","PermissionGroups");

	FString playerperms = FString(results[0].at(permsfield));

	if (PluginTemplate::config["Debug"].value("Permissions", false) == true)
	{
		Log::GetLog()->info("current player perms {}", playerperms.ToString());
	}

	playerperms.ParseIntoArray(PlayerPerms, L",", true);

	return PlayerPerms;
}

FString GetPriorPermByEOSID(FString eos_id)
{
	TArray<FString> player_groups = GetPlayerPermissions(eos_id);

	const nlohmann::json permGroups = PluginTemplate::config["PermissionGroups"];

	std::string defaultGroup = "Default";
	int minPriority = INT_MAX;
	nlohmann::json result;
	FString selectedPerm = "Default";

	for (const FString& param : player_groups)
	{
		if (permGroups.contains(param.ToString()))
		{
			int priority = static_cast<int>(permGroups[param.ToString()]["Priority"]);
			if (priority < minPriority)
			{
				minPriority = priority;
				result = permGroups[param.ToString()];
				selectedPerm = param;
			}
		}
	}

	if (result.is_null() && permGroups.contains(defaultGroup))
	{
		result = permGroups[defaultGroup];
	}

	if (PluginTemplate::config["Debug"].value("Permissions", false) == true)
	{
		Log::GetLog()->info("Selecter Permission {}", selectedPerm.ToString());
	}

	return selectedPerm;
}

bool AddPlayer(FString eosID, int playerID, FString playerName)
{
	std::vector<std::pair<std::string, std::string>> data = {
		{"EosId", eosID.ToString()},
		{"PlayerId", std::to_string(playerID)},
		{"PlayerName", playerName.ToString()}
	};

	return PluginTemplate::pluginTemplateDB->create(PluginTemplate::config["PluginDBSettings"]["TableName"].get<std::string>(), data);
}

bool ReadPlayer(FString eosID)
{
	std::string escaped_id = PluginTemplate::pluginTemplateDB->escapeString(eosID.ToString());

	std::string query = fmt::format("SELECT * FROM {} WHERE EosId='{}'", PluginTemplate::config["PluginDBSettings"]["TableName"].get<std::string>(), escaped_id);

	std::vector<std::map<std::string, std::string>> results;
	PluginTemplate::pluginTemplateDB->read(query, results);

	return results.size() <= 0 ? false : true;
}

bool UpdatePlayer(FString eosID, FString playerName)
{
	std::string unique_id = "EosId";

	std::string escaped_id = PluginTemplate::pluginTemplateDB->escapeString(eosID.ToString());

	std::vector<std::pair<std::string, std::string>> data = {
		{"PlayerName", playerName.ToString() + "123"}
	};

	std::string condition = fmt::format("{}='{}'", unique_id, escaped_id);

	return PluginTemplate::pluginTemplateDB->update(PluginTemplate::config["PluginDBSettings"]["TableName"].get<std::string>(), data, condition);
}

bool DeletePlayer(FString eosID)
{
	std::string escaped_id = PluginTemplate::pluginTemplateDB->escapeString(eosID.ToString());

	std::string condition = fmt::format("EosId='{}'", escaped_id);

	return PluginTemplate::pluginTemplateDB->deleteRow(PluginTemplate::config["PluginDBSettings"]["TableName"].get<std::string>(), condition);
}

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
	Log::GetLog()->warn("LoadDatabase");
	PluginTemplate::pluginTemplateDB = DatabaseFactory::createConnector(PluginTemplate::config["PluginDBSettings"]);

	nlohmann::ordered_json tableDefinition = {};
	if (PluginTemplate::config["PluginDBSettings"].value("UseMySQL", true) == true)
	{
		tableDefinition = {
			{"Id", "INT NOT NULL AUTO_INCREMENT"},
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
		tableDefinition = {
			{"Id","INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT"},
			{"EosId","TEXT NOT NULL UNIQUE"},
			{"PlayerId","TEXT"},
			{"PlayerName","TEXT"},
			{"CreateAt","TIMESTAMP DEFAULT CURRENT_TIMESTAMP"}
		};
	}

	PluginTemplate::pluginTemplateDB->createTableIfNotExist(PluginTemplate::config["PluginDBSettings"].value("TableName", ""), tableDefinition);


	// PermissionsDB
	if (PluginTemplate::config["PermissionsDBSettings"].value("Enabled", true) == true)
	{
		PluginTemplate::permissionsDB = DatabaseFactory::createConnector(PluginTemplate::config["PermissionsDBSettings"]);
	}

	// PointsDB (ArkShop)
	if (PluginTemplate::config["PointsDBSettings"].value("Enabled", true) == true)
	{
		PluginTemplate::pointsDB = DatabaseFactory::createConnector(PluginTemplate::config["PointsDBSettings"]);
	}
	
}