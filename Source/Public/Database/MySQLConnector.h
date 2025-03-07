#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#pragma once

#include "IDatabaseConnector.h"
#include <mysql/mysql.h>

#pragma comment(lib,"mysqlclient.lib")

class MySQLConnector : public IDatabaseConnector
{
public:
	MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& dbname, unsigned int port, int ssl_mode, const std::string& tls_version);

	~MySQLConnector();

	bool createTableIfNotExist(const std::string& tableName, const nlohmann::ordered_json& tableDefinition);

	bool alterTableIfNotExists(const std::string& tableName, const nlohmann::ordered_json& tableDefinition);

	bool reorderColumns(const std::string& tableName, const nlohmann::ordered_json& tableDefinition);

	bool create(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data);

	bool read(const std::string& query, std::vector<std::map<std::string, std::string>>& results);

	bool update(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data, const std::string& condition);

	bool deleteRow(const std::string& tableName, const std::string& condition);

	std::string escapeString(const std::string& value);

private:
	MYSQL* conn;
	MYSQL_RES* result;
	std::string _host;
	std::string _user;
	std::string _password;
	std::string _dbname;
	unsigned int _port;
	int _ssl_mode;
	std::string _tls_version;

	void configureSSL(int ssl_mode, const std::string& tls_version);
	void printError(const std::string& message);
	bool executeQuery(const std::string& query);
	bool MySQLConnect();

};

#endif