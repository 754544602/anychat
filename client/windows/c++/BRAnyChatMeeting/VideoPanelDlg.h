#if !defined(AFX_VIDEOPANELDLG_H__15023AD4_E5B0_4A48_B448_699AD60A72DD__INCLUDED_)
#define AFX_VIDEOPANELDLG_H__15023AD4_E5B0_4A48_B448_699AD60A72DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ctrls/GradientProgressCtrl.h"
#include "FullScreenDlg.h"

#define INVALID_USERID		0xFFFFFFFF			///< �Ƿ��û�ID

/**
 *	��Ƶ���Ի���
 *	��ʾָ���û�����Ƶ��������С�����ṩ��صĿ��Ʋ���
 */
class CVideoPanelDlg : public CDialog
{
// Construction
public:
	CVideoPanelDlg(CWnd* pParent = NULL);   // standard constructor


private:
	CPicture	m_vfTopLeftPic;					///< ���Ͻ�ͼƬ
	CPicture	m_vfTopMiddlePic;				///< �ϲ��м�ͼƬ
	CPicture	m_vfTopRightPic;				///< ���Ͻ�ͼƬ
	CPicture	m_vfLeftPic;					///< ��߿�ͼƬ
	CPicture	m_vfRightPic;					///< �ұ߿�ͼƬ
	CPicture	m_vfBottomLeftPic;				///< �ײ����ͼƬ
	CPicture	m_vfBottomMiddlePic;			///< �ײ��м�ͼƬ
	CPicture	m_vfBottomRightPic;				///< �ײ��ұ�ͼƬ

	CPicture	m_NetStatusPic;					///< ����״̬ͼƬ
	CRect		m_rcNetStatus;					///< ����״̬ͼ����ʾ����

	CPicture	m_VideoBkPic;					///< ��Ƶ��������ͼƬ
	CBrush		m_bkBrush;						///< ������ɫˢ
	BOOL		m_bNeedShowBkPic;				///< �Ƿ���Ҫ��ʾ����ͼƬ

	CString		m_strTitle;						///< �����ַ���
	CRect		m_rcTitle;						///< ��������
	CRect		m_rcMainDisp;					///< ����ʾ����
	CRect		m_rcVideoDisp;					///< ��Ƶ��ʾ����
	BOOL		m_bRegualShowVideo;				///< ��������ʾ��Ƶ��������Ƶ�����Σ�

	DWORD		m_dwUserId;						///< ��ǰ��Ƶ��ʾ������������û�ID
	BOOL		m_bVideoState;					///< ������Ƶ״̬
	BOOL		m_bAudioState;					///< ������Ƶ״̬

	PCHAR		m_pLastVideoBuf;				///< ���������Ƶ֡����
	DWORD		m_dwLastVideoLen;				///< ���������Ƶ֡����
	BITMAPINFOHEADER	m_bmiLastVideo;			///< ���������Ƶͷ��Ϣ
	CRITICAL_SECTION	m_hBufSection;			///< ��Ƶ�������ٽ���

	DWORD		m_dwWidth;						///< ��ǰ�û���Ƶ���
	DWORD		m_dwHeight;						///< ��ǰ�û���Ƶ�߶�

	CFullScreenDlg*	m_pFullScreenDlg;			///< ��Ƶȫ���Ի���

private:
	// ���¶�λ����ؼ�
	void RelocateControls();
	// ��ʼ����ť
	void InitSTButton(CButtonST& btn, INT bitmapID, LPCTSTR lpTooltip);
	// ������Ƶ��ʾ��������Ƶ��һ���ı�����ʾ
	void AdjustVideoDispRect(CRect& rc);


public:
	// ���ñ�����ʾ
	void SetPanelTitle(CString strTitle);
	// ���ù����û����
	void SetRelateUserId(DWORD dwUserId);
	// ��ȡ��ǰ�������û����
	DWORD GetUserId(void)			{	return m_dwUserId;		}

	// �յ��ص���Ƶ����
	void OnRecvCallBackVideoData(LPVOID lpBuf, DWORD dwLen, BITMAPINFOHEADER bmiHeader);
	// �û���Ƶ�ֱ��ʱ仯
	void OnVideoSizeChange(DWORD dwWidth, DWORD dwHeight);

// Dialog Data
	//{{AFX_DATA(CVideoPanelDlg)
	enum { IDD = IDD_DIALOG_VIDEOPANEL };
	CButtonST	m_btnVideoCtrl;
	CButtonST	m_btnSnapshot;
	CButtonST	m_btnRecord;
	CButtonST	m_btnAudioCtrl;
	CGradientProgressCtrl	m_ctrlAudioVolume;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoPanelDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVideoPanelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnButtonVideoControl();
	afx_msg void OnButtonAudioControl();
	afx_msg void OnButtonSnapshot();
	afx_msg void OnButtonRecord();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnRefreshButtonState(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefreshVideo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitFullScreen(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOPANELDLG_H__15023AD4_E5B0_4A48_B448_699AD60A72DD__INCLUDED_)
