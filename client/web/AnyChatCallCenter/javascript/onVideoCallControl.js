// JavaScript Document

	// ����������붨��
    var BRAC_ERRORCODE_SUCCESS			= 0;	 ///< û�д���
    var BRAC_ERRORCODE_SESSION_QUIT		= 100101;///< Դ�û����������Ự
    var BRAC_ERRORCODE_SESSION_OFFLINE	= 100102;///< Ŀ���û�������
    var BRAC_ERRORCODE_SESSION_BUSY		= 100103;///< Ŀ���û�æ
    var BRAC_ERRORCODE_SESSION_REFUSE	= 100104;///< Ŀ���û��ܾ��Ự
    var BRAC_ERRORCODE_SESSION_TIMEOUT	= 100105;///< �Ự����ʱ
    var BRAC_ERRORCODE_SESSION_DISCONNECT=100106;///< �������
//�Ự��ʾ��Ϣ
function ForSession(message) {
    var mBrowserWidth = document.body.offsetWidth; // ��ҳ�ɼ������
    var mBrowserHeight = document.documentElement.clientHeight; //  ��ҳ�ɼ������
    CenterPosition(mBrowserWidth, mBrowserHeight, "SessionPrompt_Div", 300, 170); //�������в㴹ֱˮƽ����
    Getdmo("Shade_Div").style.display = "none";
    Getdmo("Initiative_Call_Div").style.display = "none";// �������ں��в�
	Getdmo("BeCalls_Div").style.display = "none";// ���ر����в�
	Getdmo("advanceset_div").style.display = "none"; // �������ò�
    Getdmo("VideoShowDiv").style.display = "none"; // �������ò�
	mTargetUserId=0;
    Getdmo("SessionPrompt_Div").style.color = "White";
    Getdmo("SessionPrompt_Div").innerHTML = message;
    $("#SessionPrompt_Div").fadeTo("slow", 1);
        setTimeout("$('#SessionPrompt_Div').fadeTo('slow', 0 , BusyDivOut)", 2000);
    //$("#SessionPrompt_Div").css("left", (document.body.offsetWidth - 600) / 2 + "px");
}

//ȡ����������
function CancelCall() {
    $("#Shade_Div").hide();
    $("#Initiative_Call_Div").hide();
	BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REPLY,mTargetUserId,BRAC_ERRORCODE_SESSION_QUIT,0,0,"");  
    ForSession("ȡ������...");
}
//�����û�˫��
function VideoCallRequest(ID) {
    if (mSelfUserId == ID)
	    ForSession("���ܺ����Լ�...");
    else {
        mTargetUserId = ID;
		BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REQUEST,mTargetUserId,0,0,0,"");  // ��ָ�����û����ͻỰ����
    }
}
//ͬ��Ự
function AcceptRequestBtnClick() {
	BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REPLY,mTargetUserId,0,0,0,"");  
    $("#BeCalls_Div").hide();
}
//�ܾ��Ự
function RejectRequestBtnClick() {
	BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REPLY,mTargetUserId,BRAC_ERRORCODE_SESSION_REFUSE,0,0,"");  
    $("#Shade_Div").hide();
    $("#BeCalls_Div").hide();
    ForSession("�ܾ��Է�����...");
}
//�յ���Ƶ��������
function onVideoCallControlRequest(dwUserId, dwErrorCode, dwFlags, dwParam, szUserStr)
{
	 var UserName = BRAC_GetUserInfo(dwUserId,USERINFO_NAME); // �û�����
	 $("#Shade_Div").show();
	 $("#BeCalls_Div").show();
	 $("#BeCalls_Div_Content").html("�յ��û�  " +UserName + "  �Ự����<br />      �Ƿ�ͬ��?");
	 mTargetUserId = dwUserId;
}

//��Ƶ��������ظ�
function onVideoCallControlReply(dwUserId, dwErrorCode, dwFlags, dwParam, szUserStr)
{
	
	switch(dwErrorCode)
	{
		case BRAC_ERRORCODE_SUCCESS:
		    onSendVideoCallRequestSucess(dwUserId);
			break;
		case BRAC_ERRORCODE_SESSION_QUIT:
			ForSession("Դ�û����������Ự");
			break;
		case BRAC_ERRORCODE_SESSION_OFFLINE:
		    ForSession("Ŀ���û�������");
			break;
		case BRAC_ERRORCODE_SESSION_BUSY:
			ForSession("Ŀ���û�æ");
			break; 
		case BRAC_ERRORCODE_SESSION_REFUSE:
		 	ForSession("Ŀ���û��ܾ��Ự");
			break; 
		case BRAC_ERRORCODE_SESSION_TIMEOUT:
		 	ForSession("�Ự����ʱ");
			break; 
		case BRAC_ERRORCODE_SESSION_DISCONNECT:
			ForSession("�������");
			break; 
		
	}
}

//ͨ����ʼ
function onVideoCallControlStart(dwUserId, dwErrorCode, dwFlags, dwParam, szUserStr)
{
	BRAC_EnterRoom(dwParam, "", 0);
	Getdmo("Initiative_Call_Div").style.display = "none";
	Getdmo("hall_div").style.display = "none";
	$("#VideoShowDiv").show();
}

//��Ƶͨ������
function onVideoCallControlFinish(dwUserId, dwErrorCode, dwFlags, dwParam, szUserStr)
{
	BRAC_LeaveRoom(-1);
    ForSession("�Ự����..."); // ��ʾ��
	ShowHallDiv(true); // ��ʾ����
    //clearInterval(mRefreshVolumeTimer); // �ر�����������ʾ
}

//��Ƶ���������ͳɹ�
function onSendVideoCallRequestSucess(mTargetUserId)
{
	
	var UserName = BRAC_GetUserInfo(mTargetUserId,USERINFO_NAME); // �û�����
    UserName = UserName.fontcolor("Red");
    Getdmo("Initiative_Call_Div_Content").innerHTML = "���ں���" + UserName + "�û����ȴ��Է���Ӧ<br /><img src='./images/Others/LoadImg.gif'    style='width: 145px;height:30px;' />";
    Getdmo("Shade_Div").style.display = "block";
    Getdmo("Initiative_Call_Div").style.display = "block";
}



function BusyDivOut() {
    $("#SessionPrompt_Div").css("top", "500%");
}



