// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BRAnyChatMeeting.h"
#include "SettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define VIDEO_QUALITY_BAD			1
#define VIDEO_QUALITY_NORMAL		2
#define VIDEO_QUALITY_MIDDLE		3
#define VIDEO_QUALITY_GOOD			4
#define VIDEO_QUALITY_BEST			5

#define AUDIO_VOLUME_SLIDECHG		WM_USER + 100

#define BITRATE_REFRESHTIMER		1				///< ����ˢ�¶�ʱ��
#define RESOLUTION_REFRESHTIMER		2				///< �ֱ���ˢ�¶�ʱ��


CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsDlg)
	m_iProxyUserId = 0;
	m_iMultiCastPort = 0;
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Control(pDX, IDC_COMBO_MULTICASTNIC, m_MultiCastNicCombo);
	DDX_Control(pDX, IDC_COMBO_MULTICASTCTRL, m_MultiCastCtrlCombo);
	DDX_Control(pDX, IDC_COMBO_MULTICASTPOLITIC, m_MultiCastPoliticCombo);
	DDX_Control(pDX, IDC_COMBO_AUDIOPLAYBACK, m_AudioPlaybackCombo);
	DDX_Control(pDX, IDC_COMBO_VIDEOPRESET, m_VideoPresetCombo);
	DDX_Control(pDX, IDC_COMBO_VIDEOBITRATE, m_VideoBitrateCombo);
	DDX_Control(pDX, IDC_COMBO_VIDEOFPS, m_VideoFpsCombo);
	DDX_Control(pDX, IDC_COMBO_VIDEOSIZE, m_VideoSizeCombo);
	DDX_Control(pDX, IDC_COMBO_AUDIOWORKMODE, m_AudioWorkModeCombo);
	DDX_Control(pDX, IDC_SLIDER_WAVEINVOLUME, m_ctrlWaveInVolume);
	DDX_Control(pDX, IDC_SLIDER_WAVEOUTVOLUME, m_ctrlWaveOutVolume);
	DDX_Control(pDX, IDC_COMBO_VIDEOQUALITY, m_VideoQualityCombo);
	DDX_Control(pDX, IDC_COMBO_VIDEOCAPTURE, m_VideoCaptureCombo);
	DDX_Control(pDX, IDC_COMBO_AUDIOCAPTURE, m_AudioCaptureCombo);
	DDX_Text(pDX, IDC_EDIT_PROXYUSERID, m_iProxyUserId);
	DDX_Text(pDX, IDC_EDIT_MULTICASTPORT, m_iMultiCastPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_VIDEOCAPTURE, OnSelchangeVideoCapture)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIOCAPTURE, OnSelchangeAudioCapture)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOPROPERTY, OnButtonVideoProperty)
	ON_MESSAGE(AUDIO_VOLUME_SLIDECHG,OnAudioVolumeSlideChange)
	ON_BN_CLICKED(IDC_CHECK_AUDIOVAD, OnCheckAudioVAD)
	ON_BN_CLICKED(IDC_CHECK_AUDIOAEC, OnCheckAudioAEC)
	ON_BN_CLICKED(IDC_CHECK_AUDIONS, OnCheckAudioNS)
	ON_BN_CLICKED(IDC_CHECK_AUDIOAGC, OnCheckAudioAGC)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIOWORKMODE, OnSelchangeAudioWorkMode)
	ON_BN_CLICKED(IDC_CHECK_DEINTERLACE, OnCheckDeinterlace)
	ON_BN_CLICKED(IDC_CHECK_MICBOOST, OnCheckMicBoost)
	ON_BN_CLICKED(IDC_CHECK_STREAMPROXY, OnCheckStreamProxy)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEPROXYUSER, OnButtonChangeProxyUser)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOAPPLY, OnButtonVideoApply)
	ON_BN_CLICKED(IDC_CHECK_SERVERVIDEOSETTINGS, OnCheckServerVideoSettings)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_VIDEOBITRATE, OnSelchangeComboVideoBitrate)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIOPLAYBACK, OnSelchangeComboAudioPlayback)
	ON_CBN_SELCHANGE(IDC_COMBO_MULTICASTPOLITIC, OnSelchangeComboMulticastPolitic)
	ON_BN_CLICKED(IDC_BUTTON_MULTICASTAPPLY, OnButtonMulticastApply)
	ON_BN_CLICKED(IDC_CHECK_ENABLEP2P, OnCheckEnableP2P)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD dwIndex = 0;
	dwIndex = m_VideoQualityCombo.AddString("�ϲ�����");
	m_VideoQualityCombo.SetItemData(dwIndex,VIDEO_QUALITY_BAD);
	dwIndex = m_VideoQualityCombo.AddString("һ������");
	m_VideoQualityCombo.SetItemData(dwIndex,VIDEO_QUALITY_NORMAL);
	dwIndex = m_VideoQualityCombo.AddString("�е�����");
	m_VideoQualityCombo.SetItemData(dwIndex,VIDEO_QUALITY_MIDDLE);
	m_VideoQualityCombo.SetCurSel(dwIndex);
	dwIndex = m_VideoQualityCombo.AddString("�Ϻ�����");
	m_VideoQualityCombo.SetItemData(dwIndex,VIDEO_QUALITY_GOOD);
	dwIndex = m_VideoQualityCombo.AddString("�������");
	m_VideoQualityCombo.SetItemData(dwIndex,VIDEO_QUALITY_BEST);
	m_VideoQualityCombo.EnableWindow(FALSE);

	dwIndex = m_VideoSizeCombo.AddString("176x144");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(176,144));
	dwIndex = m_VideoSizeCombo.AddString("320x240");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(320,240));
	m_VideoSizeCombo.SetCurSel(dwIndex);
	dwIndex = m_VideoSizeCombo.AddString("352x288");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(352,288));
	dwIndex = m_VideoSizeCombo.AddString("640x480");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(640,480));
	dwIndex = m_VideoSizeCombo.AddString("720x480");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(720,480));
	dwIndex = m_VideoSizeCombo.AddString("720x576");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(720,576));
	dwIndex = m_VideoSizeCombo.AddString("800x600");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(800,600));
	dwIndex = m_VideoSizeCombo.AddString("960x720");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(960,720));
	dwIndex = m_VideoSizeCombo.AddString("1024x576");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(1024,576));
	dwIndex = m_VideoSizeCombo.AddString("1280x720");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(1280,720));
	dwIndex = m_VideoSizeCombo.AddString("1280x1024");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(1280,1024));
	dwIndex = m_VideoSizeCombo.AddString("1920x1080");
	m_VideoSizeCombo.SetItemData(dwIndex, MAKELONG(1920,1080));

	dwIndex = m_VideoFpsCombo.AddString("5  FPS");
	m_VideoFpsCombo.SetItemData(dwIndex,5);
	dwIndex = m_VideoFpsCombo.AddString("8  FPS");
	m_VideoFpsCombo.SetItemData(dwIndex,8);
	m_VideoFpsCombo.SetCurSel(dwIndex);
	dwIndex = m_VideoFpsCombo.AddString("12 FPS");
	m_VideoFpsCombo.SetItemData(dwIndex,12);
	dwIndex = m_VideoFpsCombo.AddString("15 FPS");
	m_VideoFpsCombo.SetItemData(dwIndex,15);
	dwIndex = m_VideoFpsCombo.AddString("20 FPS");
	m_VideoFpsCombo.SetItemData(dwIndex,20);
	dwIndex = m_VideoFpsCombo.AddString("25 FPS");
	m_VideoFpsCombo.SetItemData(dwIndex,25);
	dwIndex = m_VideoFpsCombo.AddString("30 FPS");
	m_VideoFpsCombo.SetItemData(dwIndex,30);

	dwIndex = m_VideoBitrateCombo.AddString("����ģʽ");
	m_VideoBitrateCombo.SetItemData(dwIndex,0);
	dwIndex = m_VideoBitrateCombo.AddString("40");
	m_VideoBitrateCombo.SetItemData(dwIndex,40*1000);
	dwIndex = m_VideoBitrateCombo.AddString("60");
	m_VideoBitrateCombo.SetItemData(dwIndex,60*1000);
	m_VideoBitrateCombo.SetCurSel(dwIndex);
	dwIndex = m_VideoBitrateCombo.AddString("100");
	m_VideoBitrateCombo.SetItemData(dwIndex,100*1000);
	dwIndex = m_VideoBitrateCombo.AddString("150");
	m_VideoBitrateCombo.SetItemData(dwIndex,150*1000);
	dwIndex = m_VideoBitrateCombo.AddString("200");
	m_VideoBitrateCombo.SetItemData(dwIndex,200*1000);
	dwIndex = m_VideoBitrateCombo.AddString("300");
	m_VideoBitrateCombo.SetItemData(dwIndex,300*1000);
	dwIndex = m_VideoBitrateCombo.AddString("400");
	m_VideoBitrateCombo.SetItemData(dwIndex,400*1000);
	dwIndex = m_VideoBitrateCombo.AddString("500");
	m_VideoBitrateCombo.SetItemData(dwIndex,500*1000);
	dwIndex = m_VideoBitrateCombo.AddString("600");
	m_VideoBitrateCombo.SetItemData(dwIndex,600*1000);
	dwIndex = m_VideoBitrateCombo.AddString("800");
	m_VideoBitrateCombo.SetItemData(dwIndex,800*1000);
	dwIndex = m_VideoBitrateCombo.AddString("1000");
	m_VideoBitrateCombo.SetItemData(dwIndex,1000*1000);
	dwIndex = m_VideoBitrateCombo.AddString("1200");
	m_VideoBitrateCombo.SetItemData(dwIndex,1200*1000);
	dwIndex = m_VideoBitrateCombo.AddString("1500");
	m_VideoBitrateCombo.SetItemData(dwIndex,1500*1000);

	dwIndex = m_VideoPresetCombo.AddString("1");
	m_VideoPresetCombo.SetItemData(dwIndex,1);
	dwIndex = m_VideoPresetCombo.AddString("2");
	m_VideoPresetCombo.SetItemData(dwIndex,2);
	dwIndex = m_VideoPresetCombo.AddString("3");
	m_VideoPresetCombo.SetItemData(dwIndex,3);
	m_VideoPresetCombo.SetCurSel(dwIndex);
	dwIndex = m_VideoPresetCombo.AddString("4");
	m_VideoPresetCombo.SetItemData(dwIndex,4);
	dwIndex = m_VideoPresetCombo.AddString("5");
	m_VideoPresetCombo.SetItemData(dwIndex,5);


	dwIndex = m_AudioWorkModeCombo.AddString("����ģʽ(Ĭ��)");
	m_AudioWorkModeCombo.SetCurSel(dwIndex);
	m_AudioWorkModeCombo.SetItemData(dwIndex,0);
	dwIndex = m_AudioWorkModeCombo.AddString("�Ÿ�ģʽ");
	m_AudioWorkModeCombo.SetItemData(dwIndex,1);
	dwIndex = m_AudioWorkModeCombo.AddString("����OKģʽ");
	m_AudioWorkModeCombo.SetItemData(dwIndex,2);
	dwIndex = m_AudioWorkModeCombo.AddString("��·����ģʽ");
	m_AudioWorkModeCombo.SetItemData(dwIndex,3);

	DWORD dwVolume = 0;
	BRAC_AudioGetVolume(BRAC_AD_WAVEOUT,dwVolume);
	m_ctrlWaveOutVolume.SetPos(dwVolume);
	m_ctrlWaveOutVolume.SetNotifyMsg(AUDIO_VOLUME_SLIDECHG);
	dwVolume = 0;
	BRAC_AudioGetVolume(BRAC_AD_WAVEIN,dwVolume);
	m_ctrlWaveInVolume.SetPos(dwVolume);
	m_ctrlWaveInVolume.SetNotifyMsg(AUDIO_VOLUME_SLIDECHG);

	// ����������������
	GetDlgItem(IDC_CHECK_STREAMPROXY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PROXYUSERID)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CHANGEPROXYUSER)->ShowWindow(SW_HIDE);

	// Ĭ���ǲ��÷��������ò���
	((CButton*)GetDlgItem(IDC_CHECK_SERVERVIDEOSETTINGS))->SetCheck(TRUE);
	RefreshVideoSettingsCtrlStatus(TRUE);

	dwIndex = m_MultiCastPoliticCombo.AddString("������������(Ĭ��)");
	m_MultiCastPoliticCombo.SetItemData(dwIndex, 0);
	m_MultiCastPoliticCombo.SetCurSel(dwIndex);
	dwIndex = m_MultiCastPoliticCombo.AddString("���鲥����");
	m_MultiCastPoliticCombo.SetItemData(dwIndex, 1);
	dwIndex = m_MultiCastPoliticCombo.AddString("����������+�鲥");
	m_MultiCastPoliticCombo.SetItemData(dwIndex, 2);

	dwIndex = m_MultiCastCtrlCombo.AddString("����ಥ��");
	m_MultiCastCtrlCombo.SetItemData(dwIndex, BRAC_MCFLAGS_JOINGROUP);
	m_MultiCastCtrlCombo.SetCurSel(dwIndex);
	dwIndex = m_MultiCastCtrlCombo.AddString("�˳��ಥ��");
	m_MultiCastCtrlCombo.SetItemData(dwIndex, BRAC_MCFLAGS_LEAVEGROUP);

	SetDlgItemText(IDC_MULTICAST_IPADDR,"224.2.3.4");
	SetDlgItemText(IDC_EDIT_MULTICASTPORT,"9810");

	dwIndex = m_MultiCastNicCombo.AddString("Ĭ�������豸");
	m_MultiCastNicCombo.SetCurSel(dwIndex);
	// ö��ϵͳ�е����������豸
	DWORD LocalIPArray[10] = {0};
	DWORD dwCount = GetLocalIPAddr(LocalIPArray, 10);
	for (int i=0; i<(int)dwCount; i++)
	{
		struct in_addr in = {0};
		in.s_addr = htonl(LocalIPArray[i]);
		m_MultiCastNicCombo.AddString(inet_ntoa(in));
	}

	((CButton*)GetDlgItem(IDC_CHECK_ENABLEP2P))->SetCheck(TRUE);		// Ĭ����P2P����

	SetTimer(BITRATE_REFRESHTIMER, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
/**
 *	������ʾ��Ϣ��ˢ����ص�״̬
 */
void CSettingsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
		RefreshAllStatus();
}

void CSettingsDlg::OnSelchangeVideoCapture() 
{
	CString strCaptureName;
	m_VideoCaptureCombo.GetLBText(m_VideoCaptureCombo.GetCurSel(),strCaptureName);
	BRAC_SelectVideoCapture(strCaptureName);	
}
// �û�ѡ��ָ������Ƶ�ɼ��豸
void CSettingsDlg::OnSelchangeAudioCapture() 
{
	CString strCaptureName;
	m_AudioCaptureCombo.GetLBText(m_AudioCaptureCombo.GetCurSel(),strCaptureName);
	BRAC_SelectAudioCapture(strCaptureName);
}
// �û�ѡ��ָ������Ƶ�����豸
void CSettingsDlg::OnSelchangeComboAudioPlayback() 
{
	CString strDeviceName;
	m_AudioPlaybackCombo.GetLBText(m_AudioPlaybackCombo.GetCurSel(),strDeviceName);
	BRAC_SelectAudioPlayback(strDeviceName);
}

// ˢ��������Ƶ�ɼ��豸
void CSettingsDlg::RefreshCaptureDevice(void)
{
	// ˢ����Ƶ�ɼ��豸�����б�
	PCHAR DeviceName[10] = {0};
	DWORD DeviceNum = 10;
	BRAC_EnumVideoCapture(DeviceName,DeviceNum);
	
	m_VideoCaptureCombo.ResetContent();
	for(INT i=0; i<(INT)DeviceNum; i++)
	{
		if(DeviceName[i])
		{
			m_VideoCaptureCombo.AddString(DeviceName[i]);
			::GlobalFree((HGLOBAL)DeviceName[i]);
			DeviceName[i] = NULL;
		}
	}
	
	// ѡ��ǰ�򿪵���Ƶ�ɼ��豸
	CHAR szVideoDeviceName[100] = {0};
	BRAC_GetCurVideoCapture(szVideoDeviceName,sizeof(szVideoDeviceName));
	m_VideoCaptureCombo.SelectString(-1,szVideoDeviceName);
	
	// ˢ����Ƶ�ɼ��豸�����б�
	DeviceNum = 10;
	BRAC_EnumAudioCapture(DeviceName,DeviceNum);
	
	m_AudioCaptureCombo.ResetContent();
	for(i=0; i<(INT)DeviceNum; i++)
	{
		if(DeviceName[i])
		{
			m_AudioCaptureCombo.AddString(DeviceName[i]);
			::GlobalFree((HGLOBAL)DeviceName[i]);
			DeviceName[i] = NULL;
		}
	}
	// ѡ��ǰ�򿪵���Ƶ�ɼ��豸
	CHAR szAudioDeviceName[100] = {0};
	BRAC_GetCurAudioCapture(szAudioDeviceName,sizeof(szAudioDeviceName));
	m_AudioCaptureCombo.SelectString(-1,szAudioDeviceName);

	// ˢ����Ƶ�����豸�����б�
	DeviceNum = 10;
	BRAC_EnumAudioPlayback(DeviceName,DeviceNum);
	
	m_AudioPlaybackCombo.ResetContent();
	for(i=0; i<(INT)DeviceNum; i++)
	{
		if(DeviceName[i])
		{
			m_AudioPlaybackCombo.AddString(DeviceName[i]);
			::GlobalFree((HGLOBAL)DeviceName[i]);
			DeviceName[i] = NULL;
		}
	}
	// ѡ��ǰ�򿪵���Ƶ�����豸
	CHAR szAudioPlayDevice[100] = {0};
	BRAC_GetCurAudioPlayback(szAudioPlayDevice,sizeof(szAudioPlayDevice));
	m_AudioPlaybackCombo.SelectString(-1,szAudioPlayDevice);	
}

void CSettingsDlg::OnButtonVideoProperty() 
{
	BRAC_ShowLVProperty(m_hWnd,"AnyChat ������Ƶ���ʵ���");
}
/**
 *	�û��϶�����������
 *	@param lParam ��Ϣ���ͷ��Ĵ��ھ��
 */
LRESULT CSettingsDlg::OnAudioVolumeSlideChange(WPARAM wParam, LPARAM lParam)
{
	if((HWND)lParam == m_ctrlWaveOutVolume.GetSafeHwnd())
	{
		DWORD dwVolume = m_ctrlWaveOutVolume.GetPos();
		BRAC_AudioSetVolume(BRAC_AD_WAVEOUT,dwVolume);
	}
	else if((HWND)lParam == m_ctrlWaveInVolume.GetSafeHwnd())
	{
		DWORD dwVolume = m_ctrlWaveInVolume.GetPos();
		BRAC_AudioSetVolume(BRAC_AD_WAVEIN,dwVolume);
	}
	return 0;
}

// ϵͳ�����仯֪ͨ
void CSettingsDlg::OnSystemVolumeChangeNotify(BRAC_AudioDevice device, DWORD dwVolume)
{
	if(device == BRAC_AD_WAVEOUT)
		m_ctrlWaveOutVolume.SetPos(dwVolume);
	else if(device == BRAC_AD_WAVEIN)
		m_ctrlWaveInVolume.SetPos(dwVolume);
}
/**
 *	�û�������Ƶ�����������
 */
void CSettingsDlg::OnCheckAudioVAD() 
{
	BOOL bEnableVAD = ((CButton*)GetDlgItem(IDC_CHECK_AUDIOVAD))->GetCheck();
	BRAC_SetSDKOption(BRAC_SO_AUDIO_VADCTRL,(PCHAR)&bEnableVAD,sizeof(bEnableVAD));	

/*	// ���Զ�̬������Ƶ�������� only for test!
	BOOL bSelect = ((CButton*)GetDlgItem(IDC_CHECK_AUDIOVAD))->GetCheck();
	DWORD dwMonoBitrate = bSelect ? 5000 : 25000;
	BRAC_SetSDKOption(BRAC_SO_AUDIO_MONOBITRATE,(PCHAR)&dwMonoBitrate,sizeof(DWORD));
	DWORD dwStereoBitrate = bSelect? 15000 : 96000;
	BRAC_SetSDKOption(BRAC_SO_AUDIO_STEREOBITRATE,(PCHAR)&dwStereoBitrate,sizeof(DWORD));
*/
}
/**
 *	�û�������Ƶ������������
 */
void CSettingsDlg::OnCheckAudioAEC() 
{
	BOOL bEnableAEC = ((CButton*)GetDlgItem(IDC_CHECK_AUDIOAEC))->GetCheck();
	BRAC_SetSDKOption(BRAC_SO_AUDIO_ECHOCTRL,(PCHAR)&bEnableAEC,sizeof(bEnableAEC));	
}
/**
 *	�û�������Ƶ������������
 */
void CSettingsDlg::OnCheckAudioNS() 
{
	BOOL bEnableNS = ((CButton*)GetDlgItem(IDC_CHECK_AUDIONS))->GetCheck();
	BRAC_SetSDKOption(BRAC_SO_AUDIO_NSCTRL,(PCHAR)&bEnableNS,sizeof(bEnableNS));	
}
/**
 *	�û�������Ƶ�Զ���������
 */
void CSettingsDlg::OnCheckAudioAGC() 
{
	BOOL bEnableAGC = ((CButton*)GetDlgItem(IDC_CHECK_AUDIOAGC))->GetCheck();
	BRAC_SetSDKOption(BRAC_SO_AUDIO_AGCCTRL,(PCHAR)&bEnableAGC,sizeof(bEnableAGC));
	
}
/**
 *	�û�ѡ����Ƶ����ģʽ
 */
void CSettingsDlg::OnSelchangeAudioWorkMode() 
{
	DWORD dwSelectMode = m_AudioWorkModeCombo.GetItemData(m_AudioWorkModeCombo.GetCurSel());
	BRAC_SetSDKOption(BRAC_SO_AUDIO_CAPTUREMODE,(PCHAR)&dwSelectMode,sizeof(dwSelectMode));
	// �޸�ģʽ�󣬵ײ�����Ч�����޸ģ������ݶ�Ӧ��ģʽ���Զ�ѡ����ʵ���Ƶ�����豸��������Ҫ��ˢ��һ����ص�״̬
	RefreshAllStatus();
}
/**
 *	�û�����Ƶ������֯����������
 *	��������ƵԴ�Ǹ���ɨ��Դ��������źţ�ʱͨ������֯���������߻�������
 */
void CSettingsDlg::OnCheckDeinterlace() 
{
	BOOL bDeinterlace = ((CButton*)GetDlgItem(IDC_CHECK_DEINTERLACE))->GetCheck();
	BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_DEINTERLACE,(PCHAR)&bDeinterlace,sizeof(bDeinterlace));	
}

// ˢ������״̬
void CSettingsDlg::RefreshAllStatus(void)
{
	// ˢ������Ƶ�ɼ��豸��Ϣ
	RefreshCaptureDevice();
	// ˢ����Ƶ��Ч����״̬
	BOOL bEnableAGC = FALSE;
	BRAC_GetSDKOption(BRAC_SO_AUDIO_AGCCTRL,(PCHAR)&bEnableAGC,sizeof(bEnableAGC));
	((CButton*)GetDlgItem(IDC_CHECK_AUDIOAGC))->SetCheck(bEnableAGC);
	BOOL bEnableAEC = FALSE;
	BRAC_GetSDKOption(BRAC_SO_AUDIO_ECHOCTRL,(PCHAR)&bEnableAEC,sizeof(bEnableAEC));
	((CButton*)GetDlgItem(IDC_CHECK_AUDIOAEC))->SetCheck(bEnableAEC);
	BOOL bEnableVAD = FALSE;
	BRAC_GetSDKOption(BRAC_SO_AUDIO_VADCTRL,(PCHAR)&bEnableVAD,sizeof(bEnableVAD));
	((CButton*)GetDlgItem(IDC_CHECK_AUDIOVAD))->SetCheck(bEnableVAD);
	BOOL bEnableNS = FALSE;
	BRAC_GetSDKOption(BRAC_SO_AUDIO_NSCTRL,(PCHAR)&bEnableNS,sizeof(bEnableNS));
	((CButton*)GetDlgItem(IDC_CHECK_AUDIONS))->SetCheck(bEnableNS);
	
	BOOL bDeinterlace = FALSE;
	BRAC_GetSDKOption(BRAC_SO_LOCALVIDEO_DEINTERLACE,(PCHAR)&bDeinterlace,sizeof(bDeinterlace));
	((CButton*)GetDlgItem(IDC_CHECK_DEINTERLACE))->SetCheck(bDeinterlace);

	// ˢ����Ƶ�ɼ�ģʽ
	DWORD dwCurMode = 0;
	BRAC_GetSDKOption(BRAC_SO_AUDIO_CAPTUREMODE,(PCHAR)&dwCurMode,sizeof(dwCurMode));
	for (int i=0; i<m_AudioWorkModeCombo.GetCount(); i++)
	{
		if(m_AudioWorkModeCombo.GetItemData(i) == dwCurMode)
		{
			m_AudioWorkModeCombo.SetCurSel(i);
			break;
		}
	}
	// ˢ��Mic��ǿ״̬
	DWORD dwMicBoostState = 0;
	BRAC_GetSDKOption(BRAC_SO_AUDIO_MICBOOST,(PCHAR)&dwMicBoostState,sizeof(dwMicBoostState));
	if(dwMicBoostState == 2)
		GetDlgItem(IDC_CHECK_MICBOOST)->EnableWindow(FALSE);
	else
	{
		GetDlgItem(IDC_CHECK_MICBOOST)->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_MICBOOST))->SetCheck(dwMicBoostState);
	}
	// ˢ����Ƶ�ɼ���ǰ�ֱ���
	DWORD dwVideoSize=0;
	BRAC_QueryUserState(-1, BRAC_USERSTATE_VIDEOSIZE, (PCHAR)&dwVideoSize, sizeof(DWORD));
	CString strVideoSize;
	strVideoSize.Format("(��ǰ�ֱ���:%dx%d)", LOWORD(dwVideoSize), HIWORD(dwVideoSize));
	GetDlgItem(IDC_STATIC_CURVIDEOSIZE)->SetWindowText(strVideoSize);
}
/**
 *	�û�����Mic��ǿ����ѡ��
 */
void CSettingsDlg::OnCheckMicBoost() 
{
	BOOL bSelect = ((CButton*)GetDlgItem(IDC_CHECK_MICBOOST))->GetCheck();
	BRAC_SetSDKOption(BRAC_SO_AUDIO_MICBOOST,(PCHAR)&bSelect,sizeof(bSelect));
}
/**
 *	�û���������ر���������
 */
void CSettingsDlg::OnCheckStreamProxy() 
{
	UpdateData();
	BOOL bStartProxy = ((CButton*)GetDlgItem(IDC_CHECK_STREAMPROXY))->GetCheck();
	if(bStartProxy)
	{
		// ������������������ñ�������û�ID
		BRAC_SetSDKOption(BRAC_SO_PROXY_VIDEOCTRL,(PCHAR)&m_iProxyUserId,sizeof(DWORD));
		BRAC_SetSDKOption(BRAC_SO_PROXY_AUDIOCTRL,(PCHAR)&m_iProxyUserId,sizeof(DWORD));
	}
	BRAC_SetSDKOption(BRAC_SO_PROXY_FUNCTIONCTRL,(PCHAR)&bStartProxy,sizeof(DWORD));
}

void CSettingsDlg::OnButtonChangeProxyUser() 
{
	UpdateData();
	// ��̬�л��������û���ID
	BRAC_SetSDKOption(BRAC_SO_PROXY_VIDEOCTRL,(PCHAR)&m_iProxyUserId,sizeof(DWORD));
	BRAC_SetSDKOption(BRAC_SO_PROXY_AUDIOCTRL,(PCHAR)&m_iProxyUserId,sizeof(DWORD));
}
/**
 *	�û�Ӧ����Ƶ�������޸�
 */
void CSettingsDlg::OnButtonVideoApply() 
{
	// ��ȡ�Ƿ�ʹ�÷�����Ĭ����Ƶ���ò���
	BOOL bUserServerVideoSettings = ((CButton*)GetDlgItem(IDC_CHECK_SERVERVIDEOSETTINGS))->GetCheck();
	BOOL bUseAppParam = !bUserServerVideoSettings;
	if(bUseAppParam)
	{
		// �������Ϊ0�����ʾʹ����������ģʽ
		DWORD dwBitrate = m_VideoBitrateCombo.GetItemData(m_VideoBitrateCombo.GetCurSel());
		// ���ñ�����Ƶ���������
		BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_BITRATECTRL,(const char*)&dwBitrate,sizeof(DWORD));
		if(dwBitrate==0)
		{
			DWORD dwQuality = m_VideoQualityCombo.GetItemData(m_VideoQualityCombo.GetCurSel());
			// ���ñ�����Ƶ���������
			BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_QUALITYCTRL,(const char*)&dwQuality,sizeof(DWORD));
		}

		DWORD dwFps = m_VideoFpsCombo.GetItemData(m_VideoFpsCombo.GetCurSel());
		// ���ñ�����Ƶ�����֡��
		BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_FPSCTRL,(PCHAR)&dwFps,sizeof(DWORD));
		// ���ñ�����Ƶ����Ĺؼ�֡���
		DWORD dwGop = dwFps >= 20 ? dwFps : dwFps*4;
		BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_GOPCTRL,(const char*)&dwGop,sizeof(DWORD));

		// ���ñ�����Ƶ�ɼ��ֱ���
		DWORD dwVideoSize = m_VideoSizeCombo.GetItemData(m_VideoSizeCombo.GetCurSel());
		DWORD dwWidth = LOWORD(dwVideoSize);
		DWORD dwHeight = HIWORD(dwVideoSize);
		BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_WIDTHCTRL,(PCHAR)&dwWidth,sizeof(DWORD));
		BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_HEIGHTCTRL,(PCHAR)&dwHeight,sizeof(DWORD));

		// ������Ƶ����Ԥ�������ֵԽ�󣬱�������Խ�ߣ�ռ��CPU��ԴҲ��Խ�ߣ�
		DWORD dwVideoPreset = m_VideoPresetCombo.GetItemData(m_VideoPresetCombo.GetCurSel());
		BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_PRESETCTRL,(const char*)&dwVideoPreset,sizeof(DWORD));
		
	}
	// ��������Ч
	BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_APPLYPARAM,(const char*)&bUseAppParam,sizeof(DWORD));
	// ����һ����ʱ��ˢ�·ֱ���
	SetTimer(RESOLUTION_REFRESHTIMER, 1500, NULL);
}
/**
 *	�û��ڷ�����Ĭ�����ò�����Ӧ�ó����Զ�������������ѡ��
 */
void CSettingsDlg::OnCheckServerVideoSettings() 
{
	BOOL bUserServerVideoSettings = ((CButton*)GetDlgItem(IDC_CHECK_SERVERVIDEOSETTINGS))->GetCheck();
	RefreshVideoSettingsCtrlStatus(bUserServerVideoSettings);
}

// �л���Ƶ�������ÿؼ�״̬
void CSettingsDlg::RefreshVideoSettingsCtrlStatus(BOOL bUseServerParam)
{
	GetDlgItem(IDC_COMBO_VIDEOBITRATE)->EnableWindow(!bUseServerParam);
	GetDlgItem(IDC_COMBO_VIDEOFPS)->EnableWindow(!bUseServerParam);
	GetDlgItem(IDC_COMBO_VIDEOPRESET)->EnableWindow(!bUseServerParam);
	GetDlgItem(IDC_COMBO_VIDEOSIZE)->EnableWindow(!bUseServerParam);
}

void CSettingsDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case BITRATE_REFRESHTIMER:
		{
			DWORD dwVideoBitrate = 0;
			CString strNotify;
			BRAC_QueryUserState(-1,BRAC_USERSTATE_VIDEOBITRATE,(PCHAR)&dwVideoBitrate,sizeof(DWORD));
			strNotify.Format(_T("Video:%.02fkbps"),(dwVideoBitrate*8)/1000.0);
			GetDlgItem(IDC_STATIC_VIDEOBITRATE)->SetWindowText(strNotify);
		}
		break;
	case RESOLUTION_REFRESHTIMER:
		KillTimer(RESOLUTION_REFRESHTIMER);
		RefreshAllStatus();
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CSettingsDlg::OnSelchangeComboVideoBitrate() 
{
	// �������Ϊ0�����ʾʹ����������ģʽ
	DWORD dwBitrate = m_VideoBitrateCombo.GetItemData(m_VideoBitrateCombo.GetCurSel());
	m_VideoQualityCombo.EnableWindow(dwBitrate==0);
}
// �û��ı��鲥����
void CSettingsDlg::OnSelchangeComboMulticastPolitic() 
{
	DWORD dwMultiCastPolitic = m_MultiCastPoliticCombo.GetItemData(m_MultiCastPoliticCombo.GetCurSel());
	BRAC_SetSDKOption(BRAC_SO_NETWORK_MULTICASTPOLITIC,(const char*)&dwMultiCastPolitic,sizeof(DWORD));
}
// �û�����IP�鲥�ӿ�
void CSettingsDlg::OnButtonMulticastApply() 
{
	UpdateData();
	DWORD dwFlags = 0;
	dwFlags |= m_MultiCastCtrlCombo.GetItemData(m_MultiCastCtrlCombo.GetCurSel());
	if(dwFlags&BRAC_MCFLAGS_JOINGROUP)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SENDDATA))->GetCheck())
			dwFlags |= BRAC_MCFLAGS_SENDDATA;
		if(((CButton*)GetDlgItem(IDC_CHECK_RECVDATA))->GetCheck())
			dwFlags |= BRAC_MCFLAGS_RECVDATA;
	}
	CString strMultiCastIP;
	GetDlgItemText(IDC_MULTICAST_IPADDR, strMultiCastIP);

	CString strNicIP;
	GetDlgItemText(IDC_COMBO_MULTICASTNIC, strNicIP);
	if(strNicIP.CompareNoCase("Ĭ�������豸") == 0)
		strNicIP = "";

	BRAC_MultiCastControl(strMultiCastIP, m_iMultiCastPort, strNicIP.GetBuffer(0), 5, dwFlags);
}

// ��ȡ����IP��ַ�б�
DWORD CSettingsDlg::GetLocalIPAddr(LPDWORD iparray, DWORD size)
{
	DWORD offset = 0;
	char hostname[256]={0};
	int res = gethostname(hostname, sizeof(hostname));
	if (res != 0)
		return 0;
	
	// ������������ȡ������Ϣ.
	hostent* pHostent = gethostbyname(hostname);
	if (pHostent==NULL)
		return 0;
	
	// �������ص�hostent��Ϣ.
	hostent& he = *pHostent;
	
	sockaddr_in sa;
	for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++)
	{
		memcpy(&sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);
		iparray[offset++] = ntohl(sa.sin_addr.s_addr);
		if(offset > size)
			break;
	}
	return offset;
}

void CSettingsDlg::OnCheckEnableP2P() 
{
	BOOL bEnableP2P = ((CButton*)GetDlgItem(IDC_CHECK_ENABLEP2P))->GetCheck();
	DWORD dwP2PPolitic = bEnableP2P ? 1 : 0;
	BRAC_SetSDKOption(BRAC_SO_NETWORK_P2PPOLITIC,(PCHAR)&dwP2PPolitic,sizeof(dwP2PPolitic));
}
