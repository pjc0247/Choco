#pragma once

#include <vector>
#include <thread>

#include "session_pool.h"
#include "app_interface.h"

namespace Choco{
	namespace App{

		/* OS specific server data */
		struct ServerData;

		class Server{
		public:
			Server();
			virtual ~Server();

			int initialize();
			void quit();

			int run(Interface *handler);

		protected:
			int initializeWorkers();
			int initializeServer();
			void quitWorkers();
			void quitServer();

			int processAccept(Session::Client *session);
			int processRead(Session::Client *session, int len);
			int processWrite(Session::Client *session, int len);

			void worker();

		protected:
			Session::Pool sessionPool;

			std::vector<std::thread> workers;
			bool running;

			Interface *handler;

			ServerData *data;
		};
	};
};