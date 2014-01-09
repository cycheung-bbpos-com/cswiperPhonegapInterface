//
//  CSwiperCDVPlugin.m
//  CashRegister
//
//  Created by BBPOS on 8/1/14.
//
//

#import "CSwiperPlugin.h"
#import <Foundation/NSJSONSerialization.h>

@implementation CSwiperPlugin

@synthesize controller;
@synthesize doLogging;

NSString* const StringFromPINKey[] = {
	@"KEY_PIN",
    @"KEY_BACK",
    @"KEY_CANCEL",
	@"KEY_CLEAR",
    @"KEY_ENTER",
    @"KEY_ENTER_WITHOUT_PIN" //Added in 2.5.0
};

NSString* const StringFromDecodeResult[] = {
	@"CSwiperControllerDecodeResultSwipeFail",
    @"CSwiperControllerDecodeResultCRCError",
    @"CSwiperControllerDecodeResultCommError",
	@"CSwiperControllerDecodeResultTrack1Error", //Deprecated
    @"CSwiperControllerDecodeResultTrack2Error", //Deprecated
    @"CSwiperControllerDecodeResultTrack3Error", //Deprecated
    @"CSwiperControllerDecodeResultUnknownError"
};

- (CDVPlugin*)initWithWebView:(UIWebView *)theWebView
{
	self = (CSwiperPlugin*)[super initWithWebView:theWebView];
	if (self)
	{
		self.doLogging = true;
		[self initializeCSwiper];
	}
	return self;
}

- (void)initializeCSwiper
{
	[self Log:@"init"];
	controller = [[CSwiperController alloc] init];
	controller.delegate = self;
	controller.detectDeviceChange = YES;
}

- (void)dealloc
{
	[controller releaseAudioResource];
	//[controller release];
	//[super dealloc];
}

- (void)Log: (NSString*)message
{
	if (self.doLogging && [message length] > 0)
	{
		NSLog(@"%@", [NSString stringWithFormat:@"Plugin %@", message]);
	}
}

- (void)isDevicePresent: (CDVInvokedUrlCommand*)command
{
	[self Log: @"isDevicePresent"];
	CDVPluginResult* pluginResult = nil;
	NSString* result = [controller isDevicePresent] ? @"true" : @"false";
	pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString: result];
	
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)getAPIVersion: (CDVInvokedUrlCommand*)command
{
	[self Log: @"getApiVersion"];
	CDVPluginResult* pluginResult = nil;
	NSString* version = [controller getApiVersion];
	pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString: version];
	
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)startCSwiper: (CDVInvokedUrlCommand*)command
{
	[self Log: @"startCSwiper"];
	[controller startCSwiper];
}

- (void)getCSwiperKsn: (CDVInvokedUrlCommand*)command
{
	[self Log: @"getCSwiperKsn"];
	[controller getCSwiperKsn];
}

- (void)getFirmwareVersion: (CDVInvokedUrlCommand*)command //Need firmware version 1.2.14 or above
{
	[self Log: @"getFirmwareVersion"];
	[controller getFirmwareVersion];
}

- (void)onGetFirmwareVersionCompleted:(NSString *)firmwareVersion
{
	[self Log:[NSString stringWithFormat:@"firmwareVersion: %@", firmwareVersion]];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onGetFirmwareVersionCompleted('%@');", firmwareVersion]];
}

- (void)onGetKsnCompleted:(NSString *)ksn
{
	[self Log:[NSString stringWithFormat:@"ksn: %@", ksn]];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onGetKsnCompleted('%@');", ksn]];
}

- (void)onGetBatteryVoltageCompleted:(NSString *)batteryVoltage
{
	[self Log:[NSString stringWithFormat:@"batteryVoltage: %@", batteryVoltage]];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onGetBatteryVoltageCompleted('%@');", batteryVoltage]];
}

- (void)onWaitingForDevice
{
	[self Log:@"onWaitingForDevice"];
	[self writeJavascript:@"window.plugins.cswiper.onWaitingForDevice();"];
}

- (void)onWaitingForCardSwipe
{
	[self Log:@"onWaitingForCardSwipe"];
	[self writeJavascript:@"window.plugins.cswiper.onWaitingForCardSwipe();"];
}

- (void)onCardSwipeDetected
{
	[self Log:@"onCardSwipeDetected"];
	[self writeJavascript:@"window.plugins.cswiper.onCardSwipeDetected();"];
}

- (void)onDecodingStart
{
	[self Log:@"onDecodingStart"];
	[self writeJavascript:@"window.plugins.cswiper.onDecodingStart();"];
}

- (void)onInterrupted
{
	[self Log:@"onInterrupted"];
	[self writeJavascript:@"window.plugins.cswiper.onInterrupted();"];
}

- (void)onIllegalStateError
{
	[self Log:@"onIllegalStateError"];
	[self writeJavascript:@"window.plugins.cswiper.onIllegalStateError();"];
}

- (void)onTimeout
{
	[self Log:@"onTimeout"];
	[self writeJavascript:@"window.plugins.cswiper.onTimeout();"];
}

- (void)onDecodeError:(CSwiperControllerDecodeResult)decodeResult
{
	[self Log:@"onDecodeError"];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onDecodeError('%@');", StringFromDecodeResult[ decodeResult]]];
}

- (void)onDecodeCompleted:(NSDictionary *)decodeData
{
	//NSString* jsonDecodeData = [decodeData cdvjk_JSONString];
	NSError* error = nil;
	NSData* jsonData = [NSJSONSerialization dataWithJSONObject: decodeData options: 0 error: &error];
	NSString* jsonDecodeData = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
	[self Log:@"onDecodeCompleted"];
	[self Log:[NSString stringWithFormat:@"decodeData: %@", jsonDecodeData]];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onDecodeCompleted('%@');", jsonDecodeData]];
}

- (void)onError:(int)errorType message:(NSString *)message
{
	[self Log:@"onError"];
	[self Log:[NSString stringWithFormat:@"errorType: %d message: %@", errorType, message]];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onDecodeCompleted(%d, '%@');", errorType, message]];
}

- (void)onNoDeviceDetected
{
	[self Log:@"onNoDeviceDetected"];
	[self writeJavascript:@"window.plugins.cswiper.onNoDeviceDetected()"];
}

// Callback for startCSwiperWithPinEntry
- (void)onWaitingForPinEntry
{
	[self Log:@"onWaitingForPinEntry"];
	[self writeJavascript:@"window.plugins.cswiper.onWaitingForPinEntry()"];
}

- (void)onPinEntryDetected:(PINKey)pinKey
{
	[self Log:@"onPinEntryDetected"];
	[self Log:[NSString stringWithFormat:@"pinKey: %d", pinKey]];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onPinEntryDetected('%@')", StringFromPINKey[pinKey]]];
}

- (void)onEPBDetected
{
	[self Log:@"onEPBDetected"];
	[self writeJavascript:@"window.plugins.cswiper.onEPBDetected()"];
}

// Device Detection
- (void)onDevicePlugged
{
	[self Log:@"onDevicePlugged"];
	[self writeJavascript:@"window.plugins.cswiper.onDevicePlugged()"];
}

- (void)onDeviceUnplugged
{
	[self Log:@"onDeviceUnplugged"];
	[self writeJavascript:@"window.plugins.cswiper.onDeviceUnplugged()"];
}

- (void)onSetMasterKeyCompleted:(BOOL)isSucc
{
	[self Log:@"onSetMasterKeyCompleted"];
	[self writeJavascript:[NSString stringWithFormat:@"window.plugins.cswiper.onSetMasterKeyCompleted(%s)", (isSucc)? "true" : "false"]];
}

- (void)onEncryptDataCompleted:(NSString *)ksn
                 encWorkingKey:(NSString *)encWorkingKey
                       encData:(NSString *)encData                 //Added since v2.11.0
{
	[self Log:@"onEncryptDataCompleted"];
}

- (void)onApduResponseReceived:(NSString *)response;                //Added since v2.12.0
{
	[self Log:@"onApduResponseReceived"];
}

- (void)onBatchApduResponseReceived:(NSDictionary *)apduResponses  //Added since v2.12.0
{
	[self Log:@"onBatchApduResponseReceived"];
}

@end
