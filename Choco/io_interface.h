#pragma once

namespace Choco{
	namespace IO{

		class Interface{
		public:
			virtual int initialize() =0;
			virtual void quit() =0;

			virtual int write(char *data,int len, int flags) =0;
			virtual int read(char *data,int len, int flags) =0;
		};
	};
};