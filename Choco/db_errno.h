#pragma once

#include "errno.h"

#undef _ERRNO_BASE
#define _ERRNO_BASE _ERRNO_BASE_DB

namespace Choco{
	namespace DB{

		_ERRNO( HostError, 1 );
		_ERRNO( ConnectionError, 2 );
		_ERRNO( ServerLostError, 3 );
		_ERRNO( SocketError, 4 );
		_ERRNO( AlreadyConnectedError, 5 );
	};
};