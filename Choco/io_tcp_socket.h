#ifdef DEPRECATED

#pragma once

#include "io_interface.h"

#include <string>

#include <WinSock2.h>

namespace Choco{
	namespace IO{
		

		class TCPSocket : public Interface{
		public:
			TCPSocket();
			virtual ~TCPSocket();

			virtual int initialize();
			virtual int initialize(char *inbuf,int inbufSize);
			virtual void quit();

			virtual int write(char *data,int len, int flags);
			virtual int read(char *data,int len, int flags);
			virtual int read(int flags);

			void reset(SOCKET acceptSocket);

			SOCKET getSocket();
			char *getBuffer();
			int getBufferSize();

			SOCKADDR_IN &getLocalAddr();
			SOCKADDR_IN &getRemoteAddr();

		protected:
			TCPSocketData *sock;

			char *inbuf;
			int inbufSize;
		};
	}
};

#endif //DEPRECATED