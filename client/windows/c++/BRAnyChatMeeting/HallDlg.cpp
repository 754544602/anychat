// HallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BRAnyChatMeeting.h"
#include "HallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHallDlg dialog


CHallDlg::CHallDlg(CWnd* pParent)
	: CDialog(CHallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHallDlg)
	m_pMainDlg = (CBRAnyChatMeetingDlg*)pParent;
	m_bWaitEnterRoom = FALSE;
	m_iRoomId = 0;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CHallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHallDlg)
	DDX_Control(pDX, IDC_LIST_ROOM, m_ctrlRoomList);
	DDX_Text(pDX, IDC_EDIT_ROOMID, m_iRoomId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHallDlg, CDialog)
	//{{AFX_MSG_MAP(CHallDlg)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ROOM, OnDblclkListRoom)
	ON_BN_CLICKED(IDC_BUTTON_ENTERROOM, OnButtonEnterRoom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHallDlg message handlers

BOOL CHallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitRoomListCtrl();
	FillRoomInfoToList();

	CHAR szConfigFile[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szConfigFile,_MAX_PATH);
	(strrchr(szConfigFile,'\\'))[1] = 0; 
	strcat(szConfigFile,DEFAULT_CONFIG_FILENAME);

	TCHAR szTempStr[200] = {0};
	// ����Ʒ����
	if(::GetPrivateProfileString(DEFAULT_CONFIG_SECTION,_T("ProductName"),NULL,szTempStr,sizeof(szTempStr),szConfigFile))
	{
		strcat(szTempStr, " �� ����");
		SetWindowText(szTempStr);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHallDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/**
 *	��ʼ����������б�
 */
void CHallDlg::InitRoomListCtrl(void)
{
	DWORD dwStyle = 0;
	dwStyle = LVS_EX_FULLROWSELECT | LVS_SHAREIMAGELISTS | LVS_REPORT|LVS_EX_FLATSB;
	
	m_ctrlRoomList.SetExtendedStyle(dwStyle);
	m_ctrlRoomList.ModifyStyle(0, LVS_REPORT);
	m_ctrlRoomList.ModifyStyle(0, WS_EX_CLIENTEDGE);
	
	m_ctrlRoomList.InsertColumn(0,_T("ID"),LVCFMT_CENTER,50);
	m_ctrlRoomList.InsertColumn(1,_T("������"),LVCFMT_CENTER,140);
	m_ctrlRoomList.InsertColumn(2,_T("��������"),LVCFMT_CENTER,80);
	m_ctrlRoomList.InsertColumn(3,_T("����״̬"),LVCFMT_CENTER,80);
}

/**
 *	������������Ϣ
 *	AnyChat SDK����û���ṩ��ȡ������Ϣ����ؽӿڣ�ʵ��ʹ�ù����У�����ͨ����SDK Filter Plus + �Զ��建��������ģʽ��SDK Filter��������Ϣ����Ӧ�ó���
 *	������������Ҳ����ͨ��SDK Filter��̬�ĸ��¸�Ӧ�ó���
 */
void CHallDlg::FillRoomInfoToList(void)
{
	for (int i=0; i<MAX_DEMO_ROOM_NUM; i++)		// ���Ϊ�˼򵥣�ֱ�����̶��ķ�����Ϣ
	{
		DWORD dwRoomId = i+1;
		CHAR szRoomId[10] = {0};
		_snprintf(szRoomId,sizeof(szRoomId),"%d",dwRoomId);

		LVITEM lvi;
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
		lvi.iItem = dwRoomId;
		lvi.iSubItem = 0;
		lvi.iImage = 0;	
		lvi.pszText = szRoomId;
		
		INT itemno = m_ctrlRoomList.InsertItem(&lvi);
		m_ctrlRoomList.SetItemData(itemno,dwRoomId);
		
		CHAR szRoomName[20] = {0};
		_snprintf(szRoomName,sizeof(szRoomName),"%d#��ʾ����",dwRoomId);
		m_ctrlRoomList.SetItemText(itemno,1,szRoomName);
		m_ctrlRoomList.SetItemText(itemno,2,"δ֪");		// ��ͨ��SDK Filter Plus�����ʵ�ַ������������Ķ�̬����
		m_ctrlRoomList.SetItemText(itemno,3,"����");
	}
}

void CHallDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
	CDialog::OnLButtonDown(nFlags, point);
}

void CHallDlg::OnClose() 
{
	if(m_pMainDlg)
		m_pMainDlg->OnUserExitSystem();
//	CDialog::OnClose();
}

void CHallDlg::OnButtonExit() 
{
	if(m_pMainDlg)
		m_pMainDlg->OnUserExitSystem();
}

/**
 *	���½����û���Ϣ
 *	@param dwUserId �Լ����û����
 */
void CHallDlg::UpdateUserInfo(DWORD dwUserId)
{
	CHAR szUserId[10] = {0};
	_snprintf(szUserId,sizeof(szUserId),"%d",(INT)dwUserId);
	GetDlgItem(IDC_STATIC_USERID)->SetWindowText(szUserId);

	CHAR szNickName[100] = {0};
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_NICKNAME,szNickName,sizeof(szNickName));
	GetDlgItem(IDC_STATIC_NICKNAME)->SetWindowText(szNickName);

	DWORD dwLevel = 0;
	CHAR szLevel[10] = {0};
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_LEVEL,(PCHAR)&dwLevel,sizeof(dwLevel));
	_snprintf(szLevel,sizeof(szLevel),"%d",(INT)dwLevel);
	GetDlgItem(IDC_STATIC_USERLEVEL)->SetWindowText(szLevel);

	CHAR szInternetIP[100] = {0};
	BRAC_QueryUserState(dwUserId,BRAC_USERSTATE_INTERNETIP,szInternetIP,sizeof(szInternetIP));
	GetDlgItem(IDC_STATIC_INTERNETIP)->SetWindowText(szInternetIP);
}
/**
 *	�û�˫����������������뷿��
 */
void CHallDlg::OnDblclkListRoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	INT itemno = pNMListView->iItem;
	if(itemno >= 0 && !m_bWaitEnterRoom)
	{
		m_bWaitEnterRoom = TRUE;
		DWORD dwRoomId = m_ctrlRoomList.GetItemData(itemno);
		BRAC_EnterRoom(dwRoomId,"",0);
	}
	*pResult = 0;
}

// ���뷿��״̬֪ͨ
void CHallDlg::OnEnterRoomNotify(DWORD dwRoomId, DWORD dwErrorCode)
{
	m_bWaitEnterRoom = FALSE;	// ���۽��뷿���Ƿ�ɹ����ȴ�״̬�Ѿ�����
	// ������뷿��ʧ�ܣ�����ʾ�û�
	if(dwErrorCode != GV_ERR_SUCCESS)
	{
		CHAR szNotify[100] = {0};
		_snprintf(szNotify,sizeof(szNotify),"���뷿��%dʧ��(�������:%d)!",dwRoomId,dwErrorCode);
		GetDlgItem(IDC_STATIC_NOTIFY)->SetWindowText(szNotify);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NOTIFY)->SetWindowText("(��ܰ��ʾ��˫�����������뷿��)");
	}
}
/**
 *	�û�������뷿�䰴ť�������Զ���ķ���
 */
void CHallDlg::OnButtonEnterRoom() 
{
	UpdateData(TRUE);
	if(m_iRoomId == 0)
		return;
	BRAC_EnterRoom(m_iRoomId,"",0);
}
