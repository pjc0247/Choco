#include "db_pool.h"

#include <thread>
#include <chrono>

#include "db_mysql.h"
#include "conig.h"

using namespace std;
using namespace std::chrono;

namespace Choco{
namespace DB{
	
	int Pool::initialize(){
		int poolSize;
		int dbType;

		int port;
		string host;
		string user, password;

		/* Pool Data */
		poolSize =
			getConfigAsInteger("db_pool_size");
		dbType = 
			getDBType( getConfig("db_name") );

		/* Connection Data */
		host = getConfig("db_server_host");
		port = getConfigAsInteger("db_server_port");
		user = getConfig("db_user");
		password = getConfig("db_password");

		for(int i=0;i<poolSize;i++){
			Interface *db;

			switch( dbType ){
			case DBType::MYSQL:
				db = new Mysql();
				break;
			case DBType::CUBRID:
				break;
			}
			queue.push( db );

			int ret;
			ret = db->initialize(
				host, port,
				user, password);

			if( ret != 0 )
				return ret;
		}

		return 0;
	}
	void Pool::quit(){
		while( !queue.empty() ){
			Interface *db;

			if( queue.try_pop(db) ){
				db->quit();
				delete db;
			}
		}
	}

	Interface *Pool::tryRequire(){
		Interface *db;
		
		if( queue.try_pop(db) )
			return db;
		else
			return nullptr;
	}
	Interface *Pool::require(){
		Interface *db;
		
		while( true ){
			if( queue.try_pop(db) )
				break;

			this_thread::sleep_for(
				milliseconds(1) );
		}

		return db;
	}
	void Pool::restore(Interface *db){
		queue.push(db);
	}

	int Pool::getDBType(const string &name){
		if( name == "mysql" ) 
			return DBType::MYSQL;
		else if( name == "cubrid" ) 
			return DBType::CUBRID;

		return 0;
	}
};
};