#pragma once

#include "json.hpp"

namespace PluginTemplate
{
	inline nlohmann::json config;
	inline bool isDebug{ false };

	inline int counter = 0;
}