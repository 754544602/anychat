// BRAnyChatMeetingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BRAnyChatMeeting.h"
#include "BRAnyChatMeetingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ͼ����ImageList�е�����
#define IMAGE_INDEX_NULL			-1
#define IMAGE_INDEX_MIC				0
#define IMAGE_INDEX_CAMERA_NORMAL	1
#define IMAGE_INDEX_CAMERA_OPEN		2


CBRAnyChatMeetingDlg::CBRAnyChatMeetingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBRAnyChatMeetingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBRAnyChatMeetingDlg)
	m_pLoginDlg = NULL;
	m_pHallDlg = NULL;
	m_dwSelfUserId = -1;
	m_pSelfVideoDlg = NULL;
	m_pSettingsDlg = NULL;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBRAnyChatMeetingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBRAnyChatMeetingDlg)
	DDX_Control(pDX, IDC_EDIT_TEXTOUTPUT, m_ctrlTextOutput);
	DDX_Control(pDX, IDC_COMBO_DISPNUM, m_ctrlDispNumCombo);
	DDX_Control(pDX, IDC_LIST_USER, m_ctrlUserList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBRAnyChatMeetingDlg, CDialog)
	//{{AFX_MSG_MAP(CBRAnyChatMeetingDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_DISPNUM, OnSelchangeComboDispNum)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, OnDblclkListUser)
	ON_BN_CLICKED(IDC_BUTTON_SENDTEXT, OnButtonSendText)
	ON_BN_CLICKED(IDC_BUTTON_ADVSET, OnButtonAdvSet)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBRAnyChatMeetingDlg message handlers

BOOL CBRAnyChatMeetingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
/*
#ifdef _HRC_USE_SKIN_
	skinppRemoveSkinHwnd(GetDlgItem(IDC_EDIT_TEXTOUTPUT)->GetSafeHwnd());
	skinppSetSkinOwnerMenu(FALSE);
#endif
*/
	TCHAR szLogoPath[MAX_PATH] = {0};
	TCHAR szLogoFile[MAX_PATH] = {0};
	GetModuleFileName(NULL,szLogoPath,sizeof(szLogoPath)); 
	(strrchr(szLogoPath,'\\'))[1] = 0; 
	strcat(szLogoPath,SUB_PATH_SKINFILE);
	_snprintf(szLogoFile,sizeof(szLogoFile),"%s\\%s",szLogoPath,_T("logo.jpg"));
	if(IsFileExist(szLogoFile))
		m_LogoPic.Load(szLogoFile);
	else
		m_LogoPic.Load(IDR_IMAGE_CHATLOGO);

	ShowWindow(SW_HIDE);
	// ����������ʾ��¼�Ի���
	m_pLoginDlg = new CLoginDlg(this);
	if(m_pLoginDlg->Create(IDD_DIALOG_LOGIN,this))
	{
		m_pLoginDlg->ModifyStyleEx(0,WS_EX_APPWINDOW);
		m_pLoginDlg->CenterWindow();
		m_pLoginDlg->ShowWindow(SW_SHOW);
	}
	// ��������������棬������
	m_pHallDlg = new CHallDlg(this);
	if(m_pHallDlg->Create(IDD_DIALOG_HALL,this))
	{
		m_pHallDlg->ModifyStyleEx(0,WS_EX_APPWINDOW);
		m_pHallDlg->CenterWindow();
		m_pHallDlg->ShowWindow(SW_HIDE);
	}
	// ��ʼ��SDK
	InitAnyChatSDK();

	InitUserListCtrl();

	DWORD dwDispNumIdx = m_ctrlDispNumCombo.AddString("һ������ʾ");
	m_ctrlDispNumCombo.SetCurSel(dwDispNumIdx);
	m_ctrlDispNumCombo.SetItemData(dwDispNumIdx,1);
	dwDispNumIdx = m_ctrlDispNumCombo.AddString("��������ʾ");
	m_ctrlDispNumCombo.SetItemData(dwDispNumIdx,2);
	dwDispNumIdx = m_ctrlDispNumCombo.AddString("�ķ�����ʾ");
	m_ctrlDispNumCombo.SetItemData(dwDispNumIdx,4);

	// ��ʼ��������Ƶ��ʾ���
	m_pSelfVideoDlg = new CVideoPanelDlg(this);
	if(m_pSelfVideoDlg->Create(IDD_DIALOG_VIDEOPANEL))
	{
		CRect rcLocalVideo;
		GetDlgItem(IDC_STATIC_LOCALVIDEO)->GetWindowRect(rcLocalVideo);
		ScreenToClient(rcLocalVideo);
		m_pSelfVideoDlg->ShowWindow(SW_SHOW);
		m_pSelfVideoDlg->MoveWindow(rcLocalVideo);
	}
	for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
	{
		m_pPanelObjectArray[i] = new CVideoPanelDlg(this);
		if(m_pPanelObjectArray[i]->Create(IDD_DIALOG_VIDEOPANEL))
		{
			CleanVideoPanelBind(m_pPanelObjectArray[i]);
			m_pPanelObjectArray[i]->ShowWindow(SW_HIDE);
		}
	}
	// Ĭ������Ϊ����ģʽ
	RelocateVideoPanel(1);

	CHAR szConfigFile[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szConfigFile,_MAX_PATH);
	(strrchr(szConfigFile,'\\'))[1] = 0; 
	strcat(szConfigFile,DEFAULT_CONFIG_FILENAME);
	
	TCHAR szTempStr[200] = {0};
	// ����Ʒ����
	if(::GetPrivateProfileString(DEFAULT_CONFIG_SECTION,_T("ProductName"),NULL,szTempStr,sizeof(szTempStr),szConfigFile))
	{
		strcat(szTempStr, " �� ������");
		SetWindowText(szTempStr);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBRAnyChatMeetingDlg::InitAnyChatSDK(void)
{
	// �򿪣��رգ�SDK����־��¼����
	BRAC_ActiveCallLog(TRUE);
	
	// ����SDK�����������Ŀ¼
	CHAR szCoreSDKPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,szCoreSDKPath,sizeof(szCoreSDKPath));
	(strrchr(szCoreSDKPath,'\\'))[1] = 0; 
	BRAC_SetSDKOption(BRAC_SO_CORESDK_PATH,szCoreSDKPath,strlen(szCoreSDKPath));
	
	// ����BRAC_InitSDK�ĵڶ���������dwFuncMode��������SDK����δ�����ص�����������ο������ĵ���
	DWORD dwFuncMode = BRAC_FUNC_VIDEO_AUTODISP | BRAC_FUNC_AUDIO_AUTOPLAY | BRAC_FUNC_CHKDEPENDMODULE | 
		BRAC_FUNC_AUDIO_VOLUMECALC | BRAC_FUNC_NET_SUPPORTUPNP | BRAC_FUNC_FIREWALL_OPEN |
		BRAC_FUNC_AUDIO_AUTOVOLUME | BRAC_FUNC_CONFIG_LOCALINI;
	BRAC_InitSDK(this->GetSafeHwnd()/*NULL*/,dwFuncMode);
	
	
	// ����¼����ʱ�ļ�����·��
	CHAR szRecordDirectory[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szRecordDirectory,MAX_PATH);
	(strrchr(szRecordDirectory,'\\'))[1] = 0; 
	strcat(szRecordDirectory,"Record");
	BRAC_SetSDKOption(BRAC_SO_RECORD_TMPDIR,szRecordDirectory,strlen(szRecordDirectory));
	// ����¼���ļ���������
	DWORD dwVideoBitrate = 200 * 1000;	// 200kbps
	BRAC_SetSDKOption(BRAC_SO_RECORD_VIDEOBR,(PCHAR)&dwVideoBitrate,sizeof(DWORD));
	DWORD dwAudioBitrate = 96 * 1000;	// 96kbps
	BRAC_SetSDKOption(BRAC_SO_RECORD_AUDIOBR,(PCHAR)&dwAudioBitrate,sizeof(DWORD));
	
	// ���ÿ�����ʱ�ļ�����·��
	CHAR szSnapShotDirectory[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szSnapShotDirectory,MAX_PATH);
	(strrchr(szSnapShotDirectory,'\\'))[1] = 0; 
	strcat(szSnapShotDirectory,"SnapShot");
	BRAC_SetSDKOption(BRAC_SO_SNAPSHOT_TMPDIR,szSnapShotDirectory,strlen(szSnapShotDirectory));
	
	// ����SDK��ʱ�ļ�·��
	CHAR szTempPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szTempPath,MAX_PATH);
	(strrchr(szTempPath,'\\'))[1] = 0; 
	strcat(szTempPath,"Temp");
	BRAC_SetSDKOption(BRAC_SO_CORESDK_TMPDIR,szTempPath,strlen(szTempPath));

	// ������Ƶ�Զ��������ܣ�Ĭ�Ϲرգ�
	DWORD bAudioAutoParam = TRUE;
	BRAC_SetSDKOption(BRAC_SO_AUDIO_AUTOPARAM,(PCHAR)&bAudioAutoParam,sizeof(DWORD));

	// ������Ƶ��ʾ����ΪDirectShowģʽ
	DWORD dwVideoDriver = BRAC_VSD_DIRECTSHOW;
	BRAC_SetSDKOption(BRAC_SO_VIDEOSHOW_DRIVERCTRL,(const char*)&dwVideoDriver, sizeof(DWORD));

	// ���÷�������֤����
	BRAC_SetServerAuthPass("BaiRuiTech");		// ��Ҫ������������SDKAuthPass����ͬ����Сд���У���SDK�����������ӵ�������
/*	
	DWORD dwVolume = 0;
	BRAC_AudioGetVolume(BRAC_AD_WAVEOUT,dwVolume);
	m_ctrlOutVolume.SetPos(dwVolume);
	
	dwVolume = 0;
	BRAC_AudioGetVolume(BRAC_AD_WAVEIN,dwVolume);
	m_ctrlInVolume.SetPos(dwVolume);
*/
/*	���ڲ���SDKָ���ͻ��˷���˿ڹ���
	DWORD dwTCPService = 8006;
	BRAC_SetSDKOption(BRAC_SO_NETWORK_TCPSERVICEPORT,(PCHAR)&dwTCPService,sizeof(DWORD));
	DWORD dwUDPService = 8008;
	BRAC_SetSDKOption(BRAC_SO_NETWORK_UDPSERVICEPORT,(PCHAR)&dwUDPService,sizeof(DWORD));
*/
}

void CBRAnyChatMeetingDlg::OnPaint() 
{
	CPaintDC paintdc(this);
	// ���ƶ�����LogoͼƬ
	m_LogoPic.Render(&paintdc,CRect(20,0,m_LogoPic.GetImageSize().cx+20,m_LogoPic.GetImageSize().cy));

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

HCURSOR CBRAnyChatMeetingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBRAnyChatMeetingDlg::OnDestroy() 
{
	if(m_pLoginDlg)
	{
		m_pLoginDlg->DestroyWindow();
		delete m_pLoginDlg;
		m_pLoginDlg = NULL;
	}
	if(m_pHallDlg)
	{
		m_pHallDlg->DestroyWindow();
		delete m_pHallDlg;
		m_pHallDlg = NULL;
	}
	if(m_pSettingsDlg)
	{
		m_pSettingsDlg->DestroyWindow();
		delete m_pSettingsDlg;
		m_pSettingsDlg = NULL;
	}

	for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
	{
		if(m_pPanelObjectArray[i])
		{
			CleanVideoPanelBind(m_pPanelObjectArray[i],FALSE);
			m_pPanelObjectArray[i]->DestroyWindow();
			delete m_pPanelObjectArray[i];
			m_pPanelObjectArray[i] = NULL;
		}
	}
	if(m_pSelfVideoDlg)
	{
		CleanVideoPanelBind(m_pSelfVideoDlg,FALSE);
		m_pSelfVideoDlg->DestroyWindow();
		delete m_pSelfVideoDlg;
		m_pSelfVideoDlg = NULL;
	}
	m_UserStateImageList.DeleteImageList();
	// �ͷ�SDK ��Դ
	BRAC_Release();
	CDialog::OnDestroy();
}

int CBRAnyChatMeetingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	
	return 0;
}

// �û��ر�ϵͳ
void CBRAnyChatMeetingDlg::OnUserExitSystem(void)
{
	PostQuitMessage(0);
//	CDialog::DestroyWindow();
}

// ���ӷ�������Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatConnectMessage(BOOL bSuccess, DWORD dwErrorCode)
{
	if(m_pLoginDlg)
		m_pLoginDlg->OnConnectNotify(bSuccess ? GV_ERR_SUCCESS : dwErrorCode);
}
// �û���½��Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatLoginMessage(DWORD dwUserId, DWORD dwErrorCode)
{
	if(m_pLoginDlg)
		m_pLoginDlg->OnLoginNotify(dwErrorCode);
	// �����¼�ɹ��������ص�¼���棬��ʾ�����������
	if(dwErrorCode == GV_ERR_SUCCESS)
	{
		m_dwSelfUserId = dwUserId;
		if(m_pLoginDlg)
			m_pLoginDlg->ShowWindow(SW_HIDE);
		if(m_pHallDlg)
		{
			m_pHallDlg->UpdateUserInfo(dwUserId);
			m_pHallDlg->ShowWindow(SW_SHOW);	
		}
	}
}
// �û����뷿����Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatEnterRoomMessage(DWORD dwRoomId, DWORD dwErrorCode)
{
	if(m_pHallDlg)
		m_pHallDlg->OnEnterRoomNotify(dwRoomId,dwErrorCode);
	// ������뷿��ɹ�������ʾ�����棬���ط����������
	if(dwErrorCode == GV_ERR_SUCCESS)
	{
		if(m_pHallDlg)
			m_pHallDlg->ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
		// �����뽹�㶨λ��������
		GetDlgItem(IDC_EDIT_TEXTINPUT)->SetFocus();
		if(m_pSelfVideoDlg)
		{
			BindUserAndVideoPanel(m_dwSelfUserId,m_pSelfVideoDlg);
		}
		// ����Ϊ����ģʽ
		m_ctrlDispNumCombo.SetCurSel(0);
		RelocateVideoPanel(1);

		// ���������Ϣ������е�����
		m_strTextOutput.Empty();
		m_ctrlTextOutput.SetWindowText("");
		// ���뻶ӭ��Ϣ
		CString strWelcomeMsg;

		CHAR szConfigFile[MAX_PATH] = {0};
		::GetModuleFileName(NULL,szConfigFile,_MAX_PATH);
		(strrchr(szConfigFile,'\\'))[1] = 0; 
		strcat(szConfigFile,DEFAULT_CONFIG_FILENAME);
		
		TCHAR szTempStr[200] = {0};
		// ����Ʒ����
		if(::GetPrivateProfileString(DEFAULT_CONFIG_SECTION,_T("ProductName"),NULL,szTempStr,sizeof(szTempStr),szConfigFile))
			strWelcomeMsg.Format("��ӭʹ��%s�����ѳɹ�����%d�Ż����ң�",szTempStr, dwRoomId);
		else
			strWelcomeMsg.Format("��ӭʹ��AnyChat����Ƶ����ƽ̨�����ѳɹ�����%d�Ż����ң�",dwRoomId);
		InsertStringToTextOutput(strWelcomeMsg);
	}
}
// ���������û���Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatOnlineUserMessage(DWORD dwUserNum, DWORD dwRoomId)
{
	DWORD dwOtherUsersNum = 0;
	// ���֮ǰ����������
	m_ctrlUserList.DeleteAllItems();
	// ���Լ������б��һ�У���Ϊ�Լ�����Ϣ���������û��б��У�
	AddNewUserToList(m_dwSelfUserId);
	
	BRAC_GetOnlineUser(NULL,dwOtherUsersNum);		///< ���Ȼ�ȡ��������������
	if(dwOtherUsersNum)
	{
		LPDWORD lpdwUserList = (LPDWORD)malloc(sizeof(DWORD)*dwOtherUsersNum);
		if(lpdwUserList)
		{
			BRAC_GetOnlineUser(lpdwUserList,dwOtherUsersNum);	///< ������ȡ�����û��б�
			// ����µ��û��б�
			for(int i=0; i<(INT)dwOtherUsersNum; i++)
				AddNewUserToList(lpdwUserList[i]);
			free(lpdwUserList);		// �ͷ���Դ
		}
	}

	// ˢ�������û�����ʾ
	RefreshOnlineUserNum();
	// ˢ�������ʾ
	RefreshVideoPanel();
}

// �û�����/�˳�������Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatUserAtRoomMessage(DWORD dwUserId, BOOL bEnter)
{
	if(bEnter)
	{
		AddNewUserToList(dwUserId);
		// ˢ�������ʾ�����п���λ��ʱ���Զ�������ʾ��壬������Ƶ
		if(GetEmptyVideoPanelIndex() != -1)
			RefreshVideoPanel();
	}
	else
	{
		// �û��뿪���䣬��Ҫ���б���������û��ļ�¼
		INT iItemCount = m_ctrlUserList.GetItemCount();
		for(int i=0; i<iItemCount; i++)
		{
			if(m_ctrlUserList.GetItemData(i) == dwUserId)
			{
				m_ctrlUserList.DeleteItem(i);
				break;
			}
		}
		// �������û���ص����
		BOOL bReleaseEmptyPanel = FALSE;
		for(int k=0; k<MAX_VIDEOPANEL_NUM; k++)
		{
			if(m_pPanelObjectArray[k] && m_pPanelObjectArray[k]->GetUserId() == dwUserId)
			{
				bReleaseEmptyPanel = TRUE;
				CleanVideoPanelBind(m_pPanelObjectArray[k]);
				break;
			}
		}
		// ����ˢ��һ����Ƶ��壬�������������������������û�
		if(bReleaseEmptyPanel)
			RefreshVideoPanel();
	}
	// ˢ�������û�����ʾ
	RefreshOnlineUserNum();

	// ����Log��Ϣ
	CHAR szUserName[50] = {0};
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_NICKNAME,szUserName,sizeof(szUserName));
	CTime tm = CTime::GetCurrentTime();
	CString strMessage;
	strMessage.Format("�û�%s(%d)%s����----(%s)",szUserName,(int)dwUserId,bEnter ? "����" : "�뿪",tm.Format("%H:%M:%S"));
	InsertStringToTextOutput(strMessage);
}
//����Ͽ���Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatLinkCloseMessage(DWORD dwReason, DWORD dwErrorCode)
{
	if(m_pSelfVideoDlg)
	{
		CleanVideoPanelBind(m_pSelfVideoDlg,FALSE);
	}
	for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
	{
		if(m_pPanelObjectArray[i])
		{
			CleanVideoPanelBind(m_pPanelObjectArray[i],FALSE);
		}
	}
	BRAC_LeaveRoom(-1);
	BRAC_Logout();		// ����SDK�ڲ���������
	m_dwSelfUserId = INVALID_USERID;
	

	CHAR szNotify[100] = {0};
	_snprintf(szNotify,sizeof(szNotify),"���������ѶϿ�(�������:%d)\r\n\r\n��ȷ�����ص�¼���棬��ȡ���˳�ϵͳ��",dwErrorCode);
	if(::MessageBox(m_hWnd,szNotify,"��ʾ",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
	{
		// �����������
		if(m_pSettingsDlg)
			m_pSettingsDlg->ShowWindow(SW_HIDE);
		// ���ش�������
		if(m_pHallDlg)
			m_pHallDlg->ShowWindow(SW_HIDE);
		// ���ص�ǰ����
		ShowWindow(SW_HIDE);
		// ��ʾ��¼����
		if(m_pLoginDlg)
			m_pLoginDlg->OnNeedShowLoginDlg();
	}
	else
	{
		OnUserExitSystem();
	}
}
// �û��õ�/�ͷ�mic��Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatMicStateChgMessage(DWORD dwUserId, BOOL bGetMic)
{
	INT iItemCount = m_ctrlUserList.GetItemCount();
	for(int i=0; i<iItemCount; i++)
	{
		if(m_ctrlUserList.GetItemData(i) == dwUserId)
		{
			RefreshListItemState(i,dwUserId);
			break;
		}
	}
}

// �û�����ͷ״̬�ı���Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatCameraStateChgMessage(DWORD dwUserId, DWORD dwState)
{
	INT iItemCount = m_ctrlUserList.GetItemCount();
	for(int i=0; i<iItemCount; i++)
	{
		if(m_ctrlUserList.GetItemData(i) == dwUserId)
		{
			RefreshListItemState(i,dwUserId);
			break;
		}
	}
}
// �û�����ģʽ�ı���Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatChatModeChgMessage(DWORD dwUserId, BOOL bPublicChat)
{

}
// �û��״̬�����仯��Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatActiveStateChgMessage(DWORD dwUserId, DWORD dwState)
{

}
// P2P����״̬�仯��Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatP2PConnectStateMessage(DWORD dwUserId, DWORD dwState)
{

}
/**
 *	�û���Ƶ�ֱ��ʸı���Ϣ
 *	Ӧ�ó���ɸ����û���ԭʼ�ֱ��ʵ�����ʾ��Ƶ����Ĵ�С�����ֻ���ı�����ʧ��
 */
void CBRAnyChatMeetingDlg::OnAnyChatVideoSizeChgMessage(DWORD dwUserId, DWORD dwWidth, DWORD dwHeight)
{
	for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
	{
		if(m_pPanelObjectArray[i])
		{
			if(m_pPanelObjectArray[i]->GetUserId() == dwUserId)
			{
				m_pPanelObjectArray[i]->OnVideoSizeChange(dwWidth, dwHeight);
				break;
			}
		}
	}
}

// �û�˽��������Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatPrivateRequestMessage(DWORD dwUserId, DWORD dwRequestId)
{

}
// �û�˽������ظ���Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatPrivateEchoMessage(DWORD dwUserId, DWORD dwErrorCode)
{

}
// �û��˳�˽����Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatPrivateExitMessage(DWORD dwUserId, DWORD dwErrorCode)
{

}

// SDK������Ϣ
void CBRAnyChatMeetingDlg::OnAnyChatSDKWarningMessage(DWORD dwErrorCode, DWORD dwReserved)
{

}

// ��Ƶ���ݻص�
void CBRAnyChatMeetingDlg::OnAnyChatVideoDataCallBack(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, LPBITMAPINFOHEADER lpbmiHeader)
{
	if(dwUserid == m_dwSelfUserId && m_pSelfVideoDlg)
		m_pSelfVideoDlg->OnRecvCallBackVideoData(lpBuf,dwLen,*lpbmiHeader);
	else
	{
		for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
		{
			if(m_pPanelObjectArray[i])
			{
				if(m_pPanelObjectArray[i]->GetUserId() == dwUserid)
				{
					m_pPanelObjectArray[i]->OnRecvCallBackVideoData(lpBuf,dwLen,*lpbmiHeader);
					break;
				}
			}
		}
	}
}
// ��Ƶ���ݻص�
void CBRAnyChatMeetingDlg::OnAnyChatAudioDataCallBack(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, LPWAVEFORMATEX lpWaveFormatEx)
{

}
// �������ֵĻص�����
void CBRAnyChatMeetingDlg::OnAnyChatTextMsgCallBack(DWORD dwFromUserid, DWORD dwToUserid, BOOL bSecret, LPCTSTR lpMsgBuf, DWORD dwLen)
{
	CString strMessage;

	CHAR szUserName[50] = {0};
	BRAC_QueryUserState(dwFromUserid,BRAC_USERSTATE_NICKNAME,szUserName,sizeof(szUserName));
	CTime tm = CTime::GetCurrentTime();
	strMessage.Format("%s˵��%s (%s)",szUserName,lpMsgBuf,tm.Format("%H:%M:%S"));
	InsertStringToTextOutput(strMessage);
}
// ͸��ͨ���ص�����
void CBRAnyChatMeetingDlg::OnAnyChatTransBufferCallBack(DWORD dwUserid, LPBYTE lpBuf, DWORD dwLen)
{

}
// ͸��ͨ��������չ�ص�����
void CBRAnyChatMeetingDlg::OnAnyChatTransBufferExCallBack(DWORD dwUserid, LPBYTE lpBuf, DWORD dwLen, DWORD wParam, DWORD lParam, DWORD dwTaskId)
{

}
// �ļ�����ص�����
void CBRAnyChatMeetingDlg::OnAnyChatTransFileCallBack(DWORD dwUserid, LPCTSTR lpFileName, LPCTSTR lpTempFilePath, DWORD dwFileLength, DWORD wParam, DWORD lParam, DWORD dwTaskId)
{

}
// �����仯�ص�����
void CBRAnyChatMeetingDlg::OnAnyChatVolumeChangeCallBack(BRAC_AudioDevice device, DWORD dwCurrentVolume)
{
	if(m_pSettingsDlg)
		m_pSettingsDlg->OnSystemVolumeChangeNotify(device,dwCurrentVolume);
}
// SDK Filter ͨ�����ݻص�����
void CBRAnyChatMeetingDlg::OnAnyChatSDKFilterDataCallBack(LPBYTE lpBuf, DWORD dwLen)
{

}
// ¼�񡢿���������ɻص�����
void CBRAnyChatMeetingDlg::OnAnyChatRecordSnapShotCallBack(DWORD dwUserid, LPCTSTR lpFileName, DWORD dwParam, BOOL bRecordType)
{

}
// �첽��Ϣ֪ͨ�ص�����
void CBRAnyChatMeetingDlg::OnAnyChatNotifyMessageCallBack(DWORD dwNotifyMsg, DWORD wParam, DWORD lParam)
{

}
/**
 *	�û��رջ����ң��˳����䣬���ش���
 */
void CBRAnyChatMeetingDlg::OnClose() 
{
	if(m_pSelfVideoDlg)
	{
		CleanVideoPanelBind(m_pSelfVideoDlg,FALSE);
	}
	for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
	{
		if(m_pPanelObjectArray[i])
		{
			CleanVideoPanelBind(m_pPanelObjectArray[i],FALSE);
		}
	}

	BRAC_LeaveRoom(-1);
	// ����������
	ShowWindow(SW_HIDE);
	if(m_pHallDlg)
	{
		m_pHallDlg->ShowWindow(SW_SHOW);
	}

//	CDialog::OnClose();
}

/**
 *	��ʼ�������û��б�ؼ�
 */
void CBRAnyChatMeetingDlg::InitUserListCtrl(void)
{
	DWORD dwStyle = 0;
	dwStyle = LVS_EX_FULLROWSELECT | LVS_SHAREIMAGELISTS | LVS_REPORT | LVS_EX_FLATSB | LVS_EX_SUBITEMIMAGES;
	dwStyle &= ~LVS_EX_GRIDLINES;
	
	m_ctrlUserList.SetExtendedStyle(dwStyle);
	m_ctrlUserList.ModifyStyle(0, LVS_REPORT);
	m_ctrlUserList.ModifyStyle(0, WS_EX_CLIENTEDGE);
	
	m_ctrlUserList.InsertColumn(0,_T("��Ƶ״̬"),LVCFMT_CENTER,20);
	m_ctrlUserList.InsertColumn(1,_T("��Ƶ״̬"),LVCFMT_CENTER,20);
	m_ctrlUserList.InsertColumn(2,_T("�û���"),LVCFMT_LEFT,120);

	m_UserStateImageList.Create(16,16,ILC_COLOR32|ILC_MASK,8,8);
	m_UserStateImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MIC));
	m_UserStateImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAMERA_CLOSE));
	m_UserStateImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAMERA_OPEN));

	m_ctrlUserList.SetImageList(&m_UserStateImageList,LVSIL_SMALL/*LVSIL_NORMAL*/);
}

// ˢ�������û�����ʾ
void CBRAnyChatMeetingDlg::RefreshOnlineUserNum(void)
{
	DWORD dwUserNum;
	BRAC_GetOnlineUser(NULL,dwUserNum);

	CHAR szUserListTitle[50] = {0};
	_snprintf(szUserListTitle,sizeof(szUserListTitle),"�û��б�(%d������)",dwUserNum+1);		// ��Ҫ�����Լ�
	GetDlgItem(IDC_STATIC_USERLIST)->SetWindowText(szUserListTitle);
}
// �����µ��û����б���
void CBRAnyChatMeetingDlg::AddNewUserToList(DWORD dwUserId)
{
	CHAR szUserName[50] = {0};
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_NICKNAME,szUserName,sizeof(szUserName));
	if(dwUserId == m_dwSelfUserId)
		strcat(szUserName,"(�Լ�)");
	
	LVITEM lvi = {0};
	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = m_ctrlUserList.GetItemCount();		// �µ��û����ǲ�����β��
	lvi.iSubItem = 0;
	lvi.iImage = 0;	
	lvi.pszText = "";
	
	INT itemno = m_ctrlUserList.InsertItem(&lvi);
	m_ctrlUserList.SetItemData(itemno,dwUserId);
	m_ctrlUserList.SetItemText(itemno,2,szUserName);
	RefreshListItemState(itemno,dwUserId);
}
/**
 *	ˢ��ָ���б���״̬
 *	�����û��豸״̬�Ĳ�ͬ�����û���ǰ����ʾ��ͬ��ͼ��
 *	@param dwItemNo �����û��б������
 *	@param dwUserId ����Ӧ���û����
 */
void CBRAnyChatMeetingDlg::RefreshListItemState(DWORD dwItemNo, DWORD dwUserId)
{
	DWORD dwMicImage,dwCameraImage;
	// ��ѯ�û��Ƿ��õ�Mic
	DWORD dwMicState;
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_HOLDMIC,(char*)&dwMicState,sizeof(dwMicState));
	dwMicImage = dwMicState ? IMAGE_INDEX_MIC : IMAGE_INDEX_NULL;
	// ��ѯ�û�������ͷ״̬
	DWORD dwCameraState;
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_CAMERA,(char*)&dwCameraState,sizeof(dwCameraState));
	if(dwCameraState == GV_CAMERA_STATE_NULL)
		dwCameraImage = IMAGE_INDEX_NULL;
	else if(dwCameraState == GV_CAMERA_STATE_NORMAL)
		dwCameraImage = IMAGE_INDEX_CAMERA_NORMAL;
	else
		dwCameraImage = IMAGE_INDEX_CAMERA_OPEN;

	m_ctrlUserList.SetItem(dwItemNo,0,LVIF_IMAGE|LVIF_TEXT|LVIF_STATE,NULL,dwCameraImage,NULL,NULL,0);
	m_ctrlUserList.SetItem(dwItemNo,1,LVIF_IMAGE|LVIF_TEXT|LVIF_STATE,NULL,dwMicImage,NULL,NULL,0);
}

/**
 *	��ȡһ�����е���ʾ������
 */
DWORD CBRAnyChatMeetingDlg::GetEmptyVideoPanelIndex(void)
{
	DWORD dwIndex = -1;
	DWORD dwCurrentDispNum = m_ctrlDispNumCombo.GetItemData(m_ctrlDispNumCombo.GetCurSel());
	for(int i=0; i<(int)dwCurrentDispNum && i<MAX_VIDEOPANEL_NUM; i++)
	{
		if(m_pPanelObjectArray[i] && m_pPanelObjectArray[i]->GetUserId() == INVALID_USERID)
		{
			dwIndex = i;
			break;
		}
	}
	return dwIndex;
}

// �ж�ָ���û�����Ƶ����Ƿ�����ʾ����
BOOL CBRAnyChatMeetingDlg::IsUserVideoPanelShow(DWORD dwUserId)
{
	BOOL bAlreadyShow = FALSE;
	for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
	{
		if(m_pPanelObjectArray[i] && m_pPanelObjectArray[i]->GetUserId() == dwUserId)
		{
			bAlreadyShow = TRUE;
			break;
		}
	}
	return bAlreadyShow;
}

// ˢ����Ƶ��ʾ���
void CBRAnyChatMeetingDlg::RefreshVideoPanel(void)
{
	DWORD dwOtherUsersNum = 0;
	BRAC_GetOnlineUser(NULL,dwOtherUsersNum);		///< ���Ȼ�ȡ��������������
	if(dwOtherUsersNum)
	{
		LPDWORD lpdwUserList = (LPDWORD)malloc(sizeof(DWORD)*dwOtherUsersNum);
		if(lpdwUserList)
		{
			BRAC_GetOnlineUser(lpdwUserList,dwOtherUsersNum);	///< ������ȡ�����û��б�
			BOOL bNoneEmptySite = FALSE;
			// ��һ��ѭ����Ϊ����λ���ҵ���Ƶ�򿪵��û�������ʾ����
			for(int i=0; i<(INT)dwOtherUsersNum; i++)
			{
				DWORD dwEmptyIndex = GetEmptyVideoPanelIndex();
				if(dwEmptyIndex == -1)
				{
					bNoneEmptySite = TRUE;
					break;
				}
				DWORD dwUserId = lpdwUserList[i];
				if(IsUserVideoPanelShow(dwUserId))
					continue;
				// ��ѯ�û�������ͷ״̬
				DWORD dwCameraState;
				BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_CAMERA,(char*)&dwCameraState,sizeof(dwCameraState));
				if(dwCameraState == IMAGE_INDEX_CAMERA_OPEN)
				{
					BindUserAndVideoPanel(dwUserId,m_pPanelObjectArray[dwEmptyIndex]);
				}
			}
			// �ڶ���ѭ����Ϊ����λ�ð�˳������û�
			for(int k=0; k<(INT)dwOtherUsersNum && !bNoneEmptySite; k++)
			{
				DWORD dwEmptyIndex = GetEmptyVideoPanelIndex();
				if(dwEmptyIndex == -1)
					break;
				DWORD dwUserId = lpdwUserList[k];
				if(!IsUserVideoPanelShow(dwUserId))
				{
					BindUserAndVideoPanel(dwUserId,m_pPanelObjectArray[dwEmptyIndex]);
				}
			}
			free(lpdwUserList);		// �ͷ���Դ
		}
	}
	else
	{
		// û���������û�������������Ƶ���
		for(int i=0; i<MAX_VIDEOPANEL_NUM; i++)
		{
			if(m_pPanelObjectArray[i])
			{
				CleanVideoPanelBind(m_pPanelObjectArray[i]);
			}
		}
	}
}

// ���ݷ�������λ��Ƶ�����ʾλ��
void CBRAnyChatMeetingDlg::RelocateVideoPanel(DWORD dwSplitNum)
{
	CRect rcPanel[MAX_VIDEOPANEL_NUM] = {0};
	CRect rcClient;
	GetDlgItem(IDC_STATIC_REMOTEVIDEO)->GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	switch(dwSplitNum)
	{
	case 1:
		rcPanel[0] = rcClient;
		break;
	case 2:
		rcPanel[0] = rcClient;	rcPanel[0].right = rcPanel[0].left + rcClient.Width()/2;
		rcPanel[1] = rcClient;	rcPanel[1].left = rcPanel[0].left + rcClient.Width()/2;
		break;
	case 4:
		rcPanel[0] = rcClient;	rcPanel[0].right = rcPanel[0].left + rcClient.Width()/2;	rcPanel[0].bottom = rcPanel[0].top + rcClient.Height()/2;
		rcPanel[1] = rcClient;	rcPanel[1].left = rcPanel[0].left + rcClient.Width()/2;		rcPanel[1].bottom = rcPanel[1].top + rcClient.Height()/2;
		rcPanel[2] = rcClient;	rcPanel[2].right = rcPanel[2].left + rcClient.Width()/2;	rcPanel[2].top = rcPanel[2].top + rcClient.Height()/2;
		rcPanel[3] = rcClient;	rcPanel[3].left = rcPanel[3].left + rcClient.Width()/2;		rcPanel[3].top = rcPanel[3].top + rcClient.Height()/2;
		break;
	default:
		break;
	}
	for (int i=0; i<MAX_VIDEOPANEL_NUM; i++)
	{
		if(i<(int)dwSplitNum)
		{
			m_pPanelObjectArray[i]->MoveWindow(rcPanel[i]);
			m_pPanelObjectArray[i]->ShowWindow(SW_SHOW);
			m_pPanelObjectArray[i]->Invalidate();
		}
		else
		{
			CleanVideoPanelBind(m_pPanelObjectArray[i]);
			m_pPanelObjectArray[i]->ShowWindow(SW_HIDE);
		}
	}
	RefreshVideoPanel();
}

// ���û�����Ƶ���
void CBRAnyChatMeetingDlg::BindUserAndVideoPanel(DWORD dwUserId, CVideoPanelDlg* lpVideoPanelDlg)
{
	ASSERT(lpVideoPanelDlg);
	CHAR szUserName[50] = {0};
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_NICKNAME,szUserName,sizeof(szUserName));
	lpVideoPanelDlg->SetRelateUserId(dwUserId);
	lpVideoPanelDlg->SetPanelTitle(dwUserId == m_dwSelfUserId ? "������Ƶ" : szUserName);
}
// ȡ����Ƶ������û��İ�
void CBRAnyChatMeetingDlg::CleanVideoPanelBind(CVideoPanelDlg* lpVideoPanelDlg, BOOL bErase)
{
	ASSERT(lpVideoPanelDlg);
	lpVideoPanelDlg->SetRelateUserId(INVALID_USERID);
	lpVideoPanelDlg->SetPanelTitle("");
	if(bErase && IsWindowVisible())
		lpVideoPanelDlg->Invalidate();
}

/**
 *	�û�ѡ�������ʾ�����б�
 */
void CBRAnyChatMeetingDlg::OnSelchangeComboDispNum() 
{
	DWORD dwSelectSplitNum = m_ctrlDispNumCombo.GetItemData(m_ctrlDispNumCombo.GetCurSel());
	RelocateVideoPanel(dwSelectSplitNum);
}
/**
 *	�û�˫���û��б����Ҫ����Ӧ���û���Ƶ��ʾ����
 */
void CBRAnyChatMeetingDlg::OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	INT itemno = pNMListView->iItem;
	do 
	{
		if(itemno < 0)
			break;
		DWORD dwUserId = m_ctrlUserList.GetItemData(itemno);
		if(dwUserId == INVALID_USERID || dwUserId == m_dwSelfUserId)
			break;
		if(IsUserVideoPanelShow(dwUserId))
			break;
		DWORD dwEmptySite = GetEmptyVideoPanelIndex();
		if(dwEmptySite == -1)	// û�пյ�λ���ˣ�Ĭ�Ͻ�0��λ�÷�����µ��û�
		{
			dwEmptySite = 0;
			CleanVideoPanelBind(m_pPanelObjectArray[0]);
		}
		BindUserAndVideoPanel(dwUserId,m_pPanelObjectArray[dwEmptySite]);
	} while (FALSE);
	*pResult = 0;
}

BOOL CBRAnyChatMeetingDlg::PreTranslateMessage(MSG* pMsg) 
{
	HWND hTextInputWnd = GetDlgItem(IDC_EDIT_TEXTINPUT)->GetSafeHwnd();
	HWND hTextOutputWnd =GetDlgItem(IDC_EDIT_TEXTOUTPUT)->GetSafeHwnd();

	BOOL bNeedTranslate = TRUE;
	if(pMsg->message == WM_KEYDOWN)
    {
		switch(pMsg->wParam)
        {
        case VK_RETURN:
			if(pMsg->hwnd == hTextInputWnd)
				OnButtonSendText();
			bNeedTranslate = FALSE;
			break;
        case VK_SPACE:
			if(pMsg->hwnd != hTextInputWnd)
				bNeedTranslate = FALSE;
			break;
		case VK_ESCAPE:
			bNeedTranslate = FALSE;
			break;
		default:
			break;
        }
	}
	else if(pMsg->message == WM_RBUTTONDOWN)
	{
		bNeedTranslate = FALSE;		// ��ʱ����Ӧ�Ҽ���Ϣ����ΪƤ�����ԭ�򣬻ᵼ�µ������Ĳ˵�������
	}
	
	return bNeedTranslate ? CDialog::PreTranslateMessage(pMsg) : TRUE;
}
/**
 *	�û����·���������Ϣ��ť
 */
void CBRAnyChatMeetingDlg::OnButtonSendText() 
{
	CString strInput,strMessage;
	GetDlgItem(IDC_EDIT_TEXTINPUT)->GetWindowText(strInput);
	if(!strInput.IsEmpty())
	{
		BRAC_SendTextMessage(-1,FALSE,strInput.GetBuffer(0),strInput.GetLength());
		
		CTime tm = CTime::GetCurrentTime();
		strMessage.Format("��˵��%s (%s)",strInput,tm.Format("%H:%M:%S"));
		InsertStringToTextOutput(strMessage);
	}
	GetDlgItem(IDC_EDIT_TEXTINPUT)->SetWindowText("");
	GetDlgItem(IDC_EDIT_TEXTINPUT)->SetFocus();
}

// ��������Ϣ����������Ϣ
void CBRAnyChatMeetingDlg::InsertStringToTextOutput(CString strMsg)
{
	m_strTextOutput += (strMsg + "\r\n");
	m_ctrlTextOutput.SetWindowText(m_strTextOutput);
	m_ctrlTextOutput.LineScroll(m_ctrlTextOutput.GetLineCount());
}
/**
 *	�û����¸߼����ð�ť����ʾ���ý���
 */
void CBRAnyChatMeetingDlg::OnButtonAdvSet() 
{
	if(!m_pSettingsDlg)
	{
		m_pSettingsDlg = new CSettingsDlg(this);
		m_pSettingsDlg->Create(IDD_DIALOG_SETTINGS,this);
	}
	m_pSettingsDlg->ShowWindow(SW_SHOW);
	m_pSettingsDlg->CenterWindow();
}

void CBRAnyChatMeetingDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	OnButtonAdvSet();
	PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}
