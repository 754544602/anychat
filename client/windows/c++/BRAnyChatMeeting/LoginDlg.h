#if !defined(AFX_LOGINDLG_H__F9FFC891_3D9F_4DE6_93DD_8A6A942BE50C__INCLUDED_)
#define AFX_LOGINDLG_H__F9FFC891_3D9F_4DE6_93DD_8A6A942BE50C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//
#include "BRAnyChatMeetingDlg.h"

class CBRAnyChatMeetingDlg;

class CLoginDlg : public CDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent);   // standard constructor

private:
	CBrush		m_bkBrush;				///< ����ˢ
	CBrush		m_bottomBrush;			///< �ײ�����ˢ
	CBrush		m_settingsBrush;		///< �������򱳾�ˢ
	CPicture	m_BannerPic;			///< ��¼�Ի��򶥲�ͼƬ

	BOOL		m_bSettingMode;			///< ��ǰ�Ƿ�Ϊ����ģʽ������ģʽ����ʾIP��ַ��˿ںţ�

	CBRAnyChatMeetingDlg*	m_pMainDlg;	///< ���Ի������

private:
	// �������ò���
	void LoadSettingsParam(void);


public:
	// ����������Ϣ֪ͨ
	void OnConnectNotify(DWORD dwErrorCode);
	// �û���¼��Ϣ֪ͨ
	void OnLoginNotify(DWORD dwErrorCode);
	// ���ӶϿ������ص�¼����
	void OnNeedShowLoginDlg(void);

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_DIALOG_LOGIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonSettings();
	afx_msg void OnButtonLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__F9FFC891_3D9F_4DE6_93DD_8A6A942BE50C__INCLUDED_)
