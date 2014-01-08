//
//  CSwiperPlugin.h
//  CSwiperPhonegapInterface
//
//  Created by BBPOS on 8/1/14.
//
//

#import <Cordova/CDV.h>
#import "CSwiperController.h"

@interface CSwiperPlugin : CDVPlugin<CSwiperControllerDelegate>
{
	CSwiperController *controller;
	BOOL doLogging;
}

@property (nonatomic, retain) CSwiperController *controller;
@property (nonatomic, assign) BOOL doLogging;

- (void)isDevicePresent: (CDVInvokedUrlCommand*)command;
- (void)getAPIVersion: (CDVInvokedUrlCommand*)command;
- (void)startCSwiper: (CDVInvokedUrlCommand*)command;
- (void)getCSwiperKsn: (CDVInvokedUrlCommand*)command;
- (void)getFirmwareVersion: (CDVInvokedUrlCommand*)command; //Need firmware version 1.2.14 or above

@end