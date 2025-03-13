#include "Database/SQLiteConnector.h"

SQLiteConnector::SQLiteConnector(const std::string& dbName)
{
	if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
	{
		printError("Failed to open SQLite database");
		db = nullptr;
	}
}

SQLiteConnector::~SQLiteConnector()
{
	if (db != nullptr)
	{
		sqlite3_close(db);
	}
}

bool SQLiteConnector::createTableIfNotExist(const std::string& tableName, const nlohmann::ordered_json& tableDefinition)
{
	std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " (";

	bool first = true;
	for (auto& [columnName, dataType] : tableDefinition.items())
	{
		if (!first)
		{
			query += ", ";
		}
		first = false;
		query += columnName + " " + dataType.get<std::string>();
	}

	query += ")";

	return executeQuery(query);
}

bool SQLiteConnector::alterTableIfNotExists(const std::string& tableName, const nlohmann::ordered_json& tableDefinition)
{
	std::string query = "ALTER TABLE " + tableName + " ";

	bool first = true;
	for (auto& [columnName, dataType] : tableDefinition.items())
	{
		if (!first)
		{
			query += ", ";
		}
		first = false;
		query += "ADD COLUMN " + columnName + " " + dataType.get<std::string>();
	}

	query += ";";

	return executeQuery(query);
}

// NOT USING
bool SQLiteConnector::reorderColumns(const std::string& tableName, const nlohmann::ordered_json& tableDefinition)
{
	return false;
}

// TODO: TEST
bool SQLiteConnector::create(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data)
{
	std::string query = "INSERT INTO " + tableName + " (";
	std::string values = " VALUES (";

	for (size_t i = 0; i < data.size(); i++)
	{
		query += data[i].first;
		values += "'" + escapeString(data[i].second) + "'";
		if (i < data.size() - 1)
		{
			query += ", ";
			values += ", ";
		}
	}

	query += ") " + values + ")";

	return executeQuery(query);
}

bool SQLiteConnector::read(const std::string& query, std::vector<std::map<std::string, std::string>>& results)
{
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		printError("Failed to prepareSQLite statement");
		return false;
	}

	int numCols = sqlite3_column_count(stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::map<std::string, std::string> rowData;
		
		for (int i = 0; i < numCols; i++)
		{
			const char* columnName = sqlite3_column_name(stmt, i);
			const char* val = (const char*)sqlite3_column_text(stmt, i);
			rowData[columnName] = val ? val : "NULL";
		}
		results.push_back(rowData);
	}

	sqlite3_finalize(stmt);
	return true;
}

// TODO: TEST
bool SQLiteConnector::update(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& data, const std::string& condition)
{
	std::string query = "UPDATE " + tableName + " SET ";

	for (size_t i = 0; i < data.size(); ++i)
	{
		query += data[i].first + " = '" + escapeString(data[i].second) + "'";
		if (i < data.size() - 1)
		{
			query += ", ";
		}
	}

	query += " WHERE " + condition;

	return executeQuery(query);
}

bool SQLiteConnector::deleteRow(const std::string& tableName, const std::string& condition)
{
	std::string query = "DELETE FROM " + tableName + " WHERE " + condition;

	return executeQuery(query);
}

std::string SQLiteConnector::escapeString(const std::string& value)
{
	std::string escaped;
	escaped.reserve(value.length());

	for (char c : value)
	{
		switch (c)
		{
		case '\'':
			escaped += "''";
			break;
		case '\"':
			escaped += "\"";
			break;
		case '\\':
			escaped += "\\";
			break;
		default:
			escaped += c;
			break;
		}
	}

	return escaped;
}


void SQLiteConnector::printError(const std::string& message)
{
	Log::GetLog()->error("{} {}", message, sqlite3_errmsg(db));
}

bool SQLiteConnector::executeQuery(const std::string& query)
{
	int rc = sqlite3_exec(db, query.c_str(), 0, 0, &errorMessage);
	if (rc != SQLITE_OK)
	{
		printError(std::string("SQLite query error: ") + errorMessage);

		sqlite3_free(errorMessage);
		return false;
	}

	return true;
}


