#pragma once

#include "errno.h"

#undef _ERRNO_BASE
#define _ERRNO_BASE _ERRNO_BASE_COMMON

namespace Choco{
	
	_ERRNO( UnknownError, 0 );
	_ERRNO( AllocationError, 1 );
};