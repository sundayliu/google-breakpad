#import "breakpad_client.h"
#import "BreakpadController.h"

NS_GOOGLE_BREAKPAD_BEGIN

extern PFN_UPLOAD_BSDEXCEPTION_CALLBACK g_callback_bsd_exception;
extern PFN_UPLOAD_NSEXCEPTION_CALLBACK g_callback_ns_exception;

void install_breakpad(size_t size_limit, PFN_UPLOAD_NSEXCEPTION_CALLBACK cbNSException, PFN_UPLOAD_BSDEXCEPTION_CALLBACK cbBsdException)
{
    g_callback_ns_exception = cbNSException;
    g_callback_bsd_exception = cbBsdException;
    [[BreakpadController sharedInstance] setUploadInterval:5*60];
    [[BreakpadController sharedInstance] setUploadingURL:@"upload.qq.com"];
    [[BreakpadController sharedInstance] start:YES];
    [[BreakpadController sharedInstance] setUploadingEnabled:YES];
    
}

void uninstall_breakpad()
{
    [[BreakpadController sharedInstance] stop];
}

NS_GOOGLE_BREAKPAD_END