#pragma once

#include "session_client.h"

#include <functional>
#include <concurrent_queue.h>

namespace Choco{
	namespace Session{

		class Pool{
		public:
			Pool();
			virtual ~Pool();

			int initialize();
			void quit();

			int getSize();

			void iterate(std::function<void(Client*)> cb);

		protected:
			concurrency::concurrent_queue<Client*> queue;

			int size;
		};
	};
};