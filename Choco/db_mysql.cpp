#include "db_mysql.h"

#include "common_errno.h"

/* MYSQL error message */
#include <errmsg.h>

#pragma comment (lib, "mysqlclient")

using namespace std;

namespace Choco{
namespace DB{
	
	Mysql::Mysql(){
	}
	Mysql::~Mysql(){
	}
	
	int Mysql::initialize(
		const string &host, int port,
		const string &user, const string &password){

		mysql = mysql_init( nullptr );
		if( mysql == nullptr )
			return AllocationError;

		MYSQL *ret;
		ret = mysql_real_connect(
			mysql,
			host.c_str(),
			user.c_str(), password.c_str(),
			nullptr, port,
			nullptr, 0 );
		if( ret == nullptr ){
			switch( mysql_errno(mysql) ){
			case CR_CONN_HOST_ERROR:
				return HostError;
			case CR_CONNECTION_ERROR:
				return ConnectionError;
			case CR_ALREADY_CONNECTED:
				return AlreadyConnectedError;
			case CR_SOCKET_CREATE_ERROR:
				return SocketError;
			case CR_SERVER_LOST:
				return ServerLostError;
			default:
				return UnknownError;
			}
		}
		
		return 0;
	}
	void Mysql::quit(){
		mysql_close( mysql );
		mysql = nullptr;
	}

	int Mysql::query(const string &query){
		int ret;

		ret = mysql_query(
			mysql, query.c_str());

		switch( ret ){
		case 0:
			return 0;
		case CR_SERVER_LOST:
			return ServerLostError;
		default:
			return UnknownError;
		}
	}

	Result *Mysql::storeResult(){
		Result *result = new Result();

		result->driverData =
			mysql_store_result( mysql );

		return result;
	}
	void Mysql::freeResult(Result *result){
		mysql_free_result( 
			(MYSQL_RES*)result->driverData);

		delete result;
	}
};
};