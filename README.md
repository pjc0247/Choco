Choco
====

Choco는 IOCP기반의 서버 프레임워크입니다.

Example
----
```C+++
using namespace Choco;

class Server : public App::Server{
  virtual int onLoad(){
    return 0;
  }
  virtual void onQuit(){
  }
  
  virtual int onConnected(Session::Client *session){
    return 0;
  }
  virtual void onDisconnected(Session::Client *session){
  }
  virtual void onReceived(
    Session::Client *session,
    char *data,int len){
    
    session->write( data,len );
  }
};
```

ToDo
---
__기능 완성__
__ORM 붙이기__
https://github.com/pjc0247/SQB
