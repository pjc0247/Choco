#pragma once

#include "errno.h"

#undef _ERRNO_BASE
#define _ERRNO_BASE _ERRNO_BASE_IO

namespace Choco{
	namespace IO{
		
		_ERRNO( SocketError, 1 );
	};
};