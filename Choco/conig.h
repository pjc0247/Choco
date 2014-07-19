#pragma once

#include <string>

namespace Choco{
	
	/* renamed version of setConfig */
	void configure(
		const std::string &key, const std::string &value);

	void setConfig(
		const std::string &key, const std::string &value);
	void setConfigWithNoOverride(
		const std::string &key, const std::string &value);
	const std::string &getConfig(const std::string &key);
	int getConfigAsInteger(const std::string &key);

	void configureDefault();
};