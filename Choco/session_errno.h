#pragma once

#include "errno.h"

#undef _ERRNO_BASE
#define _ERRNO_BASE _ERRNO_BASE_SESSION

namespace Choco{
	namespace Session{
		
		_ERRNO( SocketError, 1 );
	};
};