package com.bairuitech.anychatqueue;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.os.Bundle;
import android.util.Log;

import com.bairuitech.anychat.AnyChatCoreSDK;
import com.bairuitech.anychat.AnyChatDefine;
import com.bairuitech.anychatqueue.VideoActivity;
import com.bairuitech.common.BaseConst;
import com.bairuitech.common.BaseMethod;
import com.bairuitech.common.CustomApplication;
import com.bairuitech.common.ScreenInfo;
import com.bairuitech.main.FuncMenu;



import com.example.anychatqueue.R;



public class BussinessCenter{

	public  static AnyChatCoreSDK anychat;
	private static BussinessCenter mBussinessCenter;
	private MediaPlayer mMediaPlaer;
	public static ScreenInfo mScreenInfo;
	public static Activity mContext;
	
	public static ArrayList<Integer> mOnlineFriendIds;

	public static int selfUserId;
	public static boolean bBack = false;// 程序是否在后�?
	public static String selfUserName;
	
	private BussinessCenter() {
		initParams();
	}

	public static BussinessCenter getBussinessCenter() {
		if (mBussinessCenter == null)
			mBussinessCenter = new BussinessCenter();
		return mBussinessCenter;
	}

	private void initParams() {
		
		anychat = new AnyChatCoreSDK();
	
		mOnlineFriendIds = new ArrayList<Integer>();
	}

	/***
	 * 播放接收到呼叫音乐提示音
	 * @param context	上下文  
	 */
	private void playCallReceivedMusic(Context context) {
		mMediaPlaer = MediaPlayer.create(context, R.raw.call);
		mMediaPlaer.setOnCompletionListener(new OnCompletionListener() {

			@Override
			public void onCompletion(MediaPlayer mp) {
				// TODO Auto-generated method stub
				mMediaPlaer.start();
			}
		});
		mMediaPlaer.start();
	}

	/***
	 * 停止播放
	 */
	public void stopSessionMusic() {
		if (mMediaPlaer == null)
			return;
		try {
			mMediaPlaer.pause();
			mMediaPlaer.stop();
			mMediaPlaer.release();
			mMediaPlaer = null;
		} catch (Exception e) {
			Log.i("media-stop", "er");
		}
	}

	/***
	 * @param userId 用户id
	 * @param status 用户在线状标志，1是上线，0是下线； 
	 */
	

	public void realse() {
		anychat = null;
		
		mMediaPlaer = null;
		mScreenInfo = null;
		mContext = null;
		mBussinessCenter = null;
	}

	public void realseData() {
		
		mOnlineFriendIds.clear();
	}

	/***
	 * 发�?�呼叫事�?
	 * @param dwEventType	视频呼叫事件类型       
	 * @param dwUserId		目标userid       
	 * @param dwErrorCode	出错代码       
	 * @param dwFlags		功能标志        
	 * @param dwParam		自定义参数，传给对方        
	 * @param szUserStr		自定义参数，传给对方         
	 */
	public static void VideoCallControl(int dwEventType, int dwUserId, int dwErrorCode, int dwFlags, int dwParam, String szUserStr) {
		
		if(null == anychat) {System.out.println("anychat is null");}
		anychat.VideoCallControl(dwEventType, dwUserId, dwErrorCode, dwFlags, dwParam, szUserStr);
	}

	public void onVideoCallRequest(int dwUserId, int dwFlags,
			int dwParam, String szUserStr) {
		// TODO Auto-generated method stub
		playCallReceivedMusic(mContext);
		
	}

	public void onVideoCallReply(int dwUserId, int dwErrorCode,
			int dwFlags, int dwParam, String szUserStr) {
		// TODO Auto-generated method stub
		String strMessage = null;
		switch (dwErrorCode) {
		case AnyChatDefine.BRAC_ERRORCODE_SESSION_BUSY:
			strMessage = mContext.getString(R.string.str_returncode_bussiness);
			break;
		case AnyChatDefine.BRAC_ERRORCODE_SESSION_REFUSE:
			
			strMessage = mContext.getString(R.string.str_returncode_requestrefuse);
			break;
		case AnyChatDefine.BRAC_ERRORCODE_SESSION_OFFLINE:
			strMessage = mContext.getString(R.string.str_returncode_offline);
			stopSessionMusic();
			mContext.finish();
			break;
		case AnyChatDefine.BRAC_ERRORCODE_SESSION_QUIT:
			System.out.println("座席取消呼叫");
			strMessage = mContext.getString(R.string.str_returncode_requestcancel);
			BaseMethod.showToast(strMessage, mContext);
			stopSessionMusic();
			mContext.finish();
			break;
		case AnyChatDefine.BRAC_ERRORCODE_SESSION_TIMEOUT:
			strMessage = mContext.getString(R.string.str_returncode_timeout);
			break;
		case AnyChatDefine.BRAC_ERRORCODE_SESSION_DISCONNECT:
			strMessage = mContext.getString(R.string.str_returncode_disconnect);
			break;
		case AnyChatDefine.BRAC_ERRORCODE_SUCCESS:
			break;
		default:
			break;
		}
		if (strMessage != null && dwErrorCode != AnyChatDefine.BRAC_ERRORCODE_SESSION_TIMEOUT) {
			BaseMethod.showToast(strMessage, mContext);
			// 如果程序在后台，通知通话结束
			if (bBack) {
				Bundle bundle = new Bundle();
				bundle.putInt("USERID", dwUserId);
				BaseMethod.sendBroadCast(mContext,
						BaseConst.ACTION_BACK_CANCELSESSION, null);
			}
			stopSessionMusic();
		}
	}

	public void onVideoCallStart(int dwUserId, int dwFlags, int dwParam,
			String szUserStr,CustomApplication mApplication) {
		//呼叫开始事件响应
		stopSessionMusic();
		mApplication.setTargetUserId(dwUserId);
		mApplication.setRoomId(dwParam);
		//界面是在这里跳转的
		Intent intent = new Intent();
		intent.setClass(mContext, VideoActivity.class);
		mContext.startActivity(intent);
	}

	public void onVideoCallEnd(int dwUserId, int dwFlags, int dwParam,
			String szUserStr) {
	
		Intent intent = new Intent();
		intent.setClass(mContext, QueueActivity.class);
		mContext.startActivity(intent);
		mContext.finish();
	
	}

}

