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

		if (config.value("UseMySQL", true))
		{	
			
		}
		else
		{
			/*Log::GetLog()->warn("Goes here");
			std::string default_path = AsaApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/" + PROJECT_NAME + "/" + ".db";
			std::string sqlitePath = config.value("SQLiteDatabasePath","");
			std::string db_path = (sqlitePath == "") ? default_path : sqlitePath;

			return std::make_unique<SQLiteConnector>(db_path);*/
		}
	}
};

#endif // DATABASEFACTORY_H