#pragma once

#include "db_interface.h"

#include <concurrent_queue.h>

namespace Choco{
	namespace DB{

		class Pool{
		public:
			int initialize();
			void quit();

			Interface *tryRequire();
			Interface *require();
			void restore(Interface *db);

		protected:
			enum DBType{
				MYSQL=1,
				CUBRID
			};

			int getDBType(const std::string &name);

		protected:
			concurrency::concurrent_queue<Interface *>
				queue;
		};
	};
};