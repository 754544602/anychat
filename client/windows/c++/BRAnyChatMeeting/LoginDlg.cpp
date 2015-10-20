// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BRAnyChatMeeting.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SETTINGS_AREA_HEIGHT	40		// ��������߶�


CLoginDlg::CLoginDlg(CWnd* pParent)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_pMainDlg = (CBRAnyChatMeetingDlg*)pParent;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, OnButtonSettings)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	TCHAR szBannerPath[MAX_PATH] = {0};
	TCHAR szBannerFile[MAX_PATH] = {0};
	GetModuleFileName(NULL,szBannerPath,sizeof(szBannerPath)); 
	(strrchr(szBannerPath,'\\'))[1] = 0; 
	strcat(szBannerPath,SUB_PATH_SKINFILE);
	_snprintf(szBannerFile,sizeof(szBannerFile),"%s\\%s",szBannerPath,_T("banner.jpg"));
	if(IsFileExist(szBannerFile))
		m_BannerPic.Load(szBannerFile);
	else
		m_BannerPic.Load(IDR_IMAGE_LOGINTOP);
	
	m_bkBrush.CreateSolidBrush(RGB(228,244,255));
	m_bottomBrush.CreateSolidBrush(RGB(194,227,250));
	m_settingsBrush.CreateSolidBrush(RGB(228,244,255));

	m_bSettingMode = FALSE;
	// ��С���ڽ��棬����������
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.bottom -= 20;
	MoveWindow(rcClient);
/*
#ifdef _HRC_USE_SKIN_
	// ���ʹ��Ƥ��������Ҫ����ťλ������һ��
	CRect rcSettings;
	GetDlgItem(IDC_BUTTON_SETTINGS)->GetWindowRect(rcSettings);
	ScreenToClient(rcSettings);
	rcSettings.bottom += 8;	rcSettings.top += 8;
	GetDlgItem(IDC_BUTTON_SETTINGS)->MoveWindow(rcSettings);

	CRect rcLogin;
	GetDlgItem(IDC_BUTTON_LOGIN)->GetWindowRect(rcLogin);
	ScreenToClient(rcLogin);
	rcLogin.bottom += 8;	rcLogin.top += 8;
	GetDlgItem(IDC_BUTTON_LOGIN)->MoveWindow(rcLogin);
#endif //_HRC_USE_SKIN_
*/
	LoadSettingsParam();

	GetDlgItem(IDC_STATIC_NOTIFY)->SetWindowText("");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_bkBrush.DeleteObject();
	m_bottomBrush.DeleteObject();
	m_settingsBrush.DeleteObject();
	m_BannerPic.Free();

	m_pMainDlg = NULL;
}

// �������ò���
void CLoginDlg::LoadSettingsParam(void)
{
	CHAR szConfigFile[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szConfigFile,_MAX_PATH);
	(strrchr(szConfigFile,'\\'))[1] = 0; 
	strcat(szConfigFile,DEFAULT_CONFIG_FILENAME);
	
	TCHAR szTempStr[100] = {0};
	// ��������IP��ַ����
	if(::GetPrivateProfileString(DEFAULT_CONFIG_SECTION,_T("ServerIPAddr"),NULL,szTempStr,sizeof(szTempStr),szConfigFile))
		GetDlgItem(IDC_SERVER_IPADDR)->SetWindowText(szTempStr);
	else
		GetDlgItem(IDC_SERVER_IPADDR)->SetWindowText(DEFAULT_SERVER_IPADDR);
	// ���������˿�����
	DWORD dwPort = ::GetPrivateProfileInt(DEFAULT_CONFIG_SECTION,_T("ServerPort"),DEFAULT_SERVER_PORT,szConfigFile);
	_snprintf(szTempStr,sizeof(szTempStr),"%d",dwPort);
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(szTempStr);
	
	// ����¼�û�������
	if(::GetPrivateProfileString(DEFAULT_CONFIG_SECTION,_T("LoginName"),NULL,szTempStr,sizeof(szTempStr),szConfigFile))
		GetDlgItem(IDC_EDIT_USERNAME)->SetWindowText(szTempStr);

	// ����Ʒ����
	if(::GetPrivateProfileString(DEFAULT_CONFIG_SECTION,_T("ProductName"),NULL,szTempStr,sizeof(szTempStr),szConfigFile))
	{
		strcat(szTempStr, " �� �û���¼");
		SetWindowText(szTempStr);
	}
}

void CLoginDlg::OnClose() 
{
	ASSERT(m_pMainDlg);
	m_pMainDlg->OnUserExitSystem();
//	CDialog::OnClose();
}

BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC) 
{
	BOOL rc = CDialog::OnEraseBkgnd(pDC);
	
	return rc;
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkMode (TRANSPARENT);

	return m_bkBrush;
}

void CLoginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(rcClient);
	
//	dc.FillRect(rcClient,&m_bkBrush);
	
	// ���Ƶ�¼���涥��ͼƬ
	CRect rcBanner;
	rcBanner.left = rcClient.left;		rcBanner.top = rcClient.top;
	rcBanner.right = rcClient.right;	rcBanner.bottom = rcBanner.top + m_BannerPic.GetImageSize().cy;
	m_BannerPic.Render(&dc,rcBanner);

	// ����¼��ť�ܱ߱�����ɫ
	CRect rcBottom = rcClient;
	if(m_bSettingMode)
		rcBottom.bottom -= SETTINGS_AREA_HEIGHT;
	rcBottom.top = rcBottom.bottom - 35;
	dc.FillRect(rcBottom,&m_bottomBrush);

	// ����������򱳾���ɫ
	if(m_bSettingMode)
	{
		CRect rcSettingArea = rcClient;
		rcSettingArea.top = rcSettingArea.bottom - SETTINGS_AREA_HEIGHT;
		dc.FillRect(rcSettingArea,&m_settingsBrush);
	}

	// Do not call CDialog::OnPaint() for painting messages
}

void CLoginDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
	CDialog::OnLButtonDown(nFlags, point);
}
/**
 *	�û��������ð�ť����ʾ��������������
 */
void CLoginDlg::OnButtonSettings() 
{
	m_bSettingMode = !m_bSettingMode;

	CRect rcClient;
	GetWindowRect(rcClient);

	DWORD dwHeight = m_bSettingMode ? rcClient.Height() + SETTINGS_AREA_HEIGHT : rcClient.Height() - SETTINGS_AREA_HEIGHT;
	::SetWindowPos(m_hWnd,NULL, 0,0,rcClient.Width(), dwHeight,SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_BUTTON_SETTINGS)->SetWindowText(m_bSettingMode ? "����" : "����");
}

void CLoginDlg::OnButtonLogin() 
{
	CString strIPAddr,strPort,strUserName,strPassword;
	DWORD dwPort;
	GetDlgItem(IDC_SERVER_IPADDR)->GetWindowText(strIPAddr);
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strPort);
	GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(strUserName);
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(strPassword);
	if(strUserName.IsEmpty())
	{
		AfxMessageBox("�������û������οͷ�ʽ��¼�ɲ������룩��");
		GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();
		return;
	}
	dwPort = atoi(strPort.GetBuffer(0));
	// ���ӷ�����
	BRAC_Connect(strIPAddr.GetBuffer(0),dwPort);
	// ��¼ϵͳ
	BRAC_Login(strUserName.GetBuffer(0),strPassword.GetBuffer(0),0);
}

// ����������Ϣ֪ͨ
void CLoginDlg::OnConnectNotify(DWORD dwErrorCode)
{
	if(dwErrorCode != GV_ERR_SUCCESS)
	{
		CHAR szNotify[100] = {0};
		_snprintf(szNotify,sizeof(szNotify),"���ӷ�����ʧ��(%d)!",dwErrorCode);
		GetDlgItem(IDC_STATIC_NOTIFY)->SetWindowText(szNotify);
	}
}
// �û���¼��Ϣ֪ͨ
void CLoginDlg::OnLoginNotify(DWORD dwErrorCode)
{
	if(dwErrorCode != GV_ERR_SUCCESS)
	{
		CHAR szNotify[100] = {0};
		_snprintf(szNotify,sizeof(szNotify),"��¼ʧ��(%d)!",dwErrorCode);
		GetDlgItem(IDC_STATIC_NOTIFY)->SetWindowText(szNotify);
	}
	else		// �����¼�ɹ����򱣴�IP��ַ���˿ں��Լ���¼�û�������Ϣ�������ļ���
	{
		CString strIPAddr,strPort,strUserName;
		GetDlgItem(IDC_SERVER_IPADDR)->GetWindowText(strIPAddr);
		GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strPort);
		GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(strUserName);


		CHAR szConfigFile[MAX_PATH] = {0};
		::GetModuleFileName(NULL,szConfigFile,_MAX_PATH);
		(strrchr(szConfigFile,'\\'))[1] = 0; 
		strcat(szConfigFile,DEFAULT_CONFIG_FILENAME);

		::WritePrivateProfileString(DEFAULT_CONFIG_SECTION,_T("ServerIPAddr"),strIPAddr,szConfigFile);
		::WritePrivateProfileString(DEFAULT_CONFIG_SECTION,_T("ServerPort"),strPort,szConfigFile);
		::WritePrivateProfileString(DEFAULT_CONFIG_SECTION,_T("LoginName"),strUserName,szConfigFile);
	}
}

// ���ӶϿ������ص�¼����
void CLoginDlg::OnNeedShowLoginDlg(void)
{
	// �����ʾ��Ϣ
	GetDlgItem(IDC_STATIC_NOTIFY)->SetWindowText("");
	// Ĭ�ϻָ������������ģʽ
	if(m_bSettingMode)
		OnButtonSettings();
	ShowWindow(SW_SHOW);
}