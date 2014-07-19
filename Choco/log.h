#pragma once

#include <string>

namespace Choco{
	namespace Log{
		
		enum Level{
			Info =1,
			Debug,
			Warn,
			Error
		};

		void output(const std::string &fmt, ...);
	};
};