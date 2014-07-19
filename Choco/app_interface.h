#pragma once

#include "session_client.h"

namespace Choco{
	namespace App{

		class Interface{
		public:
			virtual int onLoad() =0;
			virtual void onQuit() =0;

			virtual int onConnected(Session::Client *session) =0;
			virtual void onDisconnected(Session::Client *session) =0;
			virtual void onReceived(
				Session::Client *session,
				char *data,int len) =0;
		};
	};
};