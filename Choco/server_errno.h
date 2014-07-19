#pragma once

#include "errno.h"

#undef _ERRNO_BASE
#define _ERRNO_BASE _ERRNO_BASE_SERVER

namespace Choco{
	namespace App{

		_ERRNO( WsaStartupError, 1 );
		_ERRNO( HostError, 2 );
		_ERRNO( SocketError, 3 );
		_ERRNO( BindError, 4 );
		_ERRNO( ListenError, 5 );
		_ERRNO( IocpError, 6 );
	};
};