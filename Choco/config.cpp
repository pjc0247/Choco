#include "conig.h"

#include <map>

using namespace std;

namespace Choco{

	static map<string,string> configs;

	void configure(
		const string &key, const string &value){

		setConfig( key, value );
	}

	void setConfig(
		const string &key, const string &value){
				
		configs[key] = value;
	}
	void setConfigWithNoOverride(
		const string &key, const string &value){

		auto pair = configs.find( key );

		if( pair == configs.end() )
			setConfig( key, value );
	}
	const string &getConfig(const string &key){
		return configs[key];
	}
	int getConfigAsInteger(const string &key){
		string value = getConfig(key);
		int intValue;

		sscanf(
			value.c_str(),
			"%d", &intValue);

		return intValue;
	}

	void configureDefault(){
		setConfigWithNoOverride(
			"worker_count", "10");

		setConfigWithNoOverride(
			"inbuf_size", "1024");
	}
};