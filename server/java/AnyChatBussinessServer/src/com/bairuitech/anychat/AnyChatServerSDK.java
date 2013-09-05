package com.bairuitech.anychat;		// �����޸İ�������

import com.bairuitech.anychat.AnyChatTransTaskOutParam;

public class AnyChatServerSDK
{
	public static final int BRAS_SERVERAPPMSG_CONNECTED		= 	1;	///< ��AnyChat���ķ��������ӳɹ�
	public static final int BRAS_SERVERAPPMSG_DISCONNECT	=	2;	///< ��AnyChat���ķ������Ͽ�����

	AnyChatServerEvent event;
	
	// �����¼��ص�֪ͨ�ӿ�
	public void SetServerEvent(AnyChatServerEvent e)
	{
		RegisterNotify();
		this.event = e;
	}
	
	// ��ѯSDK�汾��Ϣ������ʱ���
	public native String GetSDKVersion();
    
    // ע����Ϣ֪ͨ
    public native int RegisterNotify();
    
    // ��ʼ��SDK
    public native int InitSDK(int flags);
    // �ͷ���Դ
    public native int Release();
    
    // ����SDK��ʱ���ص�������dwElapse����ʱ���������λ��ms��
    public native int SetTimerEventCallBack(int elapse);
    
	// �����ļ�
	public static native int TransFile(int userid, String filepath, int wparam, int lparam, int flags, AnyChatTransTaskOutParam outParam);
	// ͸��ͨ�����ͻ�����
	public static native int TransBuffer(int userid, byte[] buf, int len);
	// ͸��ͨ�����ͻ�������չ
	public static native int TransBufferEx(int userid, byte[] buf, int len, int wparam, int lparam, int flags, AnyChatTransTaskOutParam outParam);
	// ����SDK Filter ͨ������
	public static native int SendSDKFilterData(byte[] buf, int len);
	
	// ��ָ���û���������
	public static native int SendBufToUser(int userid, byte[] buf, int len);
	// ��ָ�����䷢������
	public static native int SendBufToRoom(int roomid, byte[] buf, int len);
	
	// ���Ķ�¼�����
	public static native int StreamRecordCtrl(int dwUserId, int bStartRecord, int dwFlags, int dwParam, int dwRecordServerId);
	
	// ��Ƶ�����¼����ƣ����󡢻ظ����Ҷϵȣ�
	public static native int VideoCallControl(int dwEventType, int dwUserId, int dwErrorCode, int dwFlags, int dwParam, String lpUserStr);
	
	
	// ������Ӧ�ó�����Ϣ�ص���������
	private void OnAnyChatServerAppMessageCallBack(int dwMsg)
	{
		if(this.event != null)
			this.event.OnAnyChatServerAppMessageCallBack(dwMsg);
	}
	// SDK��ʱ���ص���������
	private void OnAnyChatTimerEventCallBack()
	{
		if(this.event != null)
			this.event.OnAnyChatTimerEventCallBack();
	}
	
	// �û������֤�ص���������
	private int OnAnyChatVerifyUserCallBack(String szUserName, String szPassword, AnyChatVerifyUserOutParam outParam)
	{
		int ret = -1;
		if(this.event != null)
			ret = this.event.OnAnyChatVerifyUserCallBack(szUserName, szPassword, outParam);
		return ret;
	}
	
	// �û���¼�ɹ��ص���������
	private void OnAnyChatUserLoginActionCallBack(int dwUserId, String szUserName, int dwLevel, String szIpAddr)
	{
		if(this.event != null)
			this.event.OnAnyChatUserLoginActionCallBack(dwUserId, szUserName, dwLevel, szIpAddr);
	}
	// �û�ע���ص���������
	private void OnAnyChatUserLogoutActionCallBack(int dwUserId)
	{
		if(this.event != null)
			this.event.OnAnyChatUserLogoutActionCallBack(dwUserId);
	}
	
	// �û�������뷿��ص���������
	private int OnAnyChatPrepareEnterRoomCallBack(int dwUserId, int dwRoomId, String szRoomName, String szPassword)
	{
		int errorcode = -1;
		if(this.event != null)
			errorcode = this.event.OnAnyChatPrepareEnterRoomCallBack(dwUserId, dwRoomId, szRoomName, szPassword);
		return errorcode;
	}
	
	// �û����뷿��ص���������
	private void OnAnyChatUserEnterRoomActionCallBack(int dwUserId, int dwRoomId)
	{
		if(this.event != null)
			this.event.OnAnyChatUserEnterRoomActionCallBack(dwUserId, dwRoomId);
	}
	
	// �û��뿪����ص���������
	private void OnAnyChatUserLeaveRoomActionCallBack(int dwUserId, int dwRoomId)
	{
		if(this.event != null)
			this.event.OnAnyChatUserLeaveRoomActionCallBack(dwUserId, dwRoomId);
	}
  
    // �ļ�����ص���������
	private void OnAnyChatTransFileCallBack(int dwUserId, String szFileName, String szTempFilePath, int dwFileLength, int wParam, int lParam, int dwTaskId)
    {
		if(this.event != null)
			this.event.OnAnyChatTransFile(dwUserId, szFileName, szTempFilePath, dwFileLength, wParam, lParam, dwTaskId);
    }
    // �������ص���������
	private void OnAnyChatTransBufferCallBack(int dwUserId, byte[] lpBuf, int dwLen)
    {
		if(this.event != null)
			this.event.OnAnyChatTransBuffer(dwUserId, lpBuf, dwLen);
    }
    // ��������չ�ص���������
	private void OnAnyChatTransBufferExCallBack(int dwUserId, byte[] lpBuf, int dwLen, int wParam, int lParam, int dwTaskId)
    {
		if(this.event != null)
			this.event.OnAnyChatTransBufferEx(dwUserId, lpBuf, dwLen, wParam, lParam, dwTaskId);
    }
    // ���������͵�SDK Filter Data���ݻص���������
	private void OnAnyChatSDKFilterDataCallBack(int dwUserId, byte[] buf, int len)
    {
		if(this.event != null)
			this.event.OnAnyChatSDKFilterData(dwUserId, buf, len);
    }
	
	// �յ��û���������ͨ�����ݻص���������
	private void OnAnyChatRecvUserTextMsgCallBack(int dwRoomId, int dwSrcUserId, int dwTarUserId, int bSecret, String szTextMessage, int dwLen)
	{
		if(this.event != null)
			this.event.OnAnyChatRecvUserTextMsgCallBack(dwRoomId, dwSrcUserId, dwTarUserId, bSecret, szTextMessage, dwLen);
	}
	
	// ������¼��ص���������
	private void OnAnyChatServerRecordCallBack(int dwUserId, int dwParam, int dwRecordServerId, int dwElapse, String szRecordFileName)
	{
		if(this.event != null)
			this.event.OnAnyChatServerRecordCallBack(dwUserId, dwParam, dwRecordServerId, dwElapse, szRecordFileName);
	}
	
	// ��Ƶͨ����Ϣ֪ͨ�ص���������
	private int OnAnyChatVideoCallEventCallBack(int dwEventType, int dwSrcUserId, int dwTarUserId, int dwErrorCode, int dwFlags, int dwParam, String lpUserStr)
	{
		if(this.event != null)
			return this.event.OnAnyChatVideoCallEventCallBack(dwEventType, dwSrcUserId, dwTarUserId, dwErrorCode, dwFlags, dwParam, lpUserStr);
		else
			return -1;
	}
	
    static {
    	System.loadLibrary("anychatserver4java");
    }
    
}


