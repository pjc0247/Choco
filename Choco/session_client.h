#pragma once

#include "io_tcp_socket.h"

#include <time.h>

#include <WinSock2.h>
#include <Windows.h>

namespace Choco{
	namespace Session{

		class Client;

		/* OS specific client data */
		struct ClientData;

		enum Action{
			Accept =1,
			Read,
			Write
		};
		struct OvData : OVERLAPPED{
			int action;
			DWORD transffered;
			DWORD recvFlags;
			Client *session;
		};

		class Client{
		public:
			Client();	
			virtual ~Client();

			int initialize();
			void quit();
			void reset(SOCKET acceptSocket);

			void write(char *buffer,int len, int flags =0);
			/* read from this session, to specific buffer and size */
			void read(char *buffer,int len, int flags =0);
			/* read from this session, to default inbuffer */
			void read(int flags =0);

			void setUserData(void *userData);
			void *getUserData();

			time_t getLastAccessTime();
			time_t getUptime();

			SOCKET getSocket();
			SOCKADDR_IN &getLocalAddr();
			SOCKADDR_IN &getRemoteAddr();

			char *getBuffer();
			int getBufferSize();

		protected:
			ClientData *data;

			void *userData;
			time_t lastAccess;
			time_t createdAt;

			char *inbuf;
			int inbufSize;
		};
	}
};