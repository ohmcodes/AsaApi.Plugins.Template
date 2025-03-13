#ifndef DATABASEFACTORY_H
#define DATABASEFACTORY_H

#pragma once

#include "MySQLConnector.h"
#include "SQLiteConnector.h"
#include <json.hpp>
#include <memory>

class DatabaseFactory
{
public:
	static std::unique_ptr<IDatabaseConnector> createConnector(const nlohmann::json config)
	{
		if (config.value("UseMySQL", true))
		{	
			int sslMode = config.value("MysqlSSLMode", -1);
			std::string tlsVersion = config.value("MysqlTLSVersion", "");

			return std::make_unique<MySQLConnector>(
				config["Host"],
				config["User"],
				config["Password"],
				config["Database"],
				config["Port"],
				sslMode,
				tlsVersion
			);
		}
		else
		{
			nlohmann::json sqliteConfig = config.value("PluginDBSettings", {});
			std::string default_path = AsaApi::Tools::GetCurrentDir() + "/ArkApi/Plugin/" + PROJECT_NAME + "/" + ".db";
			std::string sqlitePath = sqliteConfig["SQLiteDatabasePath"].get<std::string>();
			std::string db_path = (sqlitePath == "") ? default_path : sqlitePath;

			return std::make_unique<SQLiteConnector>(db_path);
		}
	}
};

#endif // DATABASEFACTORY_H