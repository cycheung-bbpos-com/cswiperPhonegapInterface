function CSwiperPlugin() {}

CSwiperPlugin.prototype = {
	
	 ERROR : -1,  
  	 ERROR_FAIL_TO_START : -2,  
  	 ERROR_FAIL_TO_GET_KSN : -3,
  	 ERROR_FAIL_TO_GET_FIRMWARE_VERSION : -4,
  	 ERROR_FAIL_TO_GET_BATTERY_VOLTAGE : -5,
  	 ERROR_INVALID_INPUT_DATA : -6,
  	 ERROR_INVALID_WORKING_KEYS : -6,
  	 ERROR_WORKING_KEYS_RECEIVE_FAILED : -7,
  	 ERROR_FAIL_TO_START_CARD_SWIPE : -7,
  	 ERROR_FAIL_TO_START_PIN_ENTRY : -8,
  	 ERROR_FAIL_TO_SET_MASTER_KEY : -9,
  	 ERROR_FAIL_TO_RECEIVE_APDU_RESPONSE : -10,
  	 ERROR_FAIL_TO_ENCRYPT_DATA : -11,
	
	DecodeResult : {
		DECODE_COMM_ERROR : "DECODE_COMM_ERROR",
		DECODE_CRC_ERROR : "DECODE_CRC_ERROR",
		DECODE_SUCCESS : "DECODE_SUCCESS",
		DECODE_SWIPE_FAIL : "DECODE_SWIPE_FAIL",
		DECODE_TRACK1_ERROR : "DECODE_TRACK1_ERROR",
		DECODE_TRACK2_ERROR : "DECODE_TRACK2_ERROR",
		DECODE_TRACK3_ERROR : "DECODE_TRACK3_ERROR",
		DECODE_UNKNOWN_ERROR : "DECODE_UNKNOWN_ERROR",		
	},	
	PinKey : {
		KEY_PIN : "KEY_PIN",
		KEY_BACK : "KEY_BACK",
		KEY_CANCEL : "KEY_CANCEL",
		KEY_CLEAR : "KEY_CLEAR",
		KEY_ENTER : "KEY_ENTER",
		KEY_ENTER_WITHOUT_PIN : "KEY_ENTER_WITHOUT_PIN",
		KEY_UNKNOWN : "KEY_UNKNOWN",
	},
	
	_EPBDetected : {},
	_PinEntryDetected : {},
	_WaitingForPinEntry : {},
	_CardSwipeDetected : {},
	_DecodeCompleted : {},
	_DecodeError : {},
	_WaitingForCardSwipe : {},
	_DevicePlugged : {},
	_DeviceUnplugged : {},
	_Error : {},
	_Interrupted : {},
	_NoDeviceDetected : {},
	_Timeout : {},
	_WaitingForDevice : {},
	_GetKsnCompleted : {},
	_GetFirmwareVersionCompleted : {},
	_IllegalStateError : {},
	
	throwAllHandlers : function()
	{
		_EPBDetected = {};
		_PinEntryDetected = {};
		_WaitingForPinEntry = {};
		_CardSwipeDetected = {};
		_DecodeCompleted = {};
		_DecodeError = {};
		_WaitingForCardSwipe = {};
		_DevicePlugged = {};
		_DeviceUnplugged = {};
		_Error = {};
		_Interrupted = {};
		_NoDeviceDetected = {};
		_Timeout = {};
		_WaitingForDevice = {};
		_GetKsnCompleted = {};
		_GetFirmwareVersionCompleted = {};
		_IllegalStateError = {};
	},
	
	getCSwiperKsn : function()
	{
		cordova.exec(null, null, 'CSwiper', 'getCSwiperKsn', []);
	},
	startCSwiper : function()
	{
		cordova.exec(null, null, 'CSwiper', 'startCSwiper', []);
	},
	isDevicePresent : function(success, fail)
	{
		cordova.exec(success, fail, 'CSwiper', 'isDevicePresent', []);
	},
	getAPIVersion : function(success, fail)
	{
		cordova.exec(success, fail, 'CSwiper', 'getAPIVersion', []);
	},
	getFirmwareVersion : function(success, fail)
	{
		cordova.exec(success, fail, 'CSwiper', 'getFirmwareVersion', []);
	},
	
	// 14 event handlers
	setOnEPBDetectedHandler : function(cb)
	{
		_EPBDetected = cb;
	},
	onEPBDetected : function()
	{
		_EPBDetected();
	},
	setOnPinEntryDetectedHandler : function(cb)
	{
		_PinEntryDetected = cb;
	},
	onPinEntryDetected : function(pinKey)
	{
		_PinEntryDetected(pinKey);
	},
	setOnWaitingForPinEntryHandler : function(cb)
	{
		_WaitingForPinEntry = cb;
	},
	onWaitingForPinEntry : function()
	{
		_WaitingForPinEntry();
	},
	setOnCardSwipeDetectedHandler : function(cb)
	{
		_CardSwipeDetected = cb;
	},
	onCardSwipeDetected : function()
	{
		_CardSwipeDetected();
	},
	setOnDecodeCompletedHandler : function(cb)
	{
		_DecodeCompleted = cb;
	},
	onDecodeCompleted : function(decodeData)
	{
		_DecodeCompleted(decodeData);
	},
	setOnDecodeErrorHandler : function(cb)
	{
		_DecodeError = cb;
	},
	onDecodeError : function(decodeResult)
	{
		_DecodeError(decodeResult);
	},
	setOnWaitingForCardSwipeHandler : function(cb)
	{
		_WaitingForCardSwipe = cb;
	},
	onWaitingForCardSwipe : function()
	{
		_WaitingForCardSwipe();
	},
	setOnDevicePluggedHandler : function(cb)
	{
		_DevicePlugged = cb;
	},
	onDevicePlugged : function()
	{
		_DevicePlugged();
	},
	setOnDeviceUnpluggedHandler : function(cb)
    {
        _DeviceUnplugged = cb;
    },
    onDeviceUnplugged : function()
    {
        _DeviceUnplugged();
    },	
	setOnErrorHandler : function(cb)
	{
		_Error = cb;
	},
	onError : function(errorType, message)
	{
		_Error(errorType, message);
	},
	setOnInterruptedHandler : function(cb)
	{
		_Interrupted = cb;
	},
	onInterrupted : function()
	{
		_Interrupted();
	},
	setOnNoDeviceDetectedHandler : function(cb)
	{
		_NoDeviceDetected = cb;
	},
	onNoDeviceDetected : function()
	{
		_NoDeviceDetected();
	},
	setOnTimeoutHandler : function(cb)
	{
		_Timeout = cb;
	},
	onTimeout : function()
	{
		_Timeout();
	},
	setOnWaitingForDeviceHandler : function(cb)
	{
		_WaitingForDevice = cb;
	},
	onWaitingForDevice : function()
	{
		_WaitingForDevice();
	},
	setOnGetKsnCompletedHandler : function(cb)
	{
		_GetKsnCompleted = cb;
	},
	onGetKsnCompleted : function(ksn)
	{
		_GetKsnCompleted(ksn);
	},
	setOnGetFirmwareVersionCompletedHandler : function(cb)
	{
		_GetFirmwareVersionCompleted = cb;
	},
	onGetFirmwareVersionCompleted : function(fwVersion)
	{
		_GetFirmwareVersionCompleted(fwVersion);
	},
	setOnIllegalStateErrorHandler : function(cb)
	{
		_IllegalStateError = cb;
	},
	onIllegalStateError : function()
	{
		_IllegalStateError();
	},
};

if (!window.plugins) {
    window.plugins = {};
}
if (!window.plugins.cswiper) {
    window.plugins.cswiper = new CSwiperPlugin();
}
