package com.bairuitech.anychat;

import com.bairuitech.anychat.VerifyUserOutParam;

// ���ݴ���֪ͨ�ӿ�
public interface AnyChatServerEvent {
	
	// ������Ӧ�ó�����Ϣ�ص���������
	public void OnAnyChatServerAppMessageCallBack(int dwMsg);
	// SDK��ʱ���ص���������
	public void OnAnyChatTimerEventCallBack();
	
	// �û������֤�ص���������
	public int OnAnyChatVerifyUserCallBack(String szUserName, String szPassword, VerifyUserOutParam outParam);
	// �û���¼�ɹ��ص���������
	public void OnAnyChatUserLoginActionCallBack(int dwUserId, String szUserName, int dwLevel, String szIpAddr);
	// �û�ע���ص���������
	public void OnAnyChatUserLogoutActionCallBack(int dwUserId);
	
	// �û�������뷿��ص���������
	public int OnAnyChatPrepareEnterRoomCallBack(int dwUserId, int dwRoomId, String szRoomName, String szPassword);
	// �û����뷿��ص���������
	public void OnAnyChatUserEnterRoomActionCallBack(int dwUserId, int dwRoomId);
	// �û��뿪����ص���������
	public void OnAnyChatUserLeaveRoomActionCallBack(int dwUserId, int dwRoomId);
	
	// �ļ�����ص���������
	public void OnAnyChatTransFile(int dwUserId, String szFileName, String szTempFilePath, int dwFileLength, int wParam, int lParam, int dwTaskId);
	// ͸��ͨ�����ݻص���������
	public void OnAnyChatTransBuffer(int dwUserId, byte[] lpBuf, int dwLen);
	// ��չ͸��ͨ�����ݻص���������
	public void OnAnyChatTransBufferEx(int dwUserId, byte[] lpBuf, int dwLen, int wParam, int lParam, int dwTaskId);
	// SDK Filter ͨ�����ݻص���������
	public void OnAnyChatSDKFilterData(int dwUserId, byte[] lpBuf, int dwLen);
	
	// �յ��û���������ͨ�����ݻص���������
	public void OnAnyChatRecvUserTextMsgCallBack(int dwRoomId, int dwSrcUserId, int dwTarUserId, int bSecret, String szTextMessage, int dwLen);
	// ������¼��ص���������
	public void OnAnyChatServerRecordCallBack(int dwUserId, int dwParam, int dwRecordServerId, int dwElapse, String szRecordFileName);
}
