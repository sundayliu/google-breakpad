#ifndef __BREAKPAD_CLIENT_H__
#define __BREAKPAD_CLIENT_H__

#include "common_macros.h"
#include <stdlib.h>

NS_GOOGLE_BREAKPAD_BEGIN

typedef bool (*PFN_UPLOAD_NSEXCEPTION_CALLBACK)(const char* reason, const char* stack);
typedef bool (*PFN_UPLOAD_BSDEXCEPTION_CALLBACK)(const char* dumpPath);

void install_breakpad(size_t size_limit, PFN_UPLOAD_NSEXCEPTION_CALLBACK cbNSException, PFN_UPLOAD_BSDEXCEPTION_CALLBACK cbBsdException);

void uninstall_breakpad();

NS_GOOGLE_BREAKPAD_END


#endif