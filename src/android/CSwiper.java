package com.bbpos.plugin.cswiper;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.bbpos.cswiper.CSwiperController;
import com.bbpos.cswiper.CSwiperController.CSwiperStateChangedListener;
import com.bbpos.cswiper.CSwiperController.DecodeResult;
import com.bbpos.cswiper.CSwiperController.PINKey;
import com.bbpos.cswiper.CSwiperController.PINKeyLocation;

public class CSwiper extends CordovaPlugin implements CSwiperStateChangedListener {

private CSwiperController cswiperController;
	
	private String jsEquvalent = "window.plugins.cswiper";
	
	@Override
	public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        assert this.cordova == null;
        this.cordova = cordova;
        this.webView = webView;
        
        initializeCSwiper();
    }

	private void initializeCSwiper() {
		// TODO Auto-generated method stub
		cswiperController = CSwiperController.createInstance(this.webView.getContext(), this);
		cswiperController.setDetectDeviceChange(true);
	}
	
	@Override
	public void onDestroy()
	{
		super.onDestroy();
		cswiperController.deleteCSwiper();		
	}
	
	private HashMap<Integer, String[]> parseAPDU(String jsonMap) {
		try {
		    JSONObject jsonObject = new JSONObject(jsonMap);
		    Iterator keys = jsonObject.keys();
		    HashMap<Integer, String[]> map = new HashMap<Integer, String[]>();
		    while (keys.hasNext()) {
		        String key = (String) keys.next();
		        String[] value = parseStringArray(jsonObject.getJSONArray(key));
		        map.put(Integer.parseInt(key), value);
		    }
		    Log.d(jsEquvalent, map.toString());
		    return map;
		} catch (JSONException e) {
		    e.printStackTrace();
		    return null;
		}
	}
	
	private String[] parseStringArray(JSONArray jsonArr) {
		try {
	        List<String> stringList = new ArrayList<String>();
	        for (int i = 0; i < jsonArr.length(); i++) {
	        	stringList.add(jsonArr.getString(i));
	        }
	        String[] stringArr = new String[stringList.size()];	        
	        return stringList.toArray(stringArr);
		} catch (Exception e) {
			e.printStackTrace();
			return new String[0];
		}
	}
	
	@Override
	public boolean execute(final String action, JSONArray args, final CallbackContext callbackContext) throws JSONException {

		try	{
			//Methods that need parameter
			if (action.equals("sendPINKeyLocation")) {
				String input = args.getString(0);				
				Log.d(jsEquvalent, action + " location: " + input);
				PINKeyLocation location = PINKeyLocation.valueOf(input);				
				cswiperController.sendPINKeyLocation(location);
				return true;				
			} else if (action.equals("batchExchangeAPDU")) {
				String input = args.getString(0);
				Log.d(jsEquvalent, action + " hashMap: " + input);
				HashMap<Integer, String[]> hashMap = parseAPDU(input);
				cswiperController.batchExchangeAPDU(hashMap);
			} else if (action.equals("encryptData")) {
				String input = args.getString(0);
				cswiperController.encryptData(input);
			} else if (action.equals("exchangeAPDU")) {
				String input = args.getString(0);
				cswiperController.exchangeAPDU(input);
			} else if (action.equals("getEPBFromViPOS")) {
				String input1 = args.getString(0);
				String input2 = args.getString(1);
				cswiperController.getEPBFromViPOS(input1, input2);
			} else if (action.equals("setDetectDeviceChange")) {
				boolean input = args.getBoolean(0);
				cswiperController.setDetectDeviceChange(input);
			} else if (action.equals("setKey")) {
				int input = args.getInt(0);
				cswiperController.setKey(input);
			} else if (action.equals("setMasterKey")) {
				String input1 = args.getString(0);
				int input2 = args.getInt(1);
				cswiperController.setMasterKey(input1, input2);
			} else if (action.equals("cswiperController")) {
				String input1 = args.getString(0);
				String input2 = args.getString(1);
				cswiperController.startCSwiper(input1, input2);
			}					
			
			try {
				Log.d(jsEquvalent, "Excecuting " + action);
				Method method = cswiperController.getClass().getMethod(action);				
				final Object ret = method.invoke(cswiperController);
				if (ret != null && ret.toString() != "null") {
					Log.d(jsEquvalent, "return " + ret.toString());
					callbackContext.success(ret.toString());
				}
			} catch (Exception e) {
				Log.d(jsEquvalent, "No method " + action);
			}			
			return true;
			
		} catch (Exception e) {
			Log.d(jsEquvalent, "Fail to execute method " + action);
			return false;
		}
	}
			
	private void invokeJsFunc(String func) {
		String js = String.format("%s.%s();", jsEquvalent, func);
		Log.d(jsEquvalent, js);
		this.webView.sendJavascript(js);
	}
	
	private void invokeJsFunc(String func, String params) {
		String js = String.format("%s.%s(%s);", jsEquvalent, func, params);
		Log.d(jsEquvalent, js);
		this.webView.sendJavascript(js);
	}
	
	@Override
	public void onEPBDetected() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onEPBDetected");
	}

	@Override
	public void onPinEntryDetected(PINKey arg0) {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onPinEntryDetected", String.format("'%s'", arg0.name()));
	}

	@Override
	public void onWaitingForPinEntry() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onWaitingForPinEntry");
	}

	@Override
	public void onCardSwipeDetected() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onCardSwipeDetected");
	}

	@Override
	public void onDecodeCompleted(HashMap<String, String> arg0) {
		// TODO Auto-generated method stub		
		this.invokeJsFunc("onDecodeCompleted", new JSONObject(arg0).toString());
	}

	@Override
	public void onDecodeError(DecodeResult arg0) {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onDecodeError", String.format("'%s'", arg0.name()));
	}

	@Override
	public void onWaitingForCardSwipe() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onWaitingForCardSwipe");
	}

	@Override
	public void onDevicePlugged() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onDevicePlugged");
	}

	@Override
	public void onDeviceUnplugged() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onDeviceUnplugged");
	}

	@Override
	public void onError(int arg0, String arg1) {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onError", String.format("%d,'%s'", arg0, arg1));
	}

	@Override
	public void onInterrupted() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onInterrupted");
	}

	@Override
	public void onNoDeviceDetected() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onNoDeviceDetected");
	}

	@Override
	public void onTimeout() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onTimeout");
	}

	@Override
	public void onWaitingForDevice() {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onWaitingForDevice");
	}

	@Override
	public void onGetKsnCompleted(String arg0) {
		// TODO Auto-generated method stub
		this.invokeJsFunc("onGetKsnCompleted", String.format("'%s'", arg0));
	}

}
