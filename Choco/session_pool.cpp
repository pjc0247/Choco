#include "session_pool.h"
#include "common_errno.h"

#include "conig.h"

using namespace std;

namespace Choco{
namespace Session{

	Pool::Pool(){
	}
	Pool::~Pool(){
	}

	int Pool::initialize(){
		int poolSize;

		poolSize = 
			getConfigAsInteger("session_pool_size");
		size = 0;

		for(int i=0;i<poolSize;i++){
			int ret;
			Client *session =
				new(nothrow) Client();

			if( session == nullptr )
				return AllocationError;

			ret = session->initialize();

			if( ret ){
				session->quit();
				delete session;
				return ret;
			}

			queue.push( session );
			size++;
		}

		return 0;
	}
	void Pool::quit(){
		while( !queue.empty() ){
			Client *session;

			if( queue.try_pop(session) ){
				session->quit();
				delete session;
			}
		}
	}

	int Pool::getSize(){
		return size;
	}

	void Pool::iterate(function<void(Client*)> cb){
		for(auto it=queue.unsafe_begin();it!=queue.unsafe_end();++it){
			cb( *it );
		}
	}
};
};