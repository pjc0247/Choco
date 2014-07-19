#pragma once

#include "db_interface.h"

#include <mysql.h>

namespace Choco{
	namespace DB{

		struct mysql_t;

		class Mysql : public Interface{
		public:
			Mysql();
			virtual ~Mysql();

			virtual int initialize(
				const std::string &host, int port,
				const std::string &user, const std::string &password);
			virtual void quit();

			virtual int query(const std::string &query);
			virtual Result *storeResult();
			virtual void freeResult(Result *result);

		protected:
			MYSQL *mysql;
		};
	};
};