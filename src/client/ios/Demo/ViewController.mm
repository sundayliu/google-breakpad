//
//  ViewController.m
//  Demo
//
//  Created by gavingu on 16/7/4.
//  Copyright © 2016年 tencent. All rights reserved.
//

#import "ViewController.h"
#import <sys/sysctl.h>

@interface ViewController ()

@end

@implementation ViewController

- (NSAttributedString *)buildWaringString {
    NSMutableAttributedString *attributedString = [[NSMutableAttributedString alloc] initWithString:@"注意: Xcode处于调试模式\n请断开调试模式再次启动App后触发崩溃测试。"];
    return [[NSAttributedString alloc] initWithAttributedString:attributedString];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    bool debugMode = debugger_should_exit();
    if (debugMode) {
        [self.labelWarning setAttributedText:[self buildWaringString]];
        [self.labelWarning setNumberOfLines:4];
    }
    
    [self.imageWarning setImage:[UIImage imageNamed:@"ImageWarning"]];
    
    [self.labelWarning setHidden:!debugMode];
    [self.imageWarning setHidden:!debugMode];
    
    [self.btnCrashObj setEnabled:!debugMode];
    [self.btnCrashUnix setEnabled:!debugMode];
    
    [self.btnCrashObj addTarget:self action:@selector(onNSExceptionButtonClick) forControlEvents:UIControlEventTouchUpInside];
    [self.btnCrashUnix addTarget:self action:@selector(onSignalButtonClick) forControlEvents:UIControlEventTouchUpInside];

}

- (void)onNSExceptionButtonClick {
    [self testNSException];
}

- (void)onSignalButtonClick {
    [self testSignalException];
}

- (void)testNSException {
    NSLog(@"it will throw an NSException ");
    NSArray * array = @[];
    NSLog(@"the element is %@", array[1]);
}

- (void)testSignalException {
    volatile int* a = reinterpret_cast<volatile int*>(0);
    *a = 1;
    //NSLog(@"test signal exception");
    //NSString * null = nil;
    //NSLog(@"print the nil string %s", [null UTF8String]);
    //NSLog(@"print the nil string to c string: %s", std::string([null UTF8String]).c_str());
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

static bool debugger_should_exit (void) {
#if !TARGET_OS_IPHONE
    return false;
#endif
    
    struct kinfo_proc info;
    size_t info_size = sizeof(info);
    int name[4];
    
    name[0] = CTL_KERN;
    name[1] = KERN_PROC;
    name[2] = KERN_PROC_PID;
    name[3] = getpid();
    
    if (sysctl(name, 4, &info, &info_size, NULL, 0) == -1) {
        NSLog(@"sysctl() failed: %s", strerror(errno));
        return false;
    }
    
    if ((info.kp_proc.p_flag & P_TRACED) != 0)
        return true;
    
    return false;
}

@end
