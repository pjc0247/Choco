#include "errno.h"

namespace Choco{
	/* MSVC specific thread_local variable */
	static _declspec(thread) int errno = 0;

	void setErrno(int _errno){
		errno = _errno;
	}
	int getErrno(){
		return errno;
	}
};