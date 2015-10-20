// VideoPanelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BRAnyChatMeeting.h"
#include "VideoPanelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_REFRESH_SPEAKVOLUME	1		///< ���·���������ʾ��ʱ��
#define TIMER_REFRESH_NETSTATUS		2		///< ˢ������״̬��ʱ��

#define WM_REFRESH_BUTTON_STATE	WM_USER + 100	///< ˢ�°�ť״̬��Ϣ
#define WM_REFRESH_VIDEO		WM_USER + 101	///< ˢ����Ƶ��Ϣ


CVideoPanelDlg::CVideoPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoPanelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoPanelDlg)
	//}}AFX_DATA_INIT
	m_dwUserId = INVALID_USERID;
}


void CVideoPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoPanelDlg)
	DDX_Control(pDX, IDC_BUTTON_VIDEOCTRL, m_btnVideoCtrl);
	DDX_Control(pDX, IDC_BUTTON_SNAPSHOT, m_btnSnapshot);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_btnRecord);
	DDX_Control(pDX, IDC_BUTTON_AUDIOCTRL, m_btnAudioCtrl);
	DDX_Control(pDX, IDC_PROGRESS_AUDIOVOLUME, m_ctrlAudioVolume);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVideoPanelDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoPanelDlg)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_VIDEOCTRL, OnButtonVideoControl)
	ON_BN_CLICKED(IDC_BUTTON_AUDIOCTRL, OnButtonAudioControl)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, OnButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_MESSAGE(WM_REFRESH_BUTTON_STATE,OnRefreshButtonState)
	ON_MESSAGE(WM_REFRESH_VIDEO,OnRefreshVideo)
	ON_MESSAGE(WM_EXIT_FULLSCREEN, OnExitFullScreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoPanelDlg message handlers

BOOL CVideoPanelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	::InitializeCriticalSection(&m_hBufSection);
	m_pLastVideoBuf = NULL;
	m_dwLastVideoLen = 0;

	m_dwWidth = 320;
	m_dwHeight = 240;

	m_pFullScreenDlg = new CFullScreenDlg(this);
	if(m_pFullScreenDlg && m_pFullScreenDlg->Create(IDD_DIALOG_FULLSCREEN, this))
	{
		m_pFullScreenDlg->ShowWindow(SW_HIDE);
		m_pFullScreenDlg->SetNotifyWnd(m_hWnd);
	}
	else
	{
		delete m_pFullScreenDlg;
		m_pFullScreenDlg = NULL;
	}

#ifdef _HRC_USE_SKIN_
	skinppSetNoSkinHwnd(m_hWnd,FALSE);		// bChildNoSkinΪTRUE����Debug�汾�£����ж��Դ���Ӧ����skin��bug
	skinppSetDialogEraseBkgnd(m_ctrlAudioVolume.GetSafeHwnd(),TRUE);
#endif
	
	m_bVideoState = FALSE;
	m_bAudioState = FALSE;

	m_vfTopLeftPic.Load(IDR_IMAGE_VIDEOFRAME_TOPLEFT);
	m_vfTopMiddlePic.Load(IDR_IMAGE_VIDEOFRAME_TOPMIDDLE);
	m_vfTopRightPic.Load(IDR_IMAGE_VIDEOFRAME_TOPRIGHT);
	m_vfLeftPic.Load(IDR_IMAGE_VIDEOFRAME_LEFT);
	m_vfRightPic.Load(IDR_IMAGE_VIDEOFRAME_RIGHT);
	m_vfBottomLeftPic.Load(IDR_IMAGE_VIDEOFRAME_BOTTOMLEFT);
	m_vfBottomMiddlePic.Load(IDR_IMAGE_VIDEOFRAME_BOTTOMMIDDLE);
	m_vfBottomRightPic.Load(IDR_IMAGE_VIDEOFRAME_BOTTOMRIGHT);

	m_NetStatusPic.Load(IDR_IMAGE_NETSTATUS_VERYGOOD);
	m_bkBrush.CreateSolidBrush(RGB(0,0,0));
	m_bNeedShowBkPic = TRUE;
	m_bRegualShowVideo = TRUE;
	GetClientRect(m_rcMainDisp);


	TCHAR szVideoBkPath[MAX_PATH] = {0};
	TCHAR szVideoBkFile[MAX_PATH] = {0};
	GetModuleFileName(NULL,szVideoBkPath,sizeof(szVideoBkPath)); 
	(strrchr(szVideoBkPath,'\\'))[1] = 0; 
	strcat(szVideoBkPath,SUB_PATH_SKINFILE);
	_snprintf(szVideoBkFile,sizeof(szVideoBkFile),"%s\\%s",szVideoBkPath,_T("videobk.jpg"));
	if(IsFileExist(szVideoBkFile))
		m_VideoBkPic.Load(szVideoBkFile);
	else
		m_VideoBkPic.Load(IDR_IMAGE_VIDEOBK);


	// ��Ƶ���ư�ť
	InitSTButton(m_btnVideoCtrl,IDB_BITMAP_OPENVIDEO,"����Ƶ");
	// �������ư�ť
	InitSTButton(m_btnAudioCtrl,IDB_BITMAP_OPENAUDIO,"������");
	// ͼ��ץ�İ�ť
	InitSTButton(m_btnSnapshot,IDB_BITMAP_SNAPSHOT,"����");
	// ¼��ť
//	InitSTButton(m_btnRecord,IDB_BITMAP_RECORDFILE,"¼��");
	m_btnRecord.ShowWindow(SW_HIDE);

	// ��ʼ��������ʾ���ؼ�
	m_ctrlAudioVolume.SetStartColor(RGB(0,255,0));
	m_ctrlAudioVolume.SetEndColor(RGB(255,0,0));
	m_ctrlAudioVolume.SetBkColor(RGB(255,255,255));
	m_ctrlAudioVolume.SetTextColor(RGB(255,255,0));
	m_ctrlAudioVolume.ShowPercent(FALSE);
	
	SetTimer(TIMER_REFRESH_SPEAKVOLUME,100,NULL);
	SetTimer(TIMER_REFRESH_NETSTATUS,2000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVideoPanelDlg::OnDestroy() 
{
	if(m_pLastVideoBuf)
	{
		free(m_pLastVideoBuf);
		m_pLastVideoBuf = NULL;
	}
	m_dwLastVideoLen = 0;

	if(m_pFullScreenDlg)
	{
		m_pFullScreenDlg->DestroyWindow();
		delete m_pFullScreenDlg;
		m_pFullScreenDlg = NULL;
	}

	::DeleteCriticalSection(&m_hBufSection);
	CDialog::OnDestroy();
}

HBRUSH CVideoPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	

	return hbr;
}

void CVideoPanelDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RelocateControls();
}

void CVideoPanelDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// ������Ϣˢ����Ƶ
//	PostMessage(WM_REFRESH_VIDEO,0,0);

	if(m_dwUserId != INVALID_USERID)
		BRAC_RepaintVideo(m_dwUserId,dc.GetSafeHdc());
}

BOOL CVideoPanelDlg::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bReturnValue = CDialog::OnEraseBkgnd(pDC);
	
	CRect rc;
	GetClientRect(rc);
	
	// �����Ͻ�
	m_vfTopLeftPic.Render(pDC,CRect(0,0,m_vfTopLeftPic.GetImageSize().cx,m_vfTopLeftPic.GetImageSize().cy));
	// ���ϲ��м�
	m_vfTopMiddlePic.Render(pDC,CRect(m_vfTopLeftPic.GetImageSize().cx,0,rc.right-m_vfTopRightPic.GetImageSize().cx,m_vfTopMiddlePic.GetImageSize().cy));
	// �����Ͻ�
	m_vfTopRightPic.Render(pDC,CRect(rc.right-m_vfTopRightPic.GetImageSize().cx,0,rc.right,m_vfTopRightPic.GetImageSize().cy));
	// ����߿�
	m_vfLeftPic.Render(pDC,CRect(0,m_vfTopLeftPic.GetImageSize().cy,m_vfLeftPic.GetImageSize().cx,rc.bottom-m_vfBottomLeftPic.GetImageSize().cy));
	// ���ұ߿�
	m_vfRightPic.Render(pDC,CRect(rc.right-m_vfRightPic.GetImageSize().cx,m_vfTopRightPic.GetImageSize().cy,rc.right,rc.bottom-m_vfBottomRightPic.GetImageSize().cy));
	// �����½�
	m_vfBottomLeftPic.Render(pDC,CRect(0,rc.bottom-m_vfBottomLeftPic.GetImageSize().cy,m_vfBottomLeftPic.GetImageSize().cx,rc.bottom));
	// ���ײ��м�
	m_vfBottomMiddlePic.Render(pDC,CRect(m_vfBottomLeftPic.GetImageSize().cx,rc.bottom-m_vfBottomLeftPic.GetImageSize().cy,rc.right-m_vfBottomRightPic.GetImageSize().cx,rc.bottom));
	// �����½�
	m_vfBottomRightPic.Render(pDC,CRect(rc.right-m_vfBottomRightPic.GetImageSize().cx,rc.bottom-m_vfBottomRightPic.GetImageSize().cy,rc.right,rc.bottom));
	
	// ������״̬
	if(m_dwUserId != INVALID_USERID)
		m_NetStatusPic.Render(pDC,m_rcNetStatus);

	// ����Ƶ����
	if(m_bNeedShowBkPic)
		m_VideoBkPic.Render(pDC,m_rcMainDisp);
	else
		pDC->FillRect(m_rcMainDisp,&m_bkBrush);
	
	// ���Ʊ���
	if(!m_strTitle.IsEmpty())
	{
		CMemDC dc(pDC, NULL,TRUE);
		dc.SetTextColor(RGB(255,255,255));
		dc.SetBkMode(TRANSPARENT);
		
		LOGFONT mLogFont = {0};
		mLogFont.lfHeight      = -12;
		mLogFont.lfWeight      = FW_NORMAL;
		mLogFont.lfQuality        = DRAFT_QUALITY;
		mLogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		strcpy(mLogFont.lfFaceName, _T("������"));
		
		HFONT mNewFont = CreateFontIndirect(&mLogFont);
		HFONT mOldFont = (HFONT)SelectObject(dc.m_hDC, mNewFont);
		
		SIZE titleStrSize = {0};
		GetTextExtentPoint32(dc.m_hDC, m_strTitle, strlen(m_strTitle.GetBuffer(0)), &titleStrSize);
		INT x = m_rcTitle.left + ((m_rcTitle.right - m_rcTitle.left) - titleStrSize.cx)/2;
		INT y = m_rcTitle.top + ((m_rcTitle.bottom - m_rcTitle.top) - titleStrSize.cy )/2;
		dc.TextOut(x,y,m_strTitle);
		
		SelectObject(dc.m_hDC, mOldFont);
		DeleteObject(mNewFont);
	}
	
	m_btnVideoCtrl.SetBk(pDC);
	m_btnAudioCtrl.SetBk(pDC);
	m_btnSnapshot.SetBk(pDC);
	m_btnRecord.SetBk(pDC);

	if(m_dwUserId != INVALID_USERID)
		BRAC_RepaintVideo(m_dwUserId, pDC->GetSafeHdc());
	
	return bReturnValue;
}

/**
 *	���¶�λ����ؼ�
 */
void CVideoPanelDlg::RelocateControls()
{
	CRect rc;
	GetClientRect(rc);

	// ���¶�λ������ʾ�ؼ��������14�����أ����ұ�14�����أ��ඥ��6������
	INT titleheight = 12;		///< ����ĸ߶�
	SetRect(m_rcTitle,rc.left+15 , rc.top+6 , rc.right-14 , rc.top+6+titleheight);

	// ���¶�λ������ʾ�ؼ�
	INT volumeheight = 4;		///< ������ʾ����ĸ߶�
	CRect rcVolume(rc.left+ 8, rc.bottom-5-volumeheight , rc.right-8 , rc.bottom-5);
	if(m_ctrlAudioVolume.m_hWnd)
		m_ctrlAudioVolume.MoveWindow(rcVolume);

	// ���¶�λ����״̬ͼ����ʾ����
	SetRect(m_rcNetStatus,rc.right-4-m_NetStatusPic.GetImageSize().cx,rc.bottom-12-m_NetStatusPic.GetImageSize().cy,rc.right-4,rc.bottom-12);

	// ���¼�����Ƶ��ʾ����
	SetRect(m_rcMainDisp,m_vfLeftPic.GetImageSize().cx,m_vfTopLeftPic.GetImageSize().cy,
		rc.right- m_vfRightPic.GetImageSize().cx,rc.bottom-m_vfBottomLeftPic.GetImageSize().cy);
	m_rcVideoDisp = m_rcMainDisp;
	if(m_bRegualShowVideo)
		AdjustVideoDispRect(m_rcVideoDisp);

	// ���¶�λ��Ƶ�������ؼ�
#define BUTTON_LEFTSIDE_VALUE	15			// ��ť����߽���
#define BUTTON_INTERVAL_VALUE	5			// ��ť֮��ļ��
#define BUTTON_WIDTH_VALUE		20
	DWORD dwIndex = 0;
	CRect btnrc;
	btnrc.bottom = rc.bottom - 10;
	btnrc.top = btnrc.bottom - BUTTON_WIDTH_VALUE;
	// ��λ��Ƶ���ư�ť
	btnrc.left = rc.left + BUTTON_LEFTSIDE_VALUE + (BUTTON_INTERVAL_VALUE + BUTTON_WIDTH_VALUE)*(dwIndex++);
	btnrc.right = btnrc.left + BUTTON_WIDTH_VALUE;
	if(m_btnVideoCtrl.m_hWnd)
		m_btnVideoCtrl.MoveWindow(btnrc);
	// ��λ��Ƶ���ư�ť
	btnrc.left = rc.left + BUTTON_LEFTSIDE_VALUE + (BUTTON_INTERVAL_VALUE + BUTTON_WIDTH_VALUE)*(dwIndex++);
	btnrc.right = btnrc.left + BUTTON_WIDTH_VALUE;
	if(m_btnAudioCtrl.m_hWnd)
		m_btnAudioCtrl.MoveWindow(btnrc);
	// ��λ���տ��ư�ť
	btnrc.left = rc.left + BUTTON_LEFTSIDE_VALUE + (BUTTON_INTERVAL_VALUE + BUTTON_WIDTH_VALUE)*(dwIndex++);
	btnrc.right = btnrc.left + BUTTON_WIDTH_VALUE;
	if(m_btnSnapshot.m_hWnd)
		m_btnSnapshot.MoveWindow(btnrc);
	// ��λ¼����ư�ť
	btnrc.left = rc.left + BUTTON_LEFTSIDE_VALUE + (BUTTON_INTERVAL_VALUE + BUTTON_WIDTH_VALUE)*(dwIndex++);
	btnrc.right = btnrc.left + BUTTON_WIDTH_VALUE;
	if(m_btnRecord.m_hWnd)
		m_btnRecord.MoveWindow(btnrc);

	if(m_dwUserId != INVALID_USERID)
		BRAC_SetVideoPos(m_dwUserId,m_hWnd,m_rcVideoDisp.left,m_rcVideoDisp.top,m_rcVideoDisp.right,m_rcVideoDisp.bottom);
}

// ���ñ�����ʾ
void CVideoPanelDlg::SetPanelTitle(CString strTitle)
{
	m_strTitle = strTitle;
//	InvalidateRect(m_rcTitle);	�ⲿ����ˢ��
}
// ��ʼ����ť
void CVideoPanelDlg::InitSTButton(CButtonST& btn, INT bitmapID, LPCTSTR lpTooltip)
{
	btn.SetBitmaps(bitmapID,RGB(255, 0, 255));
	btn.SetWindowText("");
	btn.EnableBalloonTooltip();
	btn.SetTooltipText(lpTooltip);
	btn.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	btn.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
	btn.DrawBorder(FALSE, FALSE);
	btn.DrawTransparent();
	btn.SizeToContent();
}

// ���ù����û����
void CVideoPanelDlg::SetRelateUserId(DWORD dwUserId)
{
	if(dwUserId != INVALID_USERID && m_dwUserId == INVALID_USERID)	// ���¹������µ��û�
	{
		// Ĭ�ϴ���Ƶ����Ƶ
		BRAC_UserSpeakControl(dwUserId,TRUE);
		BRAC_UserCameraControl(dwUserId,TRUE);
		BRAC_SetVideoPos(dwUserId,m_hWnd,m_rcVideoDisp.left,m_rcVideoDisp.top,m_rcVideoDisp.right,m_rcVideoDisp.bottom);

		m_bVideoState = TRUE;
		m_bAudioState = TRUE;
	}
	else if(m_dwUserId != INVALID_USERID && dwUserId == INVALID_USERID)	// ȡ�����û��Ĺ���
	{
		BRAC_UserSpeakControl(m_dwUserId,FALSE);	// ȡ������ʱ�ر�������Ƶ������
		BRAC_UserCameraControl(m_dwUserId,FALSE);
		m_bVideoState = FALSE;
		m_bAudioState = FALSE;

		m_bNeedShowBkPic = TRUE;
		m_bRegualShowVideo = TRUE;
		// ������û�����Ƶ������
		::EnterCriticalSection(&m_hBufSection);
		if(m_pLastVideoBuf)
		{
			free(m_pLastVideoBuf);
			m_pLastVideoBuf = NULL;
		}
		m_dwLastVideoLen = 0;
		::LeaveCriticalSection(&m_hBufSection);

		if(m_pFullScreenDlg)
			m_pFullScreenDlg->ShowWindow(SW_HIDE);
	}
	m_dwUserId = dwUserId;
	// ˢ�°�ť����״̬
	PostMessage(WM_REFRESH_BUTTON_STATE,0,0);
}

/**
 *	�յ��ص���Ƶ����
 *	@param lpBuf ��Ƶ���ݻ�����
 *	@param dwLen ��������С
 *	@param bmiHeader ��Ƶ��������ͷ��Ϣ����������Ƶ�Ĵ�С����ɫλ������Ϣ
 */
void CVideoPanelDlg::OnRecvCallBackVideoData(LPVOID lpBuf, DWORD dwLen, BITMAPINFOHEADER bmiHeader)
{
	if(m_bNeedShowBkPic)
	{
		m_bNeedShowBkPic = FALSE;
		Invalidate();
	}
	// ����Ƶ���ݱ�������
	::EnterCriticalSection(&m_hBufSection);
	if(!m_pLastVideoBuf || m_dwLastVideoLen < dwLen)
		m_pLastVideoBuf = (PCHAR)realloc(m_pLastVideoBuf,dwLen);

	if(m_pLastVideoBuf)
	{
		memcpy(m_pLastVideoBuf,lpBuf,dwLen);
		m_dwLastVideoLen = dwLen;
		memcpy(&m_bmiLastVideo,&bmiHeader,sizeof(BITMAPINFOHEADER));
		// ������Ϣˢ����Ƶ
		PostMessage(WM_REFRESH_VIDEO,0,0);
	}
	::LeaveCriticalSection(&m_hBufSection);
}

LRESULT CVideoPanelDlg::OnRefreshVideo(WPARAM wParam, LPARAM lParam)
{
	if(!m_pLastVideoBuf || m_dwLastVideoLen == 0 || !m_bVideoState)
		return 0;
	::EnterCriticalSection(&m_hBufSection);

	CRect dispRect(m_rcVideoDisp);
	//����Bitmap  
	BITMAPINFOHEADER  *lpbmi = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER)+m_dwLastVideoLen);  
	ZeroMemory(lpbmi,sizeof(BITMAPINFOHEADER)+m_dwLastVideoLen);
    memcpy((LPBYTE)lpbmi,&m_bmiLastVideo,sizeof(BITMAPINFOHEADER));
	memcpy(((LPBYTE)lpbmi+sizeof(BITMAPINFOHEADER)),m_pLastVideoBuf,m_dwLastVideoLen);  
	
	HDC hdc = ::GetDC(m_hWnd);
	HDC dcMem = ::CreateCompatibleDC(hdc);  
	HBITMAP hBitmap = CreateDIBitmap(hdc,lpbmi,CBM_INIT,(LPBYTE)lpbmi+sizeof(BITMAPINFOHEADER),(LPBITMAPINFO)lpbmi,DIB_RGB_COLORS);  
	//��ͼ  
	HBITMAP holdbm = (HBITMAP)SelectObject(dcMem,hBitmap);  
	::SetStretchBltMode(hdc,COLORONCOLOR);
	::StretchBlt(hdc,dispRect.left,dispRect.top,dispRect.Width(),dispRect.Height(),dcMem,0,0,m_bmiLastVideo.biWidth,m_bmiLastVideo.biHeight,SRCCOPY);
	
	SelectObject(dcMem,holdbm);  
	::DeleteDC(dcMem);
	::DeleteObject(hBitmap);
	::ReleaseDC(m_hWnd,hdc);
	free(lpbmi);
	lpbmi = NULL;
	::LeaveCriticalSection(&m_hBufSection);
	return 0;
}

void CVideoPanelDlg::OnButtonVideoControl() 
{
	if(m_dwUserId == INVALID_USERID)
		return;
	m_bVideoState = !m_bVideoState;
	BRAC_UserCameraControl(m_dwUserId,m_bVideoState);
	if(!m_bVideoState)
	{
		m_bNeedShowBkPic = TRUE;
		InvalidateRect(m_rcMainDisp);
	}
	PostMessage(WM_REFRESH_BUTTON_STATE,0,0);
}

void CVideoPanelDlg::OnButtonAudioControl() 
{
	if(m_dwUserId == INVALID_USERID)
		return;
	m_bAudioState = !m_bAudioState;
	BRAC_UserSpeakControl(m_dwUserId,m_bAudioState);
	PostMessage(WM_REFRESH_BUTTON_STATE,0,0);
}

void CVideoPanelDlg::OnButtonSnapshot() 
{
	if(m_dwUserId == INVALID_USERID)
		return;
	BRAC_SnapShot(m_dwUserId,0,0);
}

void CVideoPanelDlg::OnButtonRecord() 
{

	
}

BOOL CVideoPanelDlg::PreTranslateMessage(MSG* pMsg) 
{
	BOOL bNeedTranslate = TRUE;
	if(pMsg->message == WM_KEYDOWN)
    {
		switch(pMsg->wParam)
        {
        case VK_RETURN:
        case VK_SPACE:
		case VK_ESCAPE:
			bNeedTranslate = FALSE;
			break;
		default:
			break;
        }
	}
	return bNeedTranslate ? CDialog::PreTranslateMessage(pMsg) : TRUE;
}

// ������Ƶ��ʾ��������Ƶ��һ���ı�����ʾ
void CVideoPanelDlg::AdjustVideoDispRect(CRect& rc)
{
	float fCurRate = (float)rc.Width() / (float)rc.Height();
	float fStandardRate = (float)m_dwWidth / (float)m_dwHeight;
	if(fCurRate >= fStandardRate)		// ʵ����ʾ����ȽϿ�
	{
		DWORD dwNewWidth = (DWORD)(fStandardRate * rc.Height());
		DWORD dwRemainWidth = rc.Width() - dwNewWidth;
		rc.left += dwRemainWidth/2;
		rc.right -= dwRemainWidth/2;
	}
	else								// ʵ����ʾ����Ƚ�խ
	{
		DWORD dwNewHeight = (DWORD)(rc.Width() / fStandardRate);
		DWORD dwRemainHeight = rc.Height() - dwNewHeight;
		rc.top += dwRemainHeight/2;
		rc.bottom -= dwRemainHeight/2;
	}
	// ��󽫴�С��������Ϊż��
	if(rc.Width() % 2)
		rc.right--;
	if(rc.Height() % 2)
		rc.bottom--;
}
/**
 *	�û����˫����Ƶ��壬�ı���Ƶ��ʾ���
 */
void CVideoPanelDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(m_dwUserId == INVALID_USERID)
		return;
	if(m_bRegualShowVideo)
	{
		m_bRegualShowVideo = FALSE;
		m_rcVideoDisp = m_rcMainDisp;
		InvalidateRect(m_rcMainDisp);
	
		if(m_dwUserId != INVALID_USERID)
			BRAC_SetVideoPos(m_dwUserId,m_hWnd,m_rcVideoDisp.left,m_rcVideoDisp.top,m_rcVideoDisp.right,m_rcVideoDisp.bottom);
	}
	else
	{
		if(m_pFullScreenDlg)
		{
			m_pFullScreenDlg->ShowWindow(SW_MAXIMIZE);
			CRect rc;
			m_pFullScreenDlg->GetClientRect(rc);
			BRAC_SetVideoPos(m_dwUserId,m_pFullScreenDlg->GetSafeHwnd(),rc.left,rc.top,rc.right,rc.bottom);
		}
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CVideoPanelDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMER_REFRESH_SPEAKVOLUME:
		if(m_dwUserId != INVALID_USERID)
		{
			DOUBLE fSpeakVolume = 0.0;
			BRAC_QueryUserState(m_dwUserId,BRAC_USERSTATE_SPEAKVOLUME,(PCHAR)&fSpeakVolume,sizeof(DOUBLE));
			m_ctrlAudioVolume.SetPos((INT)fSpeakVolume);
		}
		else
			m_ctrlAudioVolume.SetPos(0);
		break;
	case TIMER_REFRESH_NETSTATUS:
		if(m_dwUserId != INVALID_USERID)
		{
			DWORD dwNetStatus = 0;
			BRAC_QueryUserState(m_dwUserId,BRAC_USERSTATE_NETWORKSTATUS,(PCHAR)&dwNetStatus,sizeof(DWORD));
			switch(dwNetStatus)
			{
			case 0:	m_NetStatusPic.Load(IDR_IMAGE_NETSTATUS_VERYGOOD);	break;
			case 1:	m_NetStatusPic.Load(IDR_IMAGE_NETSTATUS_GOOD);		break;
			case 2:	m_NetStatusPic.Load(IDR_IMAGE_NETSTATUS_NORMAL);	break;
			case 3:	m_NetStatusPic.Load(IDR_IMAGE_NETSTATUS_BAD);		break;
			case 4:	m_NetStatusPic.Load(IDR_IMAGE_NETSTATUS_VERYBAD);	break;
			default:	break;
			}
			InvalidateRect(m_rcNetStatus);

			// ��ѯ������
			DWORD dwPackLossRate = 0;
			if(BRAC_QueryUserState(m_dwUserId, BRAC_USERSTATE_PACKLOSSRATE, (PCHAR)&dwPackLossRate, sizeof(DWORD)) == 0)
			{
				CString strNotify;
				strNotify.Format("����(��ǰ�����ʣ�%d%%)", dwPackLossRate==100 ? 0 : dwPackLossRate);
				m_btnSnapshot.SetTooltipText(strNotify);
			}
		}
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CVideoPanelDlg::OnRefreshButtonState(WPARAM wParam, LPARAM lParam)
{
	// ˢ����Ƶ���ư�ť״̬
	m_btnVideoCtrl.SetBitmaps(m_bVideoState ? IDB_BITMAP_OPENVIDEO : IDB_BITMAP_CLOSEVIDEO,RGB(255, 0, 255));
	m_btnVideoCtrl.SetTooltipText(m_bVideoState ? "�ر���Ƶ" : "����Ƶ");
	CRect rcVideoBtn;
	m_btnVideoCtrl.GetWindowRect(rcVideoBtn);
	ScreenToClient(rcVideoBtn);
	InvalidateRect(rcVideoBtn);
	// ˢ����Ƶ���ư�ť״̬
	m_btnAudioCtrl.SetBitmaps(m_bAudioState ? IDB_BITMAP_OPENAUDIO : IDB_BITMAP_CLOSEAUDIO,RGB(255, 0, 255));
	m_btnAudioCtrl.SetTooltipText(m_bVideoState ? "�ر�����" : "������");
	CRect rcAudioBtn;
	m_btnAudioCtrl.GetWindowRect(rcAudioBtn);
	ScreenToClient(rcAudioBtn);
	InvalidateRect(rcAudioBtn);

	return 0;
}
// �˳�ȫ����Ϣ
LRESULT CVideoPanelDlg::OnExitFullScreen(WPARAM wParam, LPARAM lParam)
{
	// ����ȫ���Ի���
	if(m_pFullScreenDlg)
		m_pFullScreenDlg->ShowWindow(SW_HIDE);
	// ����Ƶ��ʾ�ڱ��ش���
	m_bRegualShowVideo = TRUE;
	m_rcVideoDisp = m_rcMainDisp;
	AdjustVideoDispRect(m_rcVideoDisp);
	if(m_dwUserId != INVALID_USERID)
		BRAC_SetVideoPos(m_dwUserId,m_hWnd,m_rcVideoDisp.left,m_rcVideoDisp.top,m_rcVideoDisp.right,m_rcVideoDisp.bottom);
	Invalidate();
	return 0;
}
// �û���Ƶ�ֱ��ʱ仯
void CVideoPanelDlg::OnVideoSizeChange(DWORD dwWidth, DWORD dwHeight)
{
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
}