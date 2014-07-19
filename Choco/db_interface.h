#pragma once

#include "db_errno.h"

#include <string>

namespace Choco{
	namespace DB{

		struct Result{
			void *driverData;
		};

		class Interface{
		public:

			virtual int initialize(
				const std::string &host, int port,
				const std::string &user, const std::string &password) =0;
			virtual void quit() =0;

			virtual int query(const std::string &query) =0;
			virtual Result *storeResult() =0;
			virtual void freeResult(Result *result) =0;
		};
	};
};