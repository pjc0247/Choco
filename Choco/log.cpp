#include "log.h"

#include <cstdarg>
#include <Windows.h>

using namespace std;

namespace Choco{
namespace Log{

	void output(const string &fmt, ...){
		char buf[2048];
		va_list va;

		va_start( va, fmt );
			vsprintf( buf, fmt.c_str(), va );
		va_end( va );

		SYSTEMTIME time;
		GetLocalTime( &time );

		printf("[%2d/%2d][%2d:%2d:%2d] %s\n",
			time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond,
			buf);
	}
};
};