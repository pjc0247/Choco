#pragma once

#define _ERRNO_BASE 0

#define _ERRNO_BASE_COMMON 0
#define _ERRNO_BASE_IO 1000
#define _ERRNO_BASE_DB 2000
#define _ERRNO_BASE_SERVER 3000
#define _ERRNO_BASE_SESSION 4000

#define _ERRNO(name,value) \
	const int name = value + _ERRNO_BASE;

namespace Choco{
		
	void setErrno(int errno);
	int getErrno();
};