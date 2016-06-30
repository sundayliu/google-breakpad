#ifndef __BREAKPAD_CLIENT_H__
#define __BREAKPAD_CLIENT_H__

#include <stdlib.h>
#include <string.h>

namespace google_breakpad
{
	typedef bool (*PFN_DUMP_CALLBACK)(const char* dumpPath, void* context, bool succeeded);
	typedef bool (*PFN_FILTER_CALLBACK)(void* context);
	void install_breakpad(const char* dumpDir, size_t size_limit, PFN_DUMP_CALLBACK cbDump, PFN_FILTER_CALLBACK cbFilter);
}

#endif