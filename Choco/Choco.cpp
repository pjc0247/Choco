// Choco.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "io_tcp_socket.h"
#include "app_server.h"
#include "session_client.h"

#include "conig.h"

#include <thread>
#include <mutex>

#pragma comment (lib,"ws2_32")
#pragma comment (lib,"mswsock")

class App : public Choco::App::Interface{
public:
	virtual int onLoad(){
		return 0;
	}
	virtual void onQuit(){
	}

	virtual int onConnected(Choco::Session::Client *session){
		return 0;
	}
	virtual void onDisconnected(Choco::Session::Client *session){
	}
	virtual void onReceived(
		Choco::Session::Client *session,
		char *data,int len){
	}
};

#include "common_errno.h"
#include <concurrent_queue.h>

int _tmain(int argc, _TCHAR* argv[]){
	Choco::configure("worker_count", "10");
	Choco::configure("server_host", "localhost");
	Choco::configure("server_port", "9916");
	Choco::configure("session_pool_size", "1");

	Choco::configureDefault();
	Choco::IO::TCPSocket socket;

	Choco::App::Server server;

	printf("%d\n", server.initialize());

	int ret = server.run();
	printf("%d\n", ret);

	while(true)
		Sleep(100);

	return 0;
}

            