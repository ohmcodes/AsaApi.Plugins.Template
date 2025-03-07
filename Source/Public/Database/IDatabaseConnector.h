#ifndef IDATABASECONNECTOR_H
#define IDATABASECONNECTOR_H

#pragma once

#include <json.hpp>
#include <string>
#include <vector>

#include <API/ARK/Ark.h>

class IDatabaseConnector
{
public:
	virtual ~IDatabaseConnector() = default;

	virtual bool createTableIfNotExist(const std::string& tableName, const nlohmann::ordered_json& tableDefinition) = 0;

	virtual bool alterTableIfNotExists(const std::string& tableName, const nlohmann::ordered_json& tableDefinition) = 0;

	virtual bool reorderColumns(const std::string& tableName, const nlohmann::ordered_json& tableDefinition) = 0;

	virtual bool create(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data) = 0;

	virtual bool read(const std::string& query, std::vector<std::map<std::string,std::string>>& results) = 0;

	virtual bool update(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data, const std::string& condition) = 0;

	virtual bool deleteRow(const std::string& tableName, const std::string& condition) = 0;

	virtual std::string escapeString(const std::string& value) = 0;
};


#endif //IDATABASECONNECTOR_H