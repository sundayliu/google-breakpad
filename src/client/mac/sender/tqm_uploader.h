#ifndef __TQM_UPLOADER_H__
#define __TQM_UPLOADER_H__

#import "client/mac/sender/uploader.h"
#import "client/ios/breakpad_client.h"

extern NS_GOOGLE_BREAKPAD::PFN_UPLOAD_NSEXCEPTION_CALLBACK g_callback_ns_exception;
extern NS_GOOGLE_BREAKPAD::PFN_UPLOAD_BSDEXCEPTION_CALLBACK g_callback_bsd_exception;

@interface BugReportUploader:BaseBugReportUploader

- (void)report;

@end


#endif