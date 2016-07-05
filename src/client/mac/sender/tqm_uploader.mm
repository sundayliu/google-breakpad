#import "client/mac/sender/tqm_uploader.h"
#import "common_macros.h"
#import "client/apple/Framework/BreakpadDefines.h"

#import <sys/stat.h>

NS_GOOGLE_BREAKPAD_BEGIN

PFN_UPLOAD_BSDEXCEPTION_CALLBACK g_callback_bsd_exception = NULL;
PFN_UPLOAD_NSEXCEPTION_CALLBACK g_callback_ns_exception = NULL;

NS_GOOGLE_BREAKPAD_END

const int kMinidumpFileLengthLimit = 2 * 1024 * 1024;  // 2MB

@implementation BugReportUploader

//=============================================================================
- (void)report {
    NSLog(@"[BugReportUploader] report");
    
    NSMutableDictionary* parameters = [self parameters];
    NSString *minidumpDir =
    [parameters objectForKey:@kReporterMinidumpDirectoryKey];
    NSString *minidumpID = [parameters objectForKey:@kReporterMinidumpIDKey];
    
    const char* configFile = [self getConfigFile];
    if (![minidumpID length])
    {
        remove(configFile);
        return;
    }
    
    NSString *path = [minidumpDir stringByAppendingPathComponent:minidumpID];
    path = [path stringByAppendingPathExtension:@"dmp"];
    
    // check the size of the minidump and limit it to a reasonable size
    // before attempting to load into memory and upload
    const char *fileName = [path fileSystemRepresentation];
    struct stat fileStatus;
    
    BOOL success = YES;
    
    if (!stat(fileName, &fileStatus)) {
        if (fileStatus.st_size > kMinidumpFileLengthLimit) {
            fprintf(stderr, "Breakpad Uploader: minidump file too large " \
                    "to upload : %d\n", (int)fileStatus.st_size);
            success = NO;
        }
    } else {
        fprintf(stderr, "Breakpad Uploader: unable to determine minidump " \
                "file length:%d-%s\n", errno, strerror(errno));
        success = NO;
    }
    
    if (!success)
    {
        remove(fileName);
        remove(configFile);
        return;
    }
    
    if (NS_GOOGLE_BREAKPAD::g_callback_bsd_exception)
    {
        bool ret = NS_GOOGLE_BREAKPAD::g_callback_bsd_exception(fileName);
        if (ret)
        {
            remove(fileName);
            remove(configFile);
        }
    }

}


@end