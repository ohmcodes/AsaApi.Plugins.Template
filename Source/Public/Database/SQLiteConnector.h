#ifndef SQLITECONNECTOR_H
#define SQLITECONNECTOR_H

#pragma once

#include "IDatabaseConnector.h"
#include "sqlite3/sqlite3.h"

class SQLiteConnector : public IDatabaseConnector
{
public:
	SQLiteConnector(const std::string& dbName);
	~SQLiteConnector();


	bool createTableIfNotExist(const std::string& tableName, const nlohmann::ordered_json& tableDefinition);

	bool alterTableIfNotExists(const std::string& tableName, const nlohmann::ordered_json& tableDefinition);

	bool reorderColumns(const std::string& tableName, const nlohmann::ordered_json& tableDefinition);

	bool create(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data);

	bool read(const std::string& query, std::vector<std::map<std::string, std::string>>& results);

	bool update(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data, const std::string& condition);

	bool deleteRow(const std::string& tableName, const std::string& condition);

	std::string escapeString(const std::string& value);

private:
	sqlite3* db;
	char* errorMessage;

	void printError(const std::string& message);
	bool executeQuery(const std::string& query);

};

#endif