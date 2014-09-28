// iCloudSoft Business Logic SDK

/********************************************
 *				�������岿��				*
 *******************************************/
// �ļ�����������ͣ�API��ICS_FileTransControl ���������
var ICS_FILETRANS_CTRLTYPE_OPENFILE		= 1;		// ���ļ�
var ICS_FILETRANS_CTRLTYPE_OPENDIR		= 2;		// ���ļ�����Ŀ¼

// �ļ��������Բ�ѯ��API��ICS_FileTransQueryXXXValue ���������
var ICS_FILETRANS_PROPERTY_SRCUSERID	= 1;		// ���ͷ��û�ID
var ICS_FILETRANS_PROPERTY_TARUSERID	= 2;		// ���շ��û�ID
var ICS_FILETRANS_PROPERTY_FILESIZE		= 3;		// �ļ���С
var ICS_FILETRANS_PROPERTY_FILENAME		= 4;		// �ļ���

// �û��������Զ���
var ICS_USEROBJECT_PROPERTY_NAME		= 1;		// �û���
var ICS_USEROBJECT_PROPERTY_TYPE		= 2;		// �û����
var ICS_USEROBJECT_PROPERTY_IMAGEID		= 3;		// �û�ͼ��ID
var ICS_USEROBJECT_PROPERTY_STATUS		= 4;		// �û��״̬
var ICS_USEROBJECT_PROPERTY_GROUPID		= 5;		// �û�����ID

// ����������Զ���
var ICS_GROUPOBJECT_PROPERTY_NAME		= 1;		// �û���������

// ����������Զ���
var ICS_MEETINGOBJECT_PROPERTY_GUID		= 1;		// ����GUID
var ICS_MEETINGOBJECT_PROPERTY_NAME		= 2;		// ��������
var ICS_MEETINGOBJECT_PROPERTY_THEME	= 3;		// ��������
var ICS_MEETINGOBJECT_PROPERTY_PASS		= 4;		// ��������
var ICS_MEETINGOBJECT_PROPERTY_STARTTIME =5;		// ���鿪ʼʱ��
var ICS_MEETINGOBJECT_PROPERTY_TYPE		= 6;		// ��������
var ICS_MEETINGOBJECT_PROPERTY_SPKMODE	= 7;		// ���鷢�Կ���ģʽ
var ICS_MEETINGOBJECT_PROPERTY_VIDEOCOUNT=8;		// ������Ƶ������
var ICS_MEETINGOBJECT_PROPERTY_STATUS	= 9;		// ����״̬
var ICS_MEETINGOBJECT_PROPERTY_OWNER	= 10;		// ���鴴����ID
var ICS_MEETINGOBJECT_PROPERTY_STYLE	= 11;		// ���������

// ��������
var ICS_DATATYPE_ONLINEUSERS 			= 1; 		// �û���Ϣ
var ICS_DATATYPE_MEETINGS				= 2; 		// ������Ϣ
var ICS_DATATYPE_FRIENDS 				= 3; 		// �����б�
var ICS_DATATYPE_GROUPS 				= 4; 		// �û�����
	
var ICS_STATUSTYPE_USERONLINE			= 1;		// �û�����״̬
var ICS_STATUSTYPE_MEETTYPECHG			= 50;		// ����״̬�ı�
var ICS_STATUSTYPE_MEETDELETE			= 51;		// ���鱻ɾ��
var ICS_STATUSTYPE_MEETSPKMODE			= 52;		// ���鷢��ģʽ�ı�
var ICS_STATUSTYPE_MICREFRESH			= 53;		// ��������ˢ��



// ͨ��ָ��ش��붨��
var ICS_RETCODE_SUCCESS					= 0;		// �ɹ�����������״̬
var ICS_RETCODE_SESSION_CANCEL			= 100101;	// Դ�û����������Ự
var ICS_RETCODE_SESSION_OFFLINE			= 100102;	// Ŀ���û�������
var	ICS_RETCODE_SESSION_BUSY			= 100103;	// Ŀ���û�æ
var ICS_RETCODE_SESSION_REJECT			= 100104;	// Ŀ���û��ܾ��Ự
var ICS_RETCODE_SESSION_TIMEOUT			= 100105;	// �Ự����ʱ
var ICS_RETCODE_FILETRANS_REJECT		= 100201;	// Ŀ���û��ܾ������ļ�
var ICS_RETCODE_FILETRANS_CANCEL		= 100202;	// �ļ����䱻�Է�ȡ��
var ICS_RETCODE_FILETRANS_REPEAT		= 100203;	// �ļ����ڴ��䣬�����ظ�����
var ICS_RETCODE_FILETRANS_OPENFAIL		= 100204;	// �ļ���ʧ�ܣ����ܱ�ռ��
var ICS_RETCODE_MEETING_PASSERR			= 100301;	// �������벻��ȷ
var ICS_RETCODE_MEETING_NOTINVIT		= 100302;	// û�б�����
var ICS_RETCODE_MEETING_LOCKED			= 100303	// ���鴦�ڼ���״̬



/********************************************
 *				�������岿��				*
 *******************************************/
var icloudsoft;									// iCloudSoft���DMO����

// ��ʼ��SDK�����س������
function ICS_InitSDK() {
	try {
		icloudsoft = anychat;
		var ANYCHATWEB_SO_ENABLEICSLOGIC = 11001; 
		anychat.SetSDKOptionInt(ANYCHATWEB_SO_ENABLEICSLOGIC, 1);
		// �����ص��¼�
		if(window.ActiveXObject) {
			icloudsoft.attachEvent('OnSessionRequest', OnICloudSoftSessionRequest);
			icloudsoft.attachEvent('OnSessionStart', OnICloudSoftSessionStart);
			icloudsoft.attachEvent('OnSessionFinish', OnICloudSoftSessionFinish);
			icloudsoft.attachEvent('OnStatusNotify', OnICloudSoftStatusNotify);
			icloudsoft.attachEvent('OnRecvDataFinish', OnICloudSoftRecvDataFinish);
			icloudsoft.attachEvent('OnRecvObjectData', OnICloudSoftRecvObjectData);
			icloudsoft.attachEvent('OnFileTransRequest', OnICloudSoftFileTransRequest);
			icloudsoft.attachEvent('OnFileTransReply', OnICloudSoftFileTransReply);
			icloudsoft.attachEvent('OnFileTransStart', OnICloudSoftFileTransStart);
			icloudsoft.attachEvent('OnFileTransFinish', OnICloudSoftFileTransFinish);
		} else {
			icloudsoft.OnSessionRequest = OnICloudSoftSessionRequest;
			icloudsoft.OnSessionStart = OnICloudSoftSessionStart;
			icloudsoft.OnSessionFinish = OnICloudSoftSessionFinish;
			icloudsoft.OnStatusNotify = OnICloudSoftStatusNotify;
			icloudsoft.OnRecvDataFinish = OnICloudSoftRecvDataFinish;
			icloudsoft.OnRecvObjectData = OnICloudSoftRecvObjectData;
			icloudsoft.OnFileTransRequest = OnICloudSoftFileTransRequest;
			icloudsoft.OnFileTransReply = OnICloudSoftFileTransReply;
			icloudsoft.OnFileTransStart = OnICloudSoftFileTransStart;
			icloudsoft.OnFileTransFinish = OnICloudSoftFileTransFinish;
		}
		return GV_ERR_SUCCESS;
	}
	catch (e) {
	    return -1;
	}
}

/********************************************
 *				�Ự��������				*
 *******************************************/

// ����Ự
function ICS_SessionRequest(dwUserId, dwParam) {
	return icloudsoft.SessionRequest(dwUserId, dwParam);
}
// ��ʼ�Ự
function ICS_SessionStart(dwUserId) {
	return icloudsoft.SessionStart(dwUserId);
}
// �����Ự
function ICS_SessionFinish(dwUserId, dwErrorCode) {
	return icloudsoft.SessionFinish(dwUserId, dwErrorCode);
}


/********************************************
 *				������Ϣ����				*
 *******************************************/
 
// ��ȡ����ID�б�����һ������ID���飩
function ICS_GetObjectIds(dwObjectType) {
	var idarray = new Array();
	var size = icloudsoft.PrepareFetchObjectIds(dwObjectType);
	for(var i=0; i<size; i++) {
		var objectid = icloudsoft.FetchNextObjectId(dwObjectType);
		idarray[i] = objectid;
	}
	return idarray;
}

// ��ȡ�����û�������һ��dwUserId������
function ICS_GetOnlineUserIds() {
	return ICS_GetObjectIds(ICS_DATATYPE_ONLINEUSERS);
}
// ��ȡ�û����飬����һ��groupid������
function ICS_GetGroupIds() {
	return ICS_GetObjectIds(ICS_DATATYPE_GROUPS);
}
// ��ȡ�����б�����һ��meetingid������
function ICS_GetMeetingIds() {
	return ICS_GetObjectIds(ICS_DATATYPE_MEETINGS);
}

// ��ȡ�������Ͳ���ֵ
function ICS_GetObjectIntValue(dwObjectType, dwObjectId, dwInfoName) {
	return icloudsoft.GetObjectIntValue(dwObjectType, dwObjectId, dwInfoName);
}
// ��ȡ�����ַ�������ֵ
function ICS_GetObjectStringValue(dwObjectType, dwObjectId, dwInfoName) {
	return icloudsoft.GetObjectStringValue(dwObjectType, dwObjectId, dwInfoName);
}


// ��ȡ�û���
function ICS_GetUserName(dwUserId) {
	return ICS_GetObjectStringValue(ICS_DATATYPE_ONLINEUSERS, dwUserId, ICS_USEROBJECT_PROPERTY_NAME);
}
// ��ȡ�û�����
function ICS_GetUserType(dwUserId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_ONLINEUSERS, dwUserId, ICS_USEROBJECT_PROPERTY_TYPE);
}
// ��ȡ�û�ͼ��ID
function ICS_GetUserImageId(dwUserId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_ONLINEUSERS, dwUserId, ICS_USEROBJECT_PROPERTY_IMAGEID);
}
// ��ȡ�û�����״̬
function ICS_GetUserStatus(dwUserId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_ONLINEUSERS, dwUserId, ICS_USEROBJECT_PROPERTY_STATUS);
}
// ��ȡ�û�����ID
function ICS_GetUserGroupId(dwUserId){
	return ICS_GetObjectIntValue(ICS_DATATYPE_ONLINEUSERS, dwUserId, ICS_USEROBJECT_PROPERTY_GROUPID);
}

// ��ȡ�û�������
function ICS_GetGroupName(dwGroupId){
	return ICS_GetObjectStringValue(ICS_DATATYPE_GROUPS, dwGroupId, ICS_GROUPOBJECT_PROPERTY_NAME);
}


// ��ȡ����GUID
function ICS_GetMeetingGuid(dwMeetingId) {
	return ICS_GetObjectStringValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_GUID);
}
// ��ȡ��������
function ICS_GetMeetingName(dwMeetingId) {
	return ICS_GetObjectStringValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_NAME);
}
// ��ȡ��������
function ICS_GetMeetingTheme(dwMeetingId) {
	return ICS_GetObjectStringValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_THEME);
}
// ��ȡ���鿪ʼʱ��
function ICS_GetMeetingStartTime(dwMeetingId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_STARTTIME);
}
// ��ȡ��������
function ICS_GetMeetingType(dwMeetingId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_TYPE);
}
// ��ȡ���鷢�Կ���ģʽ
function ICS_GetMeetingType(dwMeetingId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_SPKMODE);
}
// ��ȡ������Ƶ������
function ICS_GetMeetingType(dwMeetingId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_VIDEOCOUNT);
}
// ��ȡ����״̬
function ICS_GetMeetingStatus(dwMeetingId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_STATUS);
}
// ��ȡ���鴴�����û�ID
function ICS_GetMeetingOwnerUserId(dwMeetingId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_OWNER);
}
// ��ȡ���������
function ICS_GetMeetingStyle(dwMeetingId) {
	return ICS_GetObjectIntValue(ICS_DATATYPE_MEETINGS, dwMeetingId, ICS_MEETINGOBJECT_PROPERTY_STYLE);
}


/********************************************
 *				�ļ����䷽��				*
 *******************************************/
 
// �ļ���������
function ICS_FileTransRequest(dwUserId, szRequestGuid, szFilePathName) {
	return icloudsoft.FileTransRequest(dwUserId, szRequestGuid, szFilePathName);
}
// �ļ���������ظ�
function ICS_FileTransReply(szRequestGuid, dwErrorCode) {
	return icloudsoft.FileTransReply(szRequestGuid, dwErrorCode);
}
// �ļ��������Ϊ
function ICS_FileTransSaveAs(szRequestGuid, szSavePath) {
	return icloudsoft.FileTransSaveAs(szRequestGuid, szSavePath);
}
// �ļ�����
function ICS_FileTransControl(szRequestGuid, dwCtrlType) {
	return icloudsoft.FileTransControl(szRequestGuid, dwCtrlType);
}
// ��ѯ�ļ��������β���ֵ
function ICS_FileTransQueryIntValue(szRequestGuid, dwInfoName) {
	return icloudsoft.FileTransQueryIntValue(szRequestGuid, dwInfoName);
}
// ��ѯ�ļ������ַ�������ֵ
function ICS_FileTransQueryStringValue(szRequestGuid, dwInfoName) {
	return icloudsoft.FileTransQueryStringValue(szRequestGuid, dwInfoName);
}
// ��ѯ�ļ�������ȣ�0 ~ 100��
function ICS_FileTransQueryProgress(szRequestGuid) {
	return ICS_FileTransQueryIntValue(szRequestGuid, BRAC_TRANSTASK_PROGRESS);
}
// ��ѯ�ļ��������ʣ�bps��
function ICS_FileTransQueryBitrate(szRequestGuid) {
	return ICS_FileTransQueryIntValue(szRequestGuid, BRAC_TRANSTASK_BITRATE);
}
// ��ָ���ļ�
function ICS_FileTransOpenFile(szRequestGuid) {
	return ICS_FileTransControl(szRequestGuid, ICS_FILETRANS_CTRLTYPE_OPENFILE);
}
// ���ļ�����Ŀ¼
function ICS_FileTransOpenDirectory(szRequestGuid) {
	return ICS_FileTransControl(szRequestGuid, ICS_FILETRANS_CTRLTYPE_OPENDIR);
}
// ��ѯ�ļ����������Ӧ���ļ���
function ICS_FileTransGetFileName(szRequestGuid) {
	return ICS_FileTransQueryStringValue(szRequestGuid, ICS_FILETRANS_PROPERTY_FILENAME);
}
// ��ѯ�ļ����������Ӧ���ļ���С
function ICS_FileTransGetFileSize(szRequestGuid) {
	return ICS_FileTransQueryIntValue(szRequestGuid, ICS_FILETRANS_PROPERTY_FILESIZE);
}
// ��ѯ�ļ����������ͷ��û�ID
function ICS_FileTransGetSendUserId(szRequestGuid) {
	return ICS_FileTransQueryIntValue(szRequestGuid, ICS_FILETRANS_PROPERTY_SRCUSERID);
}
// ��ѯ�ļ�����������շ��û�ID
function ICS_FileTransGetRecvUserId(szRequestGuid) {
	return ICS_FileTransQueryIntValue(szRequestGuid, ICS_FILETRANS_PROPERTY_TARUSERID);
}




