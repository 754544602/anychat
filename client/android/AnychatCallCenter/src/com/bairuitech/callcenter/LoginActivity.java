package com.bairuitech.callcenter;

import com.bairuitech.anychat.AnyChatBaseEvent;
import com.bairuitech.anychat.AnyChatCoreSDK;
import com.bairuitech.bussinesscenter.BussinessCenter;
import com.bairuitech.callcenter.R;

import com.bairuitech.util.BaseMethod;
import com.bairuitech.util.ConfigEntity;
import com.bairuitech.util.ConfigHelper;
import com.bairuitech.util.DialogFactory;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;

public class LoginActivity extends Activity implements AnyChatBaseEvent,
		 OnClickListener {
	private Button configBtn;
	private Button loginBtn;
	private CheckBox mCheckRember;
	private ConfigEntity configEntity;
	private EditText mEditAccount;
	private ProgressDialog mProgressLogin;
	private Dialog dialog;
	private AnyChatCoreSDK anychat;
	private int WIDTH;
	private int HEIGHT;
	private boolean bReased = false;

	String strSelfName = "";

	private static final int DIALOGID_CONFIG = 7;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		initSdk();
		intParams();
		setDisPlayMetrics();
		initView();
		initLoginProgress();
		Log.i("LoginActivity", "onCreate");

	}

	protected void intParams() {
		configEntity = ConfigHelper.getConfigHelper().LoadConfig(this);
		BussinessCenter.getBussinessCenter();
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		Log.i("loginactivyt_onrestart", "onRestart");
		this.finish();
		super.onRestart();
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub

		return super.onKeyDown(keyCode, event);
	}

	private void initSdk() {
		if (anychat == null) {
			anychat = new AnyChatCoreSDK();
			anychat.InitSDK(android.os.Build.VERSION.SDK_INT, 0);
			ConfigHelper.getConfigHelper().ApplyVideoConfig(this);
			bReased = true;
		}
		anychat.SetBaseEvent(this);
	}

	private void initView() {
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setSoftInputMode(
				WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		this.setContentView(R.layout.login_layout);
		mEditAccount = (EditText) findViewById(R.id.edit_account);
		mCheckRember = (CheckBox) findViewById(R.id.check_issavepass);
		mCheckRember.setTextColor(Color.BLACK);
		loginBtn = (Button) findViewById(R.id.btn_login);
		loginBtn.setOnClickListener(this);
		configBtn = (Button) findViewById(R.id.btn_setting);
		configBtn.setOnClickListener(this);
		if (configEntity.IsSaveNameAndPw) {
			mCheckRember.setChecked(true);
			if (configEntity.name != null)
				mEditAccount.setText(configEntity.name);
		} else
			mCheckRember.setChecked(false);

	}


	private void initLoginProgress() {
		mProgressLogin = new ProgressDialog(this);
		mProgressLogin.setOnDismissListener(new OnDismissListener() {

			@Override
			public void onDismiss(DialogInterface dialog) {
				// TODO Auto-generated method stub
				loginBtn.setClickable(true);
			}
		});
		mProgressLogin.setMessage(this.getString(R.string.login_progress));
	}

	private void Login() {
		strSelfName = mEditAccount.getEditableText().toString();
		if (mCheckRember.isChecked()) {
			configEntity.IsSaveNameAndPw = true;
			configEntity.name = strSelfName;

		} else {
			configEntity.IsSaveNameAndPw = false;
		}
		ConfigHelper.getConfigHelper().SaveConfig(this, configEntity);
		if (mEditAccount.getText().length() == 0) {
			BaseMethod.showToast(
					this.getString(R.string.str_account_input_hint), this);
			return;
		}
//		this.anychat.Logout();
		this.anychat.Connect(configEntity.ip, configEntity.port);
		this.anychat.Login(strSelfName, "123");
		loginBtn.setClickable(false);
		mProgressLogin.show();
	}

	private void setDisPlayMetrics() {
		DisplayMetrics dMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dMetrics);
		WIDTH = dMetrics.widthPixels;
		HEIGHT = dMetrics.heightPixels;
		Log.i("", "width:" + WIDTH);
		Log.i("", "height:" + HEIGHT);
	}

	protected void onDestroy() {
		Log.i("loginactivity", "onDestroy");
		super.onDestroy();
		if (bReased) {
			anychat.Logout();
			anychat.Release();
			android.os.Process.killProcess(android.os.Process.myPid());
		}
		BussinessCenter.getBussinessCenter().realseData();
	}

	protected void onResume() {
		super.onResume();
	}

	@Override
	public void OnAnyChatConnectMessage(boolean bSuccess) {
		if (!bSuccess) {
			BaseMethod
					.showToast(getString(R.string.server_connect_error), this);
			mProgressLogin.dismiss();
		} else {
		}
	}

	@Override
	public void onAttachedToWindow() {
		// TODO Auto-generated method stub

	}

	@Override
	public void OnAnyChatEnterRoomMessage(int dwRoomId, int dwErrorCode) {
		// TODO Auto-generated method stub

	}

	@Override
	public void OnAnyChatLinkCloseMessage(int dwErrorCode) {
		// Toast.makeText(this, "���ӹرգ�error��" + dwErrorCode,
		// Toast.LENGTH_SHORT)
		// .show();
	}

	@Override
	public void OnAnyChatLoginMessage(int dwUserId, int dwErrorCode) {
		if (dwErrorCode == 0) {
			BussinessCenter.selfUserId = dwUserId;
			BussinessCenter.selfUserName=mEditAccount.getText()
					.toString();
			Intent intent=new Intent();
			intent.setClass(this, HallActivity.class);
			this.startActivity(intent);
		} else if (dwErrorCode == 200) {
			BaseMethod.showToast(
					getString(R.string.str_lggin_failed), this);
		}
		mProgressLogin.dismiss();
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		Log.i("logincativity-onpuase", "onpuase");
		super.onPause();
	}

	@Override
	public void OnAnyChatOnlineUserMessage(int dwUserNum, int dwRoomId) {
		// TODO Auto-generated method stub

	}

	@Override
	public void OnAnyChatUserAtRoomMessage(int dwUserId, boolean bEnter) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.btn_login:
			Login();
			break;
		case R.id.btn_setting:
			dialog = DialogFactory.getDialog(DIALOGID_CONFIG, configEntity,
					this);
			dialog.show();
			break;
		}
	}

}