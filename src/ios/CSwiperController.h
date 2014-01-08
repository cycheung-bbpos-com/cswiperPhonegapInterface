//
//  CSwiperController.h
//  CSwiperAPI
//
//  Created by AlexWong on 2012-12-19.
//  Copyright 2012 BBPOS LTD. All rights reserved.
//

#import <Foundation/Foundation.h>

//BBPOS

#pragma mark - Public Enum
typedef enum {
    CSwiperControllerStateIdle,
    CSwiperControllerStateWaitingForDevice,
    CSwiperControllerStateRecording,
    CSwiperControllerStateDecoding
} CSwiperControllerState;

typedef enum {
    CSwiperControllerDecodeResultSwipeFail,
    CSwiperControllerDecodeResultCRCError,
    CSwiperControllerDecodeResultCommError,
	CSwiperControllerDecodeResultTrack1Error, //Deprecated
    CSwiperControllerDecodeResultTrack2Error, //Deprecated
    CSwiperControllerDecodeResultTrack3Error, //Deprecated
    CSwiperControllerDecodeResultUnknownError
} CSwiperControllerDecodeResult;

typedef enum {
    KEY_PIN,
    KEY_BACK,
    KEY_CANCEL,
	KEY_CLEAR,
    KEY_ENTER,
    KEY_ENTER_WITHOUT_PIN //Added in 2.5.0
} PINKey; //For CSwiper with PIN entry function

typedef enum {
    PIN_CODE,
    ZIP_CODE,
    OTP_CODE
} PINMode; //Added in 2.5.0

typedef enum {
    LOCATION_1, LOCATION_2, LOCATION_3,
    LOCATION_4, LOCATION_5, LOCATION_6,
    LOCATION_7, LOCATION_8, LOCATION_9,
    LOCATION_C, LOCATION_0, LOCATION_E
} PINKeyLocation; //Added in 2.13.0

#define ERROR -1
#define ERROR_FAIL_TO_START -2
#define ERROR_FAIL_TO_GET_KSN -3
#define ERROR_FAIL_TO_GET_FIRMWARE_VERSION -4
#define ERROR_FAIL_TO_GET_BATTERY_VOLTAGE -5
#define ERROR_INVALID_WORKING_KEYS -6
#define ERROR_WORKING_KEYS_RECEIVE_FAILED -7
#define ERROR_WORKING_KEYS_INPUT_ERROR -8
#define ERROR_FAIL_TO_START_PIN_ENTRY -9                //Added in 2.5
#define ERROR_FAIL_TO_SET_MASTER_KEY -10                //Added in 2.6
#define ERROR_INVALID_INPUT_DATA -12                    //Added in 2.9.0
#define ERROR_FAIL_TO_START_CARD_SWIPE -13              //Added in 2.9.0
#define ERROR_FAIL_TO_RECEIVE_APDU_RESPONSE -14         //Added in 2.12.0
#define ERROR_FAIL_TO_ENCRYPT_DATA -15                  //Added in 2.11.0
#define ERROR_AUDIO_RECORDING_PERMISSION_DENIED -16     //Added in 2.16.0
//#define ERROR_INVALID_AMOUNT -11                      //Added in 2.7.0, Deprecated in v2.9.0, merge with ERROR_INVALID_INPUT_DATA

@protocol CSwiperControllerDelegate;

@interface CSwiperController : NSObject {
	NSObject <CSwiperControllerDelegate>* delegate;
    BOOL detectDeviceChange;
}

@property (nonatomic, assign) NSObject <CSwiperControllerDelegate>* delegate;
@property (nonatomic, assign) BOOL detectDeviceChange;

+ (CSwiperController *)sharedController;    //Added in 2.13.2
- (CSwiperControllerState)getCSwiperState;
- (BOOL)isDevicePresent;
- (void)stopCSwiper;
- (void)releaseAudioResource;

- (NSString *)getApiVersion;
- (NSString *)getApiBuildNumber;

- (NSDictionary *)getIntegratedApiVersion;
- (NSDictionary *)getIntegratedApiBuildNumber;

// --- startCSwiper ---
- (void)startCSwiper;
- (void)startCSwiper:(NSString *)randInHex dataInHex:(NSString *)dataInHex;    //Added in 2.16.0
- (void)startCSwiper:(int)numOfKeys
      encWorkingKeys:(NSArray *)encWorkingKeys
    kcvOfWorkingKeys:(NSArray *)kcvOfWorkingKeys;
- (void)startCSwiper:(NSString *)amount
           numOfKeys:(int)numOfKeys
           keyLength:(int)keyLength
      encWorkingKeys:(NSArray *)encWorkingKeys
    kcvOfWorkingKeys:(NSArray *)kcvOfWorkingKeys;   //Added in 2.15.0, Will not automatically enter PIN mode
- (void)startCSwiperWithRandomNumber:(NSString *)rand;                          //Added in 2.9.0

// --- startCSwiperWithPinEntry ---
- (void)startCSwiperWithPinEntry;
- (void)startCSwiperWithPinEntry:(int)numOfKeys
                       keyLength:(int)keyLength
                  encWorkingKeys:(NSArray *)encWorkingKeys
                kcvOfWorkingKeys:(NSArray *)kcvOfWorkingKeys; // Added in 2.5, For Format ID 47

/*
- (void)startCSwiperWithPinEntry:(NSString *)amount
                       numOfKeys:(int)numOfKeys
                       keyLength:(int)keyLength
                  encWorkingKeys:(NSArray *)encWorkingKeys
                kcvOfWorkingKeys:(NSArray *)kcvOfWorkingKeys; // Deprecated in 2.15.0 */

// --- PIN Entry Mode ---
- (void)startPINEntry;                                  //Added in 2.13.0
- (void)startPINEntry:(PINMode)pinMode;                 //Added in 2.15.0
- (void)sendPINKeyLocation:(PINKeyLocation)location;    //Added in 2.13.0

// --- Other ---
- (void)getCSwiperKsn;
- (void)getFirmwareVersion; //Need firmware version 1.2.14 or above
- (void)getBatteryVoltage;  //Need firmware version 1.2.14 or above

// --- Data encryption ---
- (void)setMasterKey:(NSString *)mk masterKeyLength:(int) mkLength; // Added in 2.6.0
- (void)encryptData:(NSString *)data; //Added in 2.11.0

// --- APDU ---
- (void)exchangeAPDU:(NSString *)data;                  //Added in 2.12.0
- (void)batchExchangeAPDU:(NSDictionary *)apduCommands; //Added in 2.12.0

// --- PIN Mapping ---
- (void)getPinPadMappingFromViPOS;                      //Addin in v2.16.0
- (void)getEPBFromViPOS:(NSString *)translatedPinInHex randInHex:(NSString *)randInHex;  //Addin in v2.16.0

@end

@protocol CSwiperControllerDelegate <NSObject>

- (void)onGetFirmwareVersionCompleted:(NSString *)firmwareVersion;
- (void)onGetKsnCompleted:(NSString *)ksn;
- (void)onGetBatteryVoltageCompleted:(NSString *)batteryVoltage;

- (void)onWaitingForDevice;
- (void)onWaitingForCardSwipe;
- (void)onCardSwipeDetected;
- (void)onDecodingStart;
- (void)onInterrupted;
- (void)onIllegalStateError;
- (void)onTimeout;
- (void)onDecodeError:(CSwiperControllerDecodeResult)decodeResult;
- (void)onDecodeCompleted:(NSDictionary *)decodeData;
- (void)onError:(int)errorType message:(NSString *)message;
- (void)onNoDeviceDetected;

@optional 

// Callback for PinEntry function
- (void)onWaitingForPinEntry;                
- (void)onPinEntryDetected:(PINKey)pinKey;
- (void)onEPBDetected;

// Callback of data encryption
- (void)onSetMasterKeyCompleted:(BOOL)isSucc;                       // Added in 2.6.0
- (void)onEncryptDataCompleted:(NSString *)ksn
                 encWorkingKey:(NSString *)encWorkingKey
                       encData:(NSString *)encData;                 //Added in 2.11.0

// Callback of APDU
- (void)onApduResponseReceived:(NSString *)response;                //Added in 2.12.0
- (void)onBatchApduResponseReceived:(NSDictionary *)apduResponses;  //Added in 2.12.0

// Callback of PIN Mapping
- (void)onReturnPinPadMappingFromViPOS:(NSString *)mappingInHex;    //Addin in v2.16.0
- (void)onReturnEPBFromViPOS:(NSString *)epbInHex;                  //Addin in v2.16.0

// Device Detection
- (void)onDevicePlugged;
- (void)onDeviceUnplugged;

@end
