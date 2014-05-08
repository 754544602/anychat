// AnyChatCallCenterServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnyChatCallCenterServer.h"
#include "AnyChatCallCenterServerDlg.h"

#include "AnyChatServerSDK.h"
#pragma comment(lib,"AnyChatServerSDK.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SENDBUF_STYLE_SYSTEM	0		///< ϵͳ�㲥
#define SENDBUF_STYLE_ROOM		1		///< ����㲥
#define SENDBUF_STYLE_USER		2		///< ָ���û�

#define TIMER_REFRESH_ONLINES	1		///< ˢ�������û�����ʱ��



// ������Ӧ�ó�����Ϣ�ص���������
void CALLBACK OnServerAppMessageExCallBack(DWORD dwNotifyMessage, DWORD wParam, DWORD lParam, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(dwNotifyMessage == BRAS_MESSAGE_CORESERVERCONN)
	{
		lpServerDlg->m_dwOnlineUsers = 0;
		if(wParam == 0)
			lpServerDlg->AppendLogString("Success connected with AnyChatCoreServer...");
		else
			lpServerDlg->AppendLogString("Disconnected from the AnyChatCoreServer");
		// ��������û��б�
		lpServerDlg->m_onlineUserList.clear();
	}
	else if(dwNotifyMessage == BRAS_MESSAGE_RECORDSERVERCONN)
	{
		if(wParam == 0)
		{
			CString strNotify;
			strNotify.Format("Success connected with AnyChatRecordServer...(serverid:%d)", lParam);
			lpServerDlg->AppendLogString(strNotify);
		}
		else
		{
			CString strNotify;
			strNotify.Format("Disconnected from the AnyChatRecordServer...(errorcode:%d, serverid:%d)", wParam, lParam);
			lpServerDlg->AppendLogString(strNotify);
		}
	}
	else
	{
		CString strNotify;
		strNotify.Format("OnServerAppMessageExCallBack(dwNotifyMessage:%d, wParam:%d, lParam:%d)", dwNotifyMessage, wParam, lParam);
		lpServerDlg->AppendLogString(strNotify);
	}
}

/**
 *	SDK��ʱ���ص��������壬�ϲ�Ӧ�ÿ����ڸûص��д���ʱ���񣬶�����Ҫ���⿪���̣߳����Ƕ�ʱ��
 *	������������̣߳�����ע��Windows��ʱ��������뿼�Ƕ��߳�ͬ��������
 *	��ʹ��SDK��TimerEvent�ص�������Ҫ���ǣ���Ϊ��ʱ���ص��ͱ�Ļص���������˳�򴥷���
 */
void CALLBACK OnTimerEventCallBack(LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg)
	{
//		lpServerDlg->AppendLogString("��ʱ���¼�");
	}
}

// �û������֤�ص���������
DWORD CALLBACK VerifyUserCallBack(IN LPCTSTR lpUserName,IN LPCTSTR lpPassword, OUT LPDWORD lpUserID, OUT LPDWORD lpUserLevel, OUT LPTSTR lpNickName,IN DWORD dwNCLen, LPVOID lpUserValue)
{
	static DWORD dwUserIdSeed = 1;
	*lpUserID = dwUserIdSeed++;

	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("VerifyUser(%s - %s) --> userid(%d)"),lpUserName,lpPassword,(int)*lpUserID);
		lpServerDlg->AppendLogString(strMsg);
	}
	return 0;
}
// �û�������뷿��ص���������
DWORD CALLBACK PrepareEnterRoomCallBack(DWORD dwUserId, DWORD dwRoomId, LPCTSTR lpRoomName,LPCTSTR lpPassword, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("PrepareEnterRoom(dwUserId:%d - dwRoomId:%d)"),(int)dwUserId,(int)dwRoomId);
		lpServerDlg->AppendLogString(strMsg);
	}
	return 0;
}
// �û���¼�ɹ��ص���������
void CALLBACK OnUserLoginActionCallBack(DWORD dwUserId, LPCTSTR szUserName, DWORD dwLevel, LPCTSTR szIpAddr, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(!lpServerDlg)
		return;
	
	// ����û�����
	int iGroupId = 1;
	BRAS_UserInfoControl(dwUserId, BRAS_USERINFO_CTRLCODE_ADDGROUP, iGroupId, 0, "�ҵĺ���");	// �û���+���뷽ʽ��¼���û�
	iGroupId = 2;
	BRAS_UserInfoControl(dwUserId, BRAS_USERINFO_CTRLCODE_ADDGROUP, iGroupId, 0, "�����ο�");	// ����Ϊ�յ��û�

	// ����ǰ���������û����Ϊ�Լ��ĺ���
	std::list<DWORD>::iterator it = lpServerDlg->m_onlineUserList.begin();
	while(it != lpServerDlg->m_onlineUserList.end())
	{
		DWORD otheruserid = *it;
		BRAS_UserInfoControl(dwUserId, BRAS_USERINFO_CTRLCODE_ADDFRIEND, otheruserid, 0, "");
		it++;
	}

	// ���ú��������Ĺ�ϵ��������������һ�����飩
	iGroupId = 1;
	it = lpServerDlg->m_onlineUserList.begin();
	while(it != lpServerDlg->m_onlineUserList.end())
	{
		DWORD otheruserid = *it;
		iGroupId = (otheruserid > 0) ? 1 : 2;		// �ο�����Ϊ�գ�userid�ɺ��ķ��������䣬Ϊ����
		BRAS_UserInfoControl(dwUserId, BRAS_USERINFO_CTRLCODE_SETGROUPRELATION, iGroupId, otheruserid, "");
		it++;
	}

	// ���õ�ǰ�û���Ϣ���û����ϣ��ͻ��˿���ͨ��API��BRAC_GetUserInfo����ȡ��Щ��Ϣ��
	int iInfoId = 1;	// InfoId��ҵ����Լ�����
	BRAS_SetUserInfo(dwUserId, iInfoId, szUserName, 0);
	iInfoId = 2;
	BRAS_SetUserInfo(dwUserId, iInfoId, szIpAddr, 0);
	iInfoId = 3;
	BRAS_SetUserInfo(dwUserId, iInfoId, "�ҵ�ǩ��", 0);
	iInfoId = 4;
	CHAR szImageId[10] = {0};
	_snprintf(szImageId, sizeof(szImageId), "%d", dwUserId%10 + 1);
	BRAS_SetUserInfo(dwUserId, iInfoId, szImageId, 0);		// �������һ��ͼ��ID

	// �������û����Ϊ�����û��ĺ����б���
	it = lpServerDlg->m_onlineUserList.begin();
	while(it != lpServerDlg->m_onlineUserList.end())
	{
		DWORD otheruserid = *it;
		// ��Ӻ���
		BRAS_UserInfoControl(otheruserid, BRAS_USERINFO_CTRLCODE_ADDFRIEND, dwUserId, 0, "");
		// �������ѷ���
		iGroupId = (dwUserId > 0) ? 1 : 2;
		BRAS_UserInfoControl(otheruserid, BRAS_USERINFO_CTRLCODE_SETGROUPRELATION, iGroupId, dwUserId, "");
		// �·�ͬ��ָ��������õĺ���ͬ�����ͻ���
		BRAS_UserInfoControl(otheruserid, BRAS_USERINFO_CTRLCODE_SYNCDATA,  0, 0, "");
		it++;
	}

	// �·�ͬ��ָ���ǰ�����õ�����ͬ������ǰ�ͻ���
	BRAS_UserInfoControl(dwUserId, BRAS_USERINFO_CTRLCODE_SYNCDATA,  0, 0, "");

	// �������û����������û��б�
	lpServerDlg->m_onlineUserList.push_back(dwUserId);

	lpServerDlg->m_dwOnlineUsers++;
	if(lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnUserLoginAction(dwUserId:%d - Name:%s)"),(int)dwUserId,szUserName);
		lpServerDlg->AppendLogString(strMsg);
	}
}
// �û�ע���ص���������
void CALLBACK OnUserLogoutActionExCallBack(DWORD dwUserId, DWORD dwErrorCode, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(!lpServerDlg)
		return;

	// �������û��б���ɾ��
	std::list<DWORD>::iterator it = lpServerDlg->m_onlineUserList.begin();
	while(it != lpServerDlg->m_onlineUserList.end())
	{
		if(*it == dwUserId)
		{
			lpServerDlg->m_onlineUserList.erase(it);
			break;
		}
	}
	// ���ķ�������֪ͨ�����û�������Ǻ��ѣ�����ʾ�������ߣ�����Ҫҵ���������Ԥ

	lpServerDlg->m_dwOnlineUsers--;
	if(lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnUserLogoutAction(dwUserId:%d, dwErrorCode:%d)"),(int)dwUserId, dwErrorCode);
		lpServerDlg->AppendLogString(strMsg);
	}
}
// �û����뷿��ص���������
void CALLBACK OnUserEnterRoomActionCallBack(DWORD dwUserId, DWORD dwRoomId, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnUserEnterRoomAction(dwUserId:%d - dwRoomId:%d)"),(int)dwUserId,(int)dwRoomId);
		lpServerDlg->AppendLogString(strMsg);
	}
}
// �û��뿪����ص���������
void CALLBACK OnUserLeaveRoomActionCallBack(DWORD dwUserId, DWORD dwRoomId, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnUserLeaveRoomAction(dwUserId:%d - dwRoomId:%d)"),(int)dwUserId,(int)dwRoomId);
		lpServerDlg->AppendLogString(strMsg);
	}
}
// �ϲ�ҵ���Զ������ݻص���������
void CALLBACK OnRecvUserFilterDataCallBack(DWORD dwUserId, BYTE* lpBuf, DWORD dwLen, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnRecvUserFilterData(dwUserId:%d - Buf:%s)"),(int)dwUserId,lpBuf);
		lpServerDlg->AppendLogString(strMsg);
	}
}

// �յ��û���������ͨ�����ݻص���������
void CALLBACK OnRecvUserTextMsgCallBack(DWORD dwRoomId, DWORD dwSrcUserId, DWORD dwTarUserId, BOOL bSecret, LPCTSTR lpTextMessage, DWORD dwLen, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnRecvUserTextMsg(dwRoomId:%d, dwSrcUserId:%d, dwTarUserId:%d - Buf:%s)"),(int)dwRoomId,(int)dwSrcUserId,(int)dwTarUserId,lpTextMessage);
		lpServerDlg->AppendLogString(strMsg);
	}
}

// ͸��ͨ�����ݻص���������
void CALLBACK OnTransBufferCallBack(DWORD dwUserId, LPBYTE lpBuf, DWORD dwLen, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnTransBufferCallBack(dwUserId:%d - BufLen:%d)"),(int)dwUserId,dwLen);
		lpServerDlg->AppendLogString(strMsg);
	}
}

// ͸��ͨ��������չ�ص���������
void CALLBACK OnTransBufferExCallBack(DWORD dwUserId, LPBYTE lpBuf, DWORD dwLen, DWORD wParam, DWORD lParam, DWORD dwTaskId, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnTransBufferExCallBack(dwUserId:%d - BufLen:%d - wParam:%d - lParam:%d - dwTaskId:%d)"),(int)dwUserId,dwLen, wParam, lParam, dwTaskId);
		lpServerDlg->AppendLogString(strMsg);
	}
}
// �ļ�����ص���������
void CALLBACK OnTransFileCallBack(DWORD dwUserId, LPCTSTR lpFileName, LPCTSTR lpTempFilePath, DWORD dwFileLength, DWORD wParam, DWORD lParam, DWORD dwTaskId, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg && lpServerDlg->m_bShowUserLog)
	{
		CString strMsg;
		strMsg.Format(_T("OnTransFileCallBack(dwUserId:%d, PathName:%s - wParam:%d - lParam:%d - dwTaskId:%d)"),(int)dwUserId, lpFileName, wParam, lParam, dwTaskId);
		lpServerDlg->AppendLogString(strMsg);
	}
}

// ������¼��ص���������
void CALLBACK OnServerRecord_CallBack(DWORD dwUserId, DWORD dwParam, DWORD dwRecordServerId, DWORD dwElapse, LPCTSTR lpRecordFileName, LPVOID lpUserValue)
{
	CAnyChatCallCenterServerDlg* lpServerDlg = (CAnyChatCallCenterServerDlg*)lpUserValue;
	if(lpServerDlg)
	{
		CString strMsg;
		strMsg.Format(_T("OnServerRecordCallBack(dwUserId:%d, FileName:%s)"),(int)dwUserId, lpRecordFileName);
		lpServerDlg->AppendLogString(strMsg);
	}
}


CAnyChatCallCenterServerDlg::CAnyChatCallCenterServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnyChatCallCenterServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnyChatCallCenterServerDlg)
	m_iTargetId = 0;
	m_bShowUserLog = FALSE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAnyChatCallCenterServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnyChatCallCenterServerDlg)
	DDX_Control(pDX, IDC_EDIT_LOG, m_ctrlEditLog);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_ComboStyle);
	DDX_Text(pDX, IDC_EDIT_TARGETID, m_iTargetId);
	DDX_Check(pDX, IDC_CHECK_SHOWLOG, m_bShowUserLog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAnyChatCallCenterServerDlg, CDialog)
	//{{AFX_MSG_MAP(CAnyChatCallCenterServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SENDBUF, OnButtonSendbuf)
	ON_BN_CLICKED(IDC_BUTTONTRANSFILE, OnButtonTransFile)
	ON_BN_CLICKED(IDC_BUTTONTRANSBUFFEREX, OnButtonTransBufferEx)
	ON_BN_CLICKED(IDC_BUTTON_TRANSBUFFER, OnButtonTransBuffer)
	ON_BN_CLICKED(IDC_BUTTON_STARTRECORD, OnButtonStartRecord)
	ON_BN_CLICKED(IDC_BUTTON_STOPRECORD, OnButtonStopRecord)
	ON_BN_CLICKED(IDC_CHECK_SHOWLOG, OnCheckShowLog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_KICKOUT, OnButtonKickOut)
	ON_BN_CLICKED(IDC_BUTTON_HANGUP, OnButtonHangUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnyChatCallCenterServerDlg message handlers

BOOL CAnyChatCallCenterServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_bShowUserLog = TRUE;
	UpdateData(FALSE);
	
	DWORD dwIndex = m_ComboStyle.AddString(_T("�����û��㲥"));
	m_ComboStyle.SetItemData(dwIndex,SENDBUF_STYLE_SYSTEM);
	m_ComboStyle.SetCurSel(dwIndex);
	dwIndex = m_ComboStyle.AddString(_T("ָ������㲥"));
	m_ComboStyle.SetItemData(dwIndex,SENDBUF_STYLE_ROOM);
	dwIndex = m_ComboStyle.AddString(_T("ָ���û�����"));
	m_ComboStyle.SetItemData(dwIndex,SENDBUF_STYLE_USER);

	
	// ���÷�����Ӧ�ó�����Ϣ�ص�����
	BRAS_SetOnServerAppMessageExCallBack(OnServerAppMessageExCallBack,this);
	// ����SDK��ʱ���ص�����
	BRAS_SetTimerEventCallBack(1000,OnTimerEventCallBack,this);
	// �����û������֤�ص�����
	BRAS_SetVerifyUserCallBack(VerifyUserCallBack,this);
	// �����û�������뷿��ص�����
	BRAS_SetPrepareEnterRoomCallBack(PrepareEnterRoomCallBack, this);
	// �����û���¼�ɹ��ص�����
	BRAS_SetOnUserLoginActionCallBack(OnUserLoginActionCallBack, this);
	// �����û�ע���ص���������չ��
	BRAS_SetOnUserLogoutActionExCallBack(OnUserLogoutActionExCallBack, this);
	// �����û����뷿��ص�����
	BRAS_SetOnUserEnterRoomActionCallBack(OnUserEnterRoomActionCallBack, this);
	// �����û��뿪����ص�����
	BRAS_SetOnUserLeaveRoomActionCallBack(OnUserLeaveRoomActionCallBack, this);
	// �����û��ϲ�ҵ���Զ������ݻص�����
	BRAS_SetOnRecvUserFilterDataCallBack(OnRecvUserFilterDataCallBack, this);
	// �û���������ͨ�����ݻص�����
	BRAS_SetOnRecvUserTextMsgCallBack(OnRecvUserTextMsgCallBack,this);
	// ����͸��ͨ�����ݻص�����
	BRAS_SetOnTransBufferCallBack(OnTransBufferCallBack, this);
	// ����͸��ͨ��������չ�ص�����
	BRAS_SetOnTransBufferExCallBack(OnTransBufferExCallBack, this);
	// �����ļ�����ص�����
	BRAS_SetOnTransFileCallBack(OnTransFileCallBack, this);
	// ���÷�����¼��ص�����
	BRAS_SetOnServerRecordCallBack(OnServerRecord_CallBack, this);
	
	BRAS_InitSDK(0);

	m_dwOnlineUsers = 0;
	SetTimer(TIMER_REFRESH_ONLINES, 1000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAnyChatCallCenterServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CAnyChatCallCenterServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAnyChatCallCenterServerDlg::OnDestroy() 
{
	BRAS_Release();

	CDialog::OnDestroy();
}

void CAnyChatCallCenterServerDlg::OnButtonSendbuf() 
{
	UpdateData(TRUE);
	CString strBuffer;
	GetDlgItemText(IDC_EDIT_SENDBUF,strBuffer);
	if(strBuffer.IsEmpty())
		return;

	DWORD dwStyle = m_ComboStyle.GetCurSel();
	if(dwStyle == SENDBUF_STYLE_SYSTEM)
		BRAS_SendBufToRoom(-1,strBuffer.GetBuffer(0),strBuffer.GetLength());
	else if(dwStyle == SENDBUF_STYLE_ROOM)
		BRAS_SendBufToRoom(m_iTargetId,strBuffer.GetBuffer(0),strBuffer.GetLength());
	else
		BRAS_SendBufToUser(m_iTargetId,strBuffer.GetBuffer(0),strBuffer.GetLength());	
}

/**
 *	��ʾ��־��Ϣ
 */
void CAnyChatCallCenterServerDlg::AppendLogString(CString logstr)
{
	m_strLogInfo += (logstr + "\r\n");
	m_ctrlEditLog.SetWindowText(m_strLogInfo);
	m_ctrlEditLog.LineScroll(m_ctrlEditLog.GetLineCount());
}

void CAnyChatCallCenterServerDlg::OnButtonTransFile() 
{
	UpdateData(TRUE);
	if(m_iTargetId == -1)
	{
		AppendLogString("BRAS_TransFile ����Ŀ���û�����Ϊ-1");
		return;
	}
	static TCHAR BASED_CODE szFilter[] = _T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,_T(""),NULL,  OFN_HIDEREADONLY ,szFilter);
	if(dlg.DoModal() == IDOK)
	{
		DWORD wParam = 1;				// wParam��lParam�Ǹ��������������ϲ�Ӧ����չ
		DWORD lParam = 2;
		DWORD dwFlags = 0;
		DWORD dwTaskId = 0;
		DWORD ret = BRAS_TransFile(m_iTargetId, dlg.GetPathName(), wParam, lParam, dwFlags, dwTaskId);
		CString strLog;
		if(ret == 0)
			strLog.Format(_T("BRAS_TransFile �����ύ�ɹ���TaskId:%d"), dwTaskId);
		else
			strLog.Format(_T("BRAS_TransFile �����ύʧ�ܣ�ErrorCode:%d"), ret);
		AppendLogString(strLog);
	}
}

// ʹ��͸��ͨ����չ������ָ���û����ʹ����ݿ黺����
void CAnyChatCallCenterServerDlg::OnButtonTransBufferEx() 
{
	UpdateData(TRUE);
	if(m_iTargetId == -1)
	{
		AppendLogString("BRAS_TransBufferEx ����Ŀ���û�����Ϊ-1");
		return;
	}
	BYTE szBuf[1024*10] = {0};		// ʵ�飬��һ���ջ�������Ŀ���û����ж��Ƿ��ܽ��յ�
	DWORD wParam = 1;				// wParam��lParam�Ǹ��������������ϲ�Ӧ����չ
	DWORD lParam = 2;
	DWORD dwFlags = 0;
	DWORD dwTaskId = 0;
	DWORD ret = BRAS_TransBufferEx(m_iTargetId, szBuf, sizeof(szBuf), wParam, lParam, dwFlags, dwTaskId);
	CString strLog;
	if(ret == 0)
		strLog.Format(_T("BRAS_TransBufferEx �����ύ�ɹ���TaskId:%d"), dwTaskId);
	else
		strLog.Format(_T("BRAS_TransBufferEx �����ύʧ�ܣ�ErrorCode:%d"), ret);
	AppendLogString(strLog);
}
// ʹ��͸��ͨ��������ָ���û����ͻ�����
void CAnyChatCallCenterServerDlg::OnButtonTransBuffer() 
{
	UpdateData(TRUE);
	CString strBuffer;
	GetDlgItemText(IDC_EDIT_SENDBUF,strBuffer);
	if(strBuffer.IsEmpty())
		return;
	if(m_iTargetId == -1)
	{
		AppendLogString("BRAS_TransBuffer ����Ŀ���û�����Ϊ-1");
		return;
	}
	BRAS_TransBuffer(m_iTargetId,(LPBYTE)strBuffer.GetBuffer(0),strBuffer.GetLength());	
}

void CAnyChatCallCenterServerDlg::OnButtonStartRecord() 
{
	UpdateData(TRUE);
	if(m_iTargetId == 0)
	{
		MessageBox("������Ŀ���û����", "����", MB_OK);
		GetDlgItem(IDC_EDIT_TARGETID)->SetFocus();
		return;
	}
	BRAS_StreamRecordCtrl(m_iTargetId, TRUE, 0, 0, -1);
}

void CAnyChatCallCenterServerDlg::OnButtonStopRecord() 
{
	UpdateData(TRUE);
	if(m_iTargetId == 0)
	{
		MessageBox("������Ŀ���û����", "����", MB_OK);
		GetDlgItem(IDC_EDIT_TARGETID)->SetFocus();
		return;
	}
	BRAS_StreamRecordCtrl(m_iTargetId, FALSE, 0, 0, -1);
}
/**
 *	�û�ѡ���Ƿ���ʾ�û����־
 *	�������û�����ʱ����ʾ�û����־��Ӱ�쵽���������������
 */
void CAnyChatCallCenterServerDlg::OnCheckShowLog() 
{
	UpdateData(TRUE);
}

void CAnyChatCallCenterServerDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == TIMER_REFRESH_ONLINES)
	{
		CString strNotify;
		strNotify.Format("�����û�����%d", m_dwOnlineUsers);
		SetDlgItemText(IDC_STATIC_ACTIVEINFO, strNotify);
	}
	CDialog::OnTimer(nIDEvent);
}
/**
 *	��Ŀ���û���ϵͳ���ߵ���Ŀ���û�����ǿ������
 */
void CAnyChatCallCenterServerDlg::OnButtonKickOut() 
{
	UpdateData(TRUE);
	if(m_iTargetId == 0)
	{
		MessageBox("������Ŀ���û����", "����", MB_OK);
		GetDlgItem(IDC_EDIT_TARGETID)->SetFocus();
		return;
	}
	BRAS_UserInfoControl(m_iTargetId, BRAS_USERINFO_CTRLCODE_KICKOUT);
}
/**
 *	�Ҷ�Ŀ���û�����Ƶ���лỰ����Ƶ���лỰΪ�ͻ��˲���API��BRAC_VideoCallControl�����ĻỰҵ��
 */
void CAnyChatCallCenterServerDlg::OnButtonHangUp() 
{
	UpdateData(TRUE);
	if(m_iTargetId == 0)
	{
		MessageBox("������Ŀ���û����", "����", MB_OK);
		GetDlgItem(IDC_EDIT_TARGETID)->SetFocus();
		return;
	}
	BRAS_VideoCallControl(BRAS_VIDEOCALL_EVENT_FINISH, m_iTargetId, 0);
}
