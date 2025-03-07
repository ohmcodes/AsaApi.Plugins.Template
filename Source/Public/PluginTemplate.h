#pragma once

#include "json.hpp"

#include "Database/DatabaseFactory.h"

namespace PluginTemplate
{
	inline nlohmann::json config;
	inline bool isDebug{ false };

	inline int counter = 0;

	inline std::unique_ptr<IDatabaseConnector> pluginTemplateDB;

}