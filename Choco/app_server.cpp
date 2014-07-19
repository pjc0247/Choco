#pragma once

#include "app_server.h"
#include "common_errno.h"
#include "server_errno.h"

#include "conig.h"

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>

using namespace std;

namespace Choco{
namespace App{

	struct ServerData{
		SOCKET socket;
		SOCKADDR_IN servAddr;
		HANDLE iocp;
	};

	Server::Server() :
		handler(nullptr), data(nullptr),
		running(false){
	}
	Server::~Server(){
		quit();
	}

	int Server::initialize(){
		int ret = 0;

		ret = initializeServer();
		if( ret )
			return ret;

		ret = initializeWorkers();
		if( ret )
			return ret;

		ret = sessionPool.initialize();
		if( ret )
			return ret;

		running = true;

		return 0;
	}
	void Server::quit(){
		quitServer();
		quitWorkers();
	}

	int Server::run(Interface *_handler){
		handler = _handler;

		sessionPool.iterate( [this](Session::Client *session){
			session->reset( data->socket );
		});
		
		return 0;
	}

	int Server::initializeWorkers(){
		int workerCount;

		workerCount =
			getConfigAsInteger("worker_count");

		auto bound = bind( &Server::worker, this );
		for(int i=0;i<workerCount;i++){		
			workers.push_back(
				thread(bound) );
		}

		return 0;
	}
	int Server::initializeServer(){
		int ret;
		WSADATA wsaData;

		string host;
		int port;

		host = getConfig("server_host");
		port = 
			getConfigAsInteger("server_port");
		
		/* allocate ServerData */
		data = new(nothrow) ServerData();
		if( data == nullptr )
			return AllocationError;

		/* WSA Startup */
		ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if( ret )
			return WsaStartupError;

		/* query hostinfo */
		hostent* hostInfo =
			gethostbyname( host.c_str() );
		if( hostInfo == nullptr )
			return HostError;

		/* create WSA Socket */
		data->socket = 
			WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if( data->socket == INVALID_SOCKET )
			return SocketError;

		/* bind */
		data->servAddr.sin_family = AF_INET;
		data->servAddr.sin_addr.s_addr = 
			*(unsigned long*)hostInfo->h_addr_list[0];
		data->servAddr.sin_port = htons( port );

		ret = ::bind(
			data->socket, (SOCKADDR*)&data->servAddr, sizeof(data->servAddr) );
		if( ret == SOCKET_ERROR )
			return BindError;

		/* listen */
		ret = listen( data->socket, 1024 );
		if( ret == SOCKET_ERROR )
			return ListenError;

		/* IOCP */
		int workerCount = 
			getConfigAsInteger("worker_count");
		data->iocp = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE, nullptr,
			0, workerCount);
		if( data->iocp == INVALID_HANDLE_VALUE )
			return IocpError;

		/* Associate listening socket with iocp */
		HANDLE hRet;
		hRet = CreateIoCompletionPort(
			(HANDLE)data->socket, data->iocp,
			(ULONG_PTR)0, 0);
		if( data->iocp == INVALID_HANDLE_VALUE )
			return IocpError;

		return 0;
	}
	void Server::quitWorkers(){
		running = false;

		for(auto &worker : workers)
			worker.detach();
		workers.clear();

		while( !workers.empty() ){
			workers.pop_back();
		}
	}
	void Server::quitServer(){
		if( data == nullptr )
			return;

		if( data->socket != INVALID_SOCKET ||
			data->socket != 0 ){

			closesocket( data->socket );
		}

		delete data;
	}

	int Server::processAccept(Session::Client *session){
		SOCKADDR_IN local, remote;
		int localSize, removeSize;

		GetAcceptExSockaddrs(
			session->getBuffer(), 0,
            sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
			(sockaddr **)&session->getLocalAddr(), &localSize,
			(sockaddr **)&session->getRemoteAddr(), &removeSize);

		setsockopt(
			session->getSocket(),
			SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			(char *)data->socket, sizeof(SOCKET));

		HANDLE hRet;
		hRet = CreateIoCompletionPort(
			(HANDLE)session->getSocket(),data->iocp,
			(ULONG_PTR)0,0);
		if( hRet == INVALID_HANDLE_VALUE )
			return IocpError;

		/* ready to receive */
		session->read();

		return 0;
	}
	int Server::processRead(Session::Client *session, int len){
		handler->onReceived(
			session,
			session->getBuffer(), len);
		session->read();

		return 0;
	}
	int Server::processWrite(Session::Client *session, int len){

		return 0;
	}

	void Server::worker(){
		while( true ){
			BOOL ret;
			DWORD tranferred; /* bytes transferred */
			DWORD key; /* IOCP key */
			Session::OvData *ov;

			ret = GetQueuedCompletionStatus(
				data->iocp,
				&tranferred,
				(PULONG_PTR)&key, (OVERLAPPED**)&ov,
				INFINITE);

			/* something went wrong
			   maybe connection was lost */
			if( !ret ){
				/* reset for next connection
				   and return to pool */
				if( ov != nullptr ){
					ov->session->reset(
						data->socket );
				}

				continue;
			}
	
			switch( ov->action ){
			case Session::Accept:
				processAccept( ov->session );
				break;
			case Session::Read:
				processRead( ov->session, tranferred );
				break;
			case Session::Write:
				processWrite( ov->session, tranferred );
				break;
			}
		}
	}
};
};