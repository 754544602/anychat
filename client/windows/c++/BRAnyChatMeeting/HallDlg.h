#if !defined(AFX_HALLDLG_H__5C543137_12C1_46B0_B480_F6F7DB0BA607__INCLUDED_)
#define AFX_HALLDLG_H__5C543137_12C1_46B0_B480_F6F7DB0BA607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HallDlg.h : header file
//

#include "BRAnyChatMeetingDlg.h"

class CBRAnyChatMeetingDlg;

/**
 *	����������ʾ�࣬������ʾ��¼�ɹ�֮��ĸ��������Ϣ���Լ�������Ϣ
 */
class CHallDlg : public CDialog
{
// Construction
public:
	CHallDlg(CWnd* pParent);   // standard constructor


private:
	CBRAnyChatMeetingDlg*	m_pMainDlg;	
	BOOL	m_bWaitEnterRoom;		///< �Ƿ����ڵȴ����뷿�䣬��ֹ�û�����˫���������

public:
	// ���½����û���Ϣ
	void UpdateUserInfo(DWORD dwUserId);
	// ���뷿��״̬֪ͨ
	void OnEnterRoomNotify(DWORD dwRoomId, DWORD dwErrorCode);

private:
	// ��ʼ����������б�
	void InitRoomListCtrl(void);
	// ������������Ϣ
	void FillRoomInfoToList(void);


// Dialog Data
	//{{AFX_DATA(CHallDlg)
	enum { IDD = IDD_DIALOG_HALL };
	CListCtrl	m_ctrlRoomList;
	int		m_iRoomId;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHallDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnButtonExit();
	afx_msg void OnDblclkListRoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonEnterRoom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HALLDLG_H__5C543137_12C1_46B0_B480_F6F7DB0BA607__INCLUDED_)
