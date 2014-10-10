package com.example.helloanychat;

import com.bairuitech.anychat.AnyChatBaseEvent;
import com.bairuitech.anychat.AnyChatCoreSDK;
import com.bairuitech.anychat.AnyChatDefine;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;

public class VideoActivity extends Activity implements AnyChatBaseEvent {

	int userID;
	boolean bOnPaused = false;

	private SurfaceView otherView;
	private SurfaceView myView;
	private ImageView mBtnSitchvideo;

	public AnyChatCoreSDK anychatSDK;

	private boolean bSelfVideoOpened = false; // ������Ƶ�Ƿ��Ѵ�
	private boolean bOtherVideoOpened = false; // �Է���Ƶ�Ƿ��Ѵ�

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Intent intent = getIntent();
		userID = Integer.parseInt(intent.getStringExtra("UserID"));

		InitSDK();
		InitLayout();
	}

	private void InitSDK() {
		anychatSDK = new AnyChatCoreSDK();
		anychatSDK.SetBaseEvent(this);
		anychatSDK.mSensorHelper.InitSensor(this);
		AnyChatCoreSDK.mCameraHelper.SetContext(this);
	}

	private void InitLayout() {
		this.setContentView(R.layout.video_frame);
		this.setTitle("��" + anychatSDK.GetUserName(userID));
		myView = (SurfaceView) findViewById(R.id.surface_local);
		otherView = (SurfaceView) findViewById(R.id.surface_remote);
		mBtnSitchvideo = (ImageView) findViewById(R.id.BtnSitchvideo);
		myView.setZOrderOnTop(true);
		mBtnSitchvideo.setOnClickListener(new OnClickListener() {
		
			@Override
			public void onClick(View v) {
				if (v == mBtnSitchvideo) {

					// ����ǲ���Java��Ƶ�ɼ�������Java���������ͷ�л�
					if (AnyChatCoreSDK
							.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_CAPDRIVER) == AnyChatDefine.VIDEOCAP_DRIVER_JAVA) {
						AnyChatCoreSDK.mCameraHelper.SwitchCamera();
						return;
					}

					String strVideoCaptures[] = anychatSDK.EnumVideoCapture();
					String temp = anychatSDK.GetCurVideoCapture();
					for (int i = 0; i < strVideoCaptures.length; i++) {
						if (!temp.equals(strVideoCaptures[i])) {
							anychatSDK.UserCameraControl(-1, 0);
							bSelfVideoOpened = false;
							anychatSDK.SelectVideoCapture(strVideoCaptures[i]);
							anychatSDK.UserCameraControl(-1, 1);
							break;
						}
					}
				}
			}
		});

		// ����ǲ���Java��Ƶ�ɼ�������Ҫ����Surface��CallBack
		if (AnyChatCoreSDK
				.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_CAPDRIVER) == AnyChatDefine.VIDEOCAP_DRIVER_JAVA) {
			myView.getHolder().addCallback(AnyChatCoreSDK.mCameraHelper);
		}

		// ����ǲ���Java��Ƶ��ʾ������Ҫ����Surface��CallBack
		if (AnyChatCoreSDK
				.GetSDKOptionInt(AnyChatDefine.BRAC_SO_VIDEOSHOW_DRIVERCTRL) == AnyChatDefine.VIDEOSHOW_DRIVER_JAVA) {
			int index = anychatSDK.mVideoHelper
					.bindVideo(otherView.getHolder());
			anychatSDK.mVideoHelper.SetVideoUser(index, userID);
		}

		SurfaceHolder holder = otherView.getHolder();
		holder.setKeepScreenOn(true);

		anychatSDK.UserCameraControl(userID, 1);
		anychatSDK.UserSpeakControl(userID, 1);

		// �ж��Ƿ���ʾ��������ͷ�л�ͼ��
		if (AnyChatCoreSDK
				.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_CAPDRIVER) == AnyChatDefine.VIDEOCAP_DRIVER_JAVA) {
			if (AnyChatCoreSDK.mCameraHelper.GetCameraNumber() > 1) {
				// Ĭ�ϴ�ǰ������ͷ
				AnyChatCoreSDK.mCameraHelper
						.SelectVideoCapture(AnyChatCoreSDK.mCameraHelper.CAMERA_FACING_FRONT);
			}
		} else {
			String[] strVideoCaptures = anychatSDK.EnumVideoCapture();
			if (strVideoCaptures != null && strVideoCaptures.length > 1) {
				// Ĭ�ϴ�ǰ������ͷ
				for (int i = 0; i < strVideoCaptures.length; i++) {
					String strDevices = strVideoCaptures[i];
					if (strDevices.indexOf("Front") >= 0) {
						anychatSDK.SelectVideoCapture(strDevices);
						break;
					}
				}
			}
		}

		// ������Ļ����ı䱾��surfaceview�Ŀ�߱�
		if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
			adjustLocalVideo(true);
		} else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
			adjustLocalVideo(false);
		}
		
		anychatSDK.UserCameraControl(-1, 1);//-1��ʾ�Ա�����Ƶ���п��ƣ��򿪱�����Ƶ
		anychatSDK.UserSpeakControl(-1, 1);//-1��ʾ�Ա�����Ƶ���п��ƣ��򿪱�����Ƶ
		
	}

	private void refreshAV() {
		anychatSDK.UserCameraControl(userID, 1);
		anychatSDK.UserSpeakControl(userID, 1);
		anychatSDK.UserCameraControl(-1, 1);
		anychatSDK.UserSpeakControl(-1, 1);
		bOtherVideoOpened = false;
		bSelfVideoOpened = false;
	}

	protected void onRestart() {
		super.onRestart();
		// ����ǲ���Java��Ƶ��ʾ������Ҫ����Surface��CallBack
		if (AnyChatCoreSDK
				.GetSDKOptionInt(AnyChatDefine.BRAC_SO_VIDEOSHOW_DRIVERCTRL) == AnyChatDefine.VIDEOSHOW_DRIVER_JAVA) {
			int index = anychatSDK.mVideoHelper
					.bindVideo(otherView.getHolder());
			anychatSDK.mVideoHelper.SetVideoUser(index, userID);
		}

		refreshAV();
		bOnPaused = false;
	}

	protected void onResume() {
		super.onResume();
	}

	protected void onPause() {
		super.onPause();
		bOnPaused = true;
		anychatSDK.UserCameraControl(userID, 0);
		anychatSDK.UserSpeakControl(userID, 0);
		anychatSDK.UserCameraControl(-1, 0);
		anychatSDK.UserSpeakControl(-1, 0);
	}

	protected void onDestroy() {
		super.onDestroy();

		anychatSDK.mSensorHelper.DestroySensor();
		finish();
	}

	public void adjustLocalVideo(boolean bLandScape) {
		float width;
		float height = 0;
		DisplayMetrics dMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dMetrics);
		width = (float) dMetrics.widthPixels / 4;
		LinearLayout layoutLocal = (LinearLayout) this
				.findViewById(R.id.frame_local_area);
		FrameLayout.LayoutParams layoutParams = (android.widget.FrameLayout.LayoutParams) layoutLocal
				.getLayoutParams();
		if (bLandScape) {

			if (AnyChatCoreSDK
					.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_WIDTHCTRL) != 0)
				height = width
						* AnyChatCoreSDK
								.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_HEIGHTCTRL)
						/ AnyChatCoreSDK
								.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_WIDTHCTRL)
						+ 5;
			else
				height = (float) 3 / 4 * width + 5;
		} else {

			if (AnyChatCoreSDK
					.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_HEIGHTCTRL) != 0)
				height = width
						* AnyChatCoreSDK
								.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_WIDTHCTRL)
						/ AnyChatCoreSDK
								.GetSDKOptionInt(AnyChatDefine.BRAC_SO_LOCALVIDEO_HEIGHTCTRL)
						+ 5;
			else
				height = (float) 4 / 3 * width + 5;
		}
		layoutParams.width = (int) width;
		layoutParams.height = (int) height;
		layoutLocal.setLayoutParams(layoutParams);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			adjustLocalVideo(true);
			AnyChatCoreSDK.mCameraHelper.setCameraDisplayOrientation();
		} else {
			adjustLocalVideo(false);
			AnyChatCoreSDK.mCameraHelper.setCameraDisplayOrientation();
		}

	}

	@Override
	public void OnAnyChatConnectMessage(boolean bSuccess) {

	}

	@Override
	public void OnAnyChatLoginMessage(int dwUserId, int dwErrorCode) {

	}

	@Override
	public void OnAnyChatEnterRoomMessage(int dwRoomId, int dwErrorCode) {

	}

	@Override
	public void OnAnyChatOnlineUserMessage(int dwUserNum, int dwRoomId) {

	}

	@Override
	public void OnAnyChatUserAtRoomMessage(int dwUserId, boolean bEnter) {
		if (!bEnter) {
			anychatSDK.UserCameraControl(dwUserId, 0);
			anychatSDK.UserSpeakControl(dwUserId, 0);
			bOtherVideoOpened = false;
		} else {

			int index = anychatSDK.mVideoHelper
					.bindVideo(otherView.getHolder());
			anychatSDK.mVideoHelper.SetVideoUser(index, dwUserId);

			anychatSDK.UserCameraControl(dwUserId, 1);
			anychatSDK.UserSpeakControl(dwUserId, 1);
		}
	}

	@Override
	public void OnAnyChatLinkCloseMessage(int dwErrorCode) {
		// �������ӶϿ�֮���ϲ���Ҫ�����ر��Ѿ��򿪵�����Ƶ�豸
		if (bOtherVideoOpened) {
			anychatSDK.UserCameraControl(userID, 0);
			anychatSDK.UserSpeakControl(userID, 0);
			bOtherVideoOpened = false;
		}
		if (bSelfVideoOpened) {
			anychatSDK.UserCameraControl(-1, 0);
			anychatSDK.UserSpeakControl(-1, 0);
			bSelfVideoOpened = false;
		}
	}
}
