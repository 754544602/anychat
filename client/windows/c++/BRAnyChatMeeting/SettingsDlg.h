#if !defined(AFX_SETTINGSDLG_H__E76DFB33_AB7D_4885_BD00_B0F0969AFA97__INCLUDED_)
#define AFX_SETTINGSDLG_H__E76DFB33_AB7D_4885_BD00_B0F0969AFA97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDlg.h : header file
//
#include "Ctrls/SlideBar.h"


/**
 *	�߼����öԻ�����
 */
class CSettingsDlg : public CDialog
{
// Construction
public:
	CSettingsDlg(CWnd* pParent = NULL);   // standard constructor


public:
	// ˢ��������Ƶ�ɼ��豸
	void RefreshCaptureDevice(void);
	// ϵͳ�����仯֪ͨ
	void OnSystemVolumeChangeNotify(BRAC_AudioDevice device, DWORD dwVolume);

	// ˢ������״̬
	void RefreshAllStatus(void);
	// �л���Ƶ�������ÿؼ�״̬
	void RefreshVideoSettingsCtrlStatus(BOOL bUseServerParam);
	// ��ȡ����IP��ַ�б�
	DWORD GetLocalIPAddr(LPDWORD iparray, DWORD size);
	
// Dialog Data
	//{{AFX_DATA(CSettingsDlg)
	enum { IDD = IDD_DIALOG_SETTINGS };
	CComboBox	m_MultiCastNicCombo;
	CComboBox	m_MultiCastCtrlCombo;
	CComboBox	m_MultiCastPoliticCombo;
	CComboBox	m_AudioPlaybackCombo;
	CComboBox	m_VideoPresetCombo;
	CComboBox	m_VideoBitrateCombo;
	CComboBox	m_VideoFpsCombo;
	CComboBox	m_VideoSizeCombo;
	CComboBox	m_AudioWorkModeCombo;
	CSlideBar	m_ctrlWaveInVolume;
	CSlideBar	m_ctrlWaveOutVolume;
	CComboBox	m_VideoQualityCombo;
	CComboBox	m_VideoCaptureCombo;
	CComboBox	m_AudioCaptureCombo;
	int		m_iProxyUserId;
	int		m_iMultiCastPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeVideoCapture();
	afx_msg void OnSelchangeAudioCapture();
	afx_msg void OnButtonVideoProperty();
	afx_msg LRESULT OnAudioVolumeSlideChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCheckAudioVAD();
	afx_msg void OnCheckAudioAEC();
	afx_msg void OnCheckAudioNS();
	afx_msg void OnCheckAudioAGC();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeAudioWorkMode();
	afx_msg void OnCheckDeinterlace();
	afx_msg void OnCheckMicBoost();
	afx_msg void OnCheckStreamProxy();
	afx_msg void OnButtonChangeProxyUser();
	afx_msg void OnButtonVideoApply();
	afx_msg void OnCheckServerVideoSettings();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboVideoBitrate();
	afx_msg void OnSelchangeComboAudioPlayback();
	afx_msg void OnSelchangeComboMulticastPolitic();
	afx_msg void OnButtonMulticastApply();
	afx_msg void OnCheckEnableP2P();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__E76DFB33_AB7D_4885_BD00_B0F0969AFA97__INCLUDED_)
