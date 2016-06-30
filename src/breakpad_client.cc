#include "breakpad_client.h"
#include "client/linux/handler/exception_handler.h"
#include "client/linux/handler/minidump_descriptor.h"

#include <stdio.h>
#include <stdlib.h>

namespace google_breakpad
{
	PFN_DUMP_CALLBACK g_minidump_callback = NULL;
	static bool DumpCallback(const MinidumpDescriptor& descriptor, void* context, bool succeeded)
	{
		if (g_minidump_callback)
		{
			return g_minidump_callback(descriptor.path(), context, succeeded);
		}

		return succeeded;
	}

	void install_breakpad(const char* dumpDir, size_t size_limit, PFN_DUMP_CALLBACK cbDump, PFN_FILTER_CALLBACK cbFilter)
	{
		static bool run_once = false;
		if (run_once)
		{
			return;
		}
		run_once = true;
		g_minidump_callback = cbDump;
		static MinidumpDescriptor descriptor(dumpDir);
		descriptor.set_size_limit(size_limit);
		static ExceptionHandler eh(descriptor, cbFilter, DumpCallback, NULL, true, -1);
	}
}