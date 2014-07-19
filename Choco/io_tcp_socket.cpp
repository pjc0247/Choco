#ifdef DEPRECATED

#include "io_tcp_socket.h"

#include "io_errno.h"
#include "common_errno.h"

#include <WinSock2.h>
#include <Windows.h>
#include <mswsock.h>

using namespace std;

namespace Choco{
namespace IO{

	struct TCPSocketData{
		SOCKET socket;
		SOCKADDR_IN local, remote;

		OvData ov;
	};

	TCPSocket::TCPSocket() :
		sock(nullptr),
		inbuf(nullptr), inbufSize(0){
	}
	TCPSocket::~TCPSocket(){
	}

	int TCPSocket::initialize(){
		sock = new(nothrow) TCPSocketData();
		if( sock == nullptr )
			return AllocationError;

		sock->socket = 
			socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( sock->socket == INVALID_SOCKET )
			return SocketError;

		int val = 0;

		setsockopt(
			sock->socket,
			SOL_SOCKET, SO_SNDBUF, 
			(char *)&val, sizeof(val));
		setsockopt(
			sock->socket,
			SOL_SOCKET, SO_RCVBUF, 
			(char *)&val, sizeof(val));

		memset( &sock->ov,0, sizeof(OVERLAPPED) );

		sock->ov.socket = this;

		return 0;
	}
	int TCPSocket::initialize(char *_inbuf,int _inbufSize){
		int ret = 0;

		ret = initialize();

		inbuf = _inbuf;
		inbufSize = _inbufSize;

		return ret;
	}
	void TCPSocket::quit(){
		if( sock == nullptr )
			return;
		if( sock->socket != INVALID_SOCKET ) 
			closesocket( sock->socket );

		delete sock;
	}
	int TCPSocket::write(char *data,int len, int flags){
		WSABUF buf;
		DWORD written;

		buf.buf = data;
		buf.len = len;
		sock->ov.action = Write;

		WSASend(
			sock->socket,
			&buf, 1, &written, flags,
			nullptr, nullptr);

		return 0;
	}
	int TCPSocket::read(char *data,int len, int flags){
		WSABUF buf;
		DWORD written;

		buf.buf = inbuf;
		buf.len = inbufSize;
		sock->ov.action = Read;

		return WSARecv(
			sock->socket,
			&buf, 1, &written, (DWORD*)&flags,
			&sock->ov, nullptr);
	}
	int TCPSocket::read(int flags){
		return read(
			inbuf, inbufSize,
			flags);
	}

	void TCPSocket::reset(SOCKET acceptSocket){
		TransmitFile(
			sock->socket, nullptr, 0, 0,
			(OVERLAPPED*)&sock->ov, nullptr,
			TF_DISCONNECT | TF_REUSE_SOCKET);

		sock->ov.action = Accept;

		BOOL ret;
		ret = AcceptEx(
			acceptSocket,
			sock->socket,
			inbuf, 0,
			sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
			nullptr, &sock->ov);
	}

	SOCKET TCPSocket::getSocket(){
		return sock->socket;
	}
	char *TCPSocket::getBuffer(){
		return inbuf;
	}
	int TCPSocket::getBufferSize(){
		return inbufSize;
	}

	SOCKADDR_IN &TCPSocket::getLocalAddr(){
		return sock->local;
	}
	SOCKADDR_IN &TCPSocket::getRemoteAddr(){
		return sock->remote;
	}
}
};

#endif //DEPRECATED