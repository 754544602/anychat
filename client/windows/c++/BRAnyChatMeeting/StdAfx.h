// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A8A3ACB9_38E8_4C26_B5ED_DAB9B0822D10__INCLUDED_)
#define AFX_STDAFX_H__A8A3ACB9_38E8_4C26_B5ED_DAB9B0822D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>

#include "Ctrls/Picture.h"
#include "Ctrls/memdc.h"
#include "Ctrls/BtnST.h"
//#include "Ctrls/NewLabel.h"

#include <BRAnyChatCoreSDK.h>
#pragma comment(lib,"BRAnyChatCore.lib")

BOOL IsFileExist(LPCTSTR lpFilePath);

#define   _HRC_USE_SKIN_					///< �Ƿ�ʹ��Ƥ��

//=======================
#ifdef _HRC_USE_SKIN_
#include "Ctrls\\SkinPPWTL.h"
#pragma comment(lib,"Ctrls\\SkinPPWTL.lib")
#endif
//========================

#define SUB_PATH_SKINFILE		_T("Skins")					///< Ƥ���ļ���Ŀ¼
#define DEFAULT_SKIN_FILENAME	_T("AlphaOS.ssk")			///< Ĭ��Ƥ���ļ���

#define DEFAULT_CONFIG_FILENAME _T("AnyChatSDK.ini")		///< Ĭ�ϱ��������ļ���
#define DEFAULT_CONFIG_SECTION	_T("Custom Settings")		///< �Զ�������

#define DEFAULT_SERVER_IPADDR	_T("demo.anychat.cn")		///< Ĭ�Ϸ�����IP��ַ
#define DEFAULT_SERVER_PORT		8906						///< Ĭ�Ϸ������˿ں�

#define MAX_DEMO_ROOM_NUM		4							///< �����ʾ������
#define MAX_VIDEOPANEL_NUM		4							///< �����Ƶ�����


//����ͷ״̬����
#define GV_CAMERA_STATE_NULL	0				///< û������ͷ
#define GV_CAMERA_STATE_NORMAL	1				///< ������ͷ��û�д�
#define GV_CAMERA_STATE_OPEN	2				///< ����ͷ�Ѵ�



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A8A3ACB9_38E8_4C26_B5ED_DAB9B0822D10__INCLUDED_)
