// BRAnyChatMeetingDlg.h : header file
//

#if !defined(AFX_BRANYCHATMEETINGDLG_H__C55CF205_B790_49C7_B91F_6035C8EFC030__INCLUDED_)
#define AFX_BRANYCHATMEETINGDLG_H__C55CF205_B790_49C7_B91F_6035C8EFC030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoginDlg.h"
#include "hallDlg.h"
#include "VideoPanelDlg.h"
#include "SettingsDlg.h"
#include <BRAnyChatSDKProc.h>

class CLoginDlg;
class CHallDlg;
class CVideoPanelDlg;
class CSettingsDlg;

class CBRAnyChatMeetingDlg : public CDialog,
							 public CBRAnyChatSDKProc
{
// Construction
public:
	CBRAnyChatMeetingDlg(CWnd* pParent = NULL);	// standard constructor


public:
	CLoginDlg*	m_pLoginDlg;			///< ��¼�Ի������
	CHallDlg*	m_pHallDlg;				///< ��������Ի������
	CSettingsDlg*	m_pSettingsDlg;		///< �߼����öԻ������

	CVideoPanelDlg*	m_pSelfVideoDlg;	///< ������Ƶ�Ի������
	CVideoPanelDlg*	m_pPanelObjectArray[MAX_VIDEOPANEL_NUM];		///< �����û���Ƶ�����ʾ��������


	DWORD		m_dwSelfUserId;			///< �Լ����û����

private:
	CImageList	m_UserStateImageList;	///< �û�״̬ͼ��
	CString		m_strTextOutput;		///< ���������Ϣ
	CPicture	m_LogoPic;				///< ������LogoͼƬ

private:
	// ��ʼ��AnyChat SDK
	void InitAnyChatSDK(void);
	// ��ʼ�������û��б�ؼ�
	void InitUserListCtrl(void);
	// ˢ�������û�����ʾ
	void RefreshOnlineUserNum(void);
	// ˢ��ָ���б���״̬
	void RefreshListItemState(DWORD dwItemNo, DWORD dwUserId);
	// �����µ��û����б���
	void AddNewUserToList(DWORD dwUserId);

	// ���ݷ�������λ��Ƶ�����ʾλ��
	void RelocateVideoPanel(DWORD dwSplitNum);
	// ˢ����Ƶ��ʾ���
	void RefreshVideoPanel(void);
	// ��ȡһ�����е���ʾ������
	DWORD GetEmptyVideoPanelIndex(void);
	// �ж�ָ���û�����Ƶ����Ƿ�����ʾ����
	BOOL IsUserVideoPanelShow(DWORD dwUserId);
	// ���û�����Ƶ���
	void BindUserAndVideoPanel(DWORD dwUserId, CVideoPanelDlg* lpVideoPanelDlg);
	// ȡ����Ƶ������û��İ�
	void CleanVideoPanelBind(CVideoPanelDlg* lpVideoPanelDlg, BOOL bErase=TRUE);

	// ��������Ϣ����������Ϣ
	void InsertStringToTextOutput(CString strMsg);

public:
	// �û��ر�ϵͳ
	void OnUserExitSystem(void);


protected:		// ����CBRAnyChatSDKProc����麯��

	// ���ӷ�������Ϣ
	virtual void OnAnyChatConnectMessage(BOOL bSuccess, DWORD dwErrorCode);
	// �û���½��Ϣ
	virtual void OnAnyChatLoginMessage(DWORD dwUserId, DWORD dwErrorCode);
	// �û����뷿����Ϣ
	virtual void OnAnyChatEnterRoomMessage(DWORD dwRoomId, DWORD dwErrorCode);
	// ���������û���Ϣ
	virtual void OnAnyChatOnlineUserMessage(DWORD dwUserNum, DWORD dwRoomId);
	// �û��õ�/�ͷ�mic��Ϣ
	virtual void OnAnyChatMicStateChgMessage(DWORD dwUserId, BOOL bGetMic);
	// �û�����/�˳�������Ϣ
	virtual void OnAnyChatUserAtRoomMessage(DWORD dwUserId, BOOL bEnter);
	//����Ͽ���Ϣ
	virtual void OnAnyChatLinkCloseMessage(DWORD dwReason, DWORD dwErrorCode);
	
	// �û�����ͷ״̬�ı���Ϣ
	virtual void OnAnyChatCameraStateChgMessage(DWORD dwUserId, DWORD dwState);
	// �û�����ģʽ�ı���Ϣ
	virtual void OnAnyChatChatModeChgMessage(DWORD dwUserId, BOOL bPublicChat);
	// �û��״̬�����仯��Ϣ
	virtual void OnAnyChatActiveStateChgMessage(DWORD dwUserId, DWORD dwState);
	// P2P����״̬�仯��Ϣ
	virtual void OnAnyChatP2PConnectStateMessage(DWORD dwUserId, DWORD dwState);
	// �û���Ƶ�ֱ��ʸı���Ϣ
	virtual void OnAnyChatVideoSizeChgMessage(DWORD dwUserId, DWORD dwWidth, DWORD dwHeight);
	
	// �û�˽��������Ϣ
	virtual void OnAnyChatPrivateRequestMessage(DWORD dwUserId, DWORD dwRequestId);
	// �û�˽������ظ���Ϣ
	virtual void OnAnyChatPrivateEchoMessage(DWORD dwUserId, DWORD dwErrorCode);
	// �û��˳�˽����Ϣ
	virtual void OnAnyChatPrivateExitMessage(DWORD dwUserId, DWORD dwErrorCode);
	
	// SDK������Ϣ
	virtual void OnAnyChatSDKWarningMessage(DWORD dwErrorCode, DWORD dwReserved);
	
	// ��Ƶ���ݻص�
	virtual void OnAnyChatVideoDataCallBack(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, LPBITMAPINFOHEADER lpbmiHeader);
	// ��Ƶ���ݻص�
	virtual void OnAnyChatAudioDataCallBack(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, LPWAVEFORMATEX lpWaveFormatEx);
	// �������ֵĻص�����
	virtual void OnAnyChatTextMsgCallBack(DWORD dwFromUserid, DWORD dwToUserid, BOOL bSecret, LPCTSTR lpMsgBuf, DWORD dwLen);
	// ͸��ͨ���ص�����
	virtual void OnAnyChatTransBufferCallBack(DWORD dwUserid, LPBYTE lpBuf, DWORD dwLen);
	// ͸��ͨ��������չ�ص�����
	virtual void OnAnyChatTransBufferExCallBack(DWORD dwUserid, LPBYTE lpBuf, DWORD dwLen, DWORD wParam, DWORD lParam, DWORD dwTaskId);
	// �ļ�����ص�����
	virtual void OnAnyChatTransFileCallBack(DWORD dwUserid, LPCTSTR lpFileName, LPCTSTR lpTempFilePath, DWORD dwFileLength, DWORD wParam, DWORD lParam, DWORD dwTaskId);
	// �����仯�ص�����
	virtual void OnAnyChatVolumeChangeCallBack(BRAC_AudioDevice device, DWORD dwCurrentVolume);
	// SDK Filter ͨ�����ݻص�����
	virtual void OnAnyChatSDKFilterDataCallBack(LPBYTE lpBuf, DWORD dwLen);
	// ¼�񡢿���������ɻص�����
	virtual void OnAnyChatRecordSnapShotCallBack(DWORD dwUserid, LPCTSTR lpFileName, DWORD dwParam, BOOL bRecordType);
	// �첽��Ϣ֪ͨ�ص�����
	virtual void OnAnyChatNotifyMessageCallBack(DWORD dwNotifyMsg, DWORD wParam, DWORD lParam);


// Dialog Data
	//{{AFX_DATA(CBRAnyChatMeetingDlg)
	enum { IDD = IDD_DIALOG_MEETING };
	CEdit	m_ctrlTextOutput;
	CComboBox	m_ctrlDispNumCombo;
	CListCtrl	m_ctrlUserList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBRAnyChatMeetingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBRAnyChatMeetingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSelchangeComboDispNum();
	afx_msg void OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSendText();
	afx_msg void OnButtonAdvSet();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRANYCHATMEETINGDLG_H__C55CF205_B790_49C7_B91F_6035C8EFC030__INCLUDED_)
