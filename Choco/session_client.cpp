#include "session_client.h"
#include "session_errno.h"

#include "common_errno.h"

#include "conig.h"

#include <MSWSock.h>
#include <WinSock2.h>
#include <Windows.h>

using namespace std;

namespace Choco{
namespace Session{

	struct ClientData{
		SOCKET socket;
		SOCKADDR_IN local, remote;

		OvData ov;
	};

	Client::Client() :
		data(nullptr),
		userData(nullptr), createdAt(time(nullptr)),
		inbuf(nullptr), inbufSize(0){
	}
	Client::~Client(){
	}

	int Client::initialize(){
		int ret;

		inbufSize =
			getConfigAsInteger("inbuf_size");

		inbuf = new(nothrow) char[inbufSize];
		if( inbuf == nullptr )
			return AllocationError;

		data = new(nothrow) ClientData();
		if( data == nullptr )
			return AllocationError;

		/* create a socket */
		data->socket = 
			socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( data->socket == INVALID_SOCKET )
			return SocketError;

		int val = 0;

		setsockopt(
			data->socket,
			SOL_SOCKET, SO_SNDBUF, 
			(char *)&val, sizeof(val));
		setsockopt(
			data->socket,
			SOL_SOCKET, SO_RCVBUF, 
			(char *)&val, sizeof(val));

		memset( &data->ov,0, sizeof(OVERLAPPED) );
		data->ov.session = this;

		return ret;
	}
	void Client::quit(){
		if( data == nullptr )
			return;
		if( data->socket != INVALID_SOCKET ) 
			closesocket( data->socket );
		
		delete data;
	}
	void Client::reset(SOCKET acceptSocket){
		BOOL ret;

		TransmitFile(
			data->socket, nullptr, 0, 0,
			(OVERLAPPED*)&data->ov, nullptr,
			TF_DISCONNECT | TF_REUSE_SOCKET);		

		data->ov.action = Accept;
		ret = AcceptEx(
			acceptSocket,
			data->socket,
			inbuf, 0,
			sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
			nullptr, &data->ov);
	}

	void Client::setUserData(void *_userData){
		userData = _userData;
	}
	void *Client::getUserData(){
		return userData;
	}

	time_t Client::getLastAccessTime(){
		return lastAccess;
	}
	time_t Client::getUptime(){
		return time(nullptr) - createdAt;
	}

	SOCKET Client::getSocket(){
		return data->socket;
	}
	SOCKADDR_IN &Client::getLocalAddr(){
		return data->local;
	}
	SOCKADDR_IN &Client::getRemoteAddr(){
		return data->remote;
	}
	char *Client::getBuffer(){
		return inbuf;
	}
	int Client::getBufferSize(){
		return inbufSize;
	}
}
};