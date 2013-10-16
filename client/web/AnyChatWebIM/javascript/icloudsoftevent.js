// iCloudSoft Business Logic  SDK

/********************************************
 *				�¼��ص�����				*
 *******************************************/

 // �����û�ID��ȡ��������
 function GetUserGroupIdByUserId(userid) {
	var groupid = 0;
	var grouplist = BRAC_GetUserGroups();
	for(var i=0; i<grouplist.length; i++) {
		var friendlist = BRAC_GetGroupFriends(grouplist[i]);
		for(var j=0; j<friendlist.length; j++) {
			if(friendlist[j] == userid) {
				return grouplist[i];
			}
		}
	}
	return 0;
 }
 
 
// �û��Ự����
function OnICloudSoftSessionRequest(dwSrcUserId, dwTargetUserId, wParam) {
    $("#Shade_Div").show();
    $("#BeCalls_Div").show();
    $("#BeCalls_Div_Content").html("�յ��û�  " + BRAC_GetUserName(dwSrcUserId) + "  �Ự����<br />      �Ƿ�ͬ��?");
    mTargetUserId = dwSrcUserId;
}
// �û��Ự��ʼ
function OnICloudSoftSessionStart(dwSrcUserId, dwTargetUserId, dwRoomId) {
    mMissionQuantity = 0;
    Getdmo("ReceiveMsg").innerHTML = "";
    Getdmo("Initiative_Call_Div").style.display = "none";
    Getdmo("HallDiv").style.display = "none";
    chatuserid = dwSrcUserId;
    $("#VideoShowDiv").show();
    $("#mTargetPhoto").html("<img src='" + GetUserImageAddrById(mTargetUserId, 50) + "' alt='�û�ͷ��' style='height:65px;width:65px;' />");
    $("#mTargetInfo").html("Ŀ���ID��" + mTargetUserId + "<br />Ŀ�����ƣ�" + BRAC_GetUserName(mTargetUserId) + "<br />�������飺" + BRAC_GetGroupName(GetUserGroupIdByUserId(mTargetUserId)) + "<br />");
    //    ComboboxInint();
    InitAdvanced();
    ReceiveMsgBoxScroll();
}
// �û��Ự����
function OnICloudSoftSessionFinish(dwSrcUserId, dwTargetUserId, dwErrorCode) {
    if (dwErrorCode == ICS_RETCODE_SESSION_REJECT) {
        ForSession("�Է��ܾ��Ự����...");
    }
    else if (dwErrorCode == ICS_RETCODE_SESSION_BUSY) {
        ForSession("�û����ڻỰ��...");
    }
    else if (dwErrorCode == ICS_RETCODE_SESSION_CANCEL) {
        ForSession("�û�ȡ���Ự����...");
        Getdmo("BeCalls_Div").style.display = "none";
    }
    else
        BackToHall();
}
// ״̬֪ͨ
function OnICloudSoftStatusNotify(dwDataType, dwObjectId, dwStatus, wParam, lParam, sParam) {

}
// �����������
function OnICloudSoftRecvDataFinish(dwDataType, dwParam) {

}
// �յ�����
function OnICloudSoftRecvObjectData(dwDataType, dwObjectId) {
    
}
// �ļ���������
function OnICloudSoftFileTransRequest(dwSrcUserId, szRequestGuid, dwFileSize, szFileName) {
    MessageFormat();
    MessageFlicker();
    var main_div = document.createElement("div");
    main_div.id = "Receive" + szRequestGuid;
    main_div.className = "ReceiveInform";
    main_div.setAttribute("Receiving", "false");
    main_div.setAttribute("Belong", szRequestGuid);
    main_div.innerHTML = "&nbsp&nbsp�ļ���:" + szFileName +
                          "<br />&nbsp&nbsp�ļ���С: " + UnitConversion(dwFileSize, "MB", "KB", "B", 1) + "<br />";
    Getdmo("ReceiveMsg").appendChild(main_div);

    var under_receive = document.createElement("a");
    under_receive.id = "Receive" + szRequestGuid;
    under_receive.style.marginLeft = "10px";
    under_receive.innerHTML = "����";
    under_receive.onclick = function () {
        ICS_FileTransReply(szRequestGuid, 0) //���ջظ�
    }
    main_div.appendChild(under_receive);

    var under_saveAs = document.createElement("a");
    under_saveAs.id = "SaveAs" + szRequestGuid;
    under_saveAs.setAttribute("Belong", dwSrcUserId);
    under_saveAs.style.marginLeft = "5px";
    under_saveAs.innerHTML = "���Ϊ";
    under_saveAs.onclick = function () {
        if (ICS_FileTransSaveAs(szRequestGuid, "") == 0)
            ICS_FileTransReply(szRequestGuid, 0);
    }
    main_div.appendChild(under_saveAs);

    var under_refuse = document.createElement("a");
    under_refuse.id = "Refuse" + dwSrcUserId;
    under_refuse.setAttribute("Belong", dwSrcUserId);
    under_refuse.style.marginLeft = "5px";
    under_refuse.innerHTML = "�ܾ�";
    under_refuse.onclick = function () {
        ICS_FileTransReply(szRequestGuid, ICS_RETCODE_FILETRANS_REJECT); //�ܾ��ظ�
        $("#Receive" + szRequestGuid).html("&nbsp&nbsp���ܾ��˶Է��ļ�: " + ViewFileDetail(szRequestGuid) + " �ķ�������...");
        $("#Receive" + szRequestGuid).css("color", "#999999");
    }
    main_div.appendChild(under_refuse);
    ReceiveMsgBoxScroll();
}
// ��ʼ���� ��ʾ��������
function DisplayProgress(guid, id) {
    // ���ɽ�������
    // ���������
    var upper_div = document.createElement("div");
    upper_div.className = "ProgressBorder";
    Getdmo(id).appendChild(upper_div);
    // ������ɫ��
    var img = document.createElement("img");
    img.id = "TheProgress" + guid;
    img.src = "./images/dialog/Progress.png";
    img.style.width = "0px";
    img.style.marginTop = "4px";
    upper_div.appendChild(img);

    var under_div = document.createElement("div");
    under_div.style.width = "100%";
    Getdmo(id).appendChild(under_div);

    var under_div_space = document.createElement("div");
    under_div_space.className = "Bitrate" + guid;
    under_div_space.style.styleFloat = "left";
    under_div_space.style.width = "45%";
    under_div.appendChild(under_div_space);

    var under_div_cancel = document.createElement("div");
    under_div_cancel.id = "Discontinue" + guid;
    under_div_cancel.className = "CancelTransmit";
    under_div_cancel.innerHTML = "ȡ��";
    under_div_cancel.onclick = function () {
        ICS_FileTransReply(guid, ICS_RETCODE_FILETRANS_CANCEL);
        ReplyMsgStyle(id, "����ֹ���ļ�: " + ViewFileDetail(guid) + "�ķ���...", true);
        ReceiveMsgBoxScroll();
    }
    under_div.appendChild(under_div_cancel);
}
// �ļ�����ظ�
function OnICloudSoftFileTransReply(szRequestGuid, dwErrorCode) {
    if (dwErrorCode == ICS_RETCODE_FILETRANS_REJECT) { // �Է��ܾ�����
        ReplyMsgStyle("SendFileMsg" + szRequestGuid, "&nbsp&nbsp�Է��ܾ������ļ� " + ViewFileDetail(szRequestGuid) + "...<br />&nbsp&nbsp����ʧ��", true);
    }
    else if (dwErrorCode == ICS_RETCODE_FILETRANS_CANCEL) { // �ļ����䱻ȡ��
        if (ICS_FileTransGetSendUserId(szRequestGuid) == mTargetUserId)//�ж�����һ��ȡ������
            ReplyMsgStyle("Receive" + szRequestGuid, "&nbsp&nbsp�Է�ȡ�����ļ� " + ViewFileDetail(szRequestGuid) + " �Ĵ���...<br />&nbsp&nbsp����ʧ��", true);
        else
            ReplyMsgStyle("SendFileMsg" + szRequestGuid, "&nbsp&nbsp�Է��ж��� " + ViewFileDetail(szRequestGuid) + " �Ľ���...<br />&nbsp&nbsp����ʧ��", true);
    }
}
// ��ʼ�ļ�����
function OnICloudSoftFileTransStart(szRequestGuid) {
    mMissionQuantity++;
    if (mMissionQuantity == 1) mTransTimer = setInterval(ProgressBar, 300);
    var receiver_id = ICS_FileTransGetRecvUserId(szRequestGuid); // ���ͷ�ID
    var sender_id = ICS_FileTransGetSendUserId(szRequestGuid); // ���շ�ID
    if (sender_id == mSelfUserId) {
        ReplyMsgStyle("SendFileMsg" + szRequestGuid, "��ʼ�����ļ� " + ViewFileDetail(szRequestGuid) + "...", false);
        DisplayProgress(szRequestGuid, "SendFileMsg" + szRequestGuid);
    }
    else {
        ReplyMsgStyle("Receive" + szRequestGuid, "��ʼ�����ļ� " + ViewFileDetail(szRequestGuid) + "...", false);
        $("#Receive" + szRequestGuid).attr("Receiving", "true"); //�������ڲ�Ϊ���ڽ���״̬
        DisplayProgress(szRequestGuid, "Receive" + szRequestGuid);
    }
}
// ��̬�ı������
function ProgressBar() {
    $(".Receive").each(function () {
        if ($(this).attr("Receiving") == "true") {
            $("#TheProgress" + $(this).attr("Belong")).css("width", ICS_FileTransQueryProgress($(this).attr("Belong")) * 2 + "px");
        }
    });
}
// �ļ��������
function OnICloudSoftFileTransFinish(szRequestGuid) {
    var receiver_id = ICS_FileTransGetRecvUserId(szRequestGuid); // ���ͷ�ID
    var sender_id = ICS_FileTransGetSendUserId(szRequestGuid); // ���շ�ID
    if (sender_id == mSelfUserId) // ���ͷ�
        OperationFile(szRequestGuid, "SendFileMsg");
    else  // ���շ�
        OperationFile(szRequestGuid, "Receive");
}
// ������� ��ʾ ���ļ�  ��Ŀ¼ ��ť
function OperationFile(szRequestGuid, id) {
    Getdmo(id + szRequestGuid).innerHTML = "";
    var Result;
    if (id == "SendFileMsg") Result = ") �������.<br />";
    else Result = ") �������.<br />";
    var left_div = document.createElement("div");
    left_div.style.styleFloat = "left";
    left_div.style.cssFloat = "left";
    Getdmo(id + szRequestGuid).appendChild(left_div);
    var left_img = document.createElement("img");
    left_img.src = "./images/dialog/OpenFolder.png";
    left_img.style.width = "50px";
    left_img.style.height = "50px";
    left_div.appendChild(left_img);

    var right_div = document.createElement("div");
    right_div.style.fontSize = "12px";
    right_div.innerHTML = "&nbsp&nbsp�ļ� " + ICS_FileTransGetFileName(szRequestGuid) + "<br />&nbsp&nbsp��С (" +
                          UnitConversion(ICS_FileTransGetFileSize(szRequestGuid), "MB", "KB", "B", 1) + Result;
    Getdmo(id + szRequestGuid).appendChild(right_div);

    var open_file = document.createElement("a");
    open_file.className = "OperateFile";
    open_file.style.marginLeft = "10px";
    open_file.innerHTML = "���ļ�";
    open_file.onclick = function () {
        ICS_FileTransOpenFile(szRequestGuid);
    }
    Getdmo(id + szRequestGuid).appendChild(open_file);

    var open_path = document.createElement("a");
    open_path.className = "OperateFile";
    open_path.innerHTML = "��Ŀ¼";
    open_path.onclick = function () {
        ICS_FileTransOpenDirectory(szRequestGuid);
    }
    Getdmo(id + szRequestGuid).appendChild(open_path);
    Getdmo(id + szRequestGuid).style.height = "55px";
}

// ��ȡ�û�ͼ��
function GetUserImageAddrById(userid, type) {
    var imageid = parseInt(BRAC_GetUserInfo(userid, 4));
    if (type == 20)
        return "./images/avatar/20/" + imageid + ".gif";
    else if (type == 30)
        return "./images/avatar/30/" + imageid + ".gif";
    else
        return "./images/avatar/50/" + imageid + ".gif";
}

//�ж��յ��ַ������Ƿ���[]  ��ñ���
function Expression(message, type) {
    var reg = /\[(.*?)\]/gi; //�ж��ַ�[]��������
    var tmp = message.match(reg); //����յ���Ϣ���Ƿ����[]
    if (tmp) {//����[]
        for (var i = 0; i < tmp.length; i++) {//ѭ��ȡ��ÿ��[]
            var GetAttr = tmp[i].replace(reg, "$1"); //��ȡ[]�е�����
            for (var j = 0; j < mExpressionArray.length; j++) {//���[]�����Ƿ����������ж�Ӧ�ַ�
                if (mExpressionArray[j].split(":")[0] == GetAttr) {//�ж�Ӧ�ַ�
                    message = message.replace(tmp[i], "<img src='./images/emotion/" + mExpressionArray[j].split(":")[1] + ".gif' />"); //������Ҫ����ַ�ת��ΪͼƬ������ʾ
                    if (type == 0)
                        $("#SendMsg").html(message);
                    break;
                }
            }
        }
    }
    return message;
}

//�Ự��ʾ��Ϣ
function ForSession(message) {
    var VHeight = document.body.clientHeight; //  (�������ߵĸ�)
    $("#SessionPrompt_Div").css("top", "255px"); //������ʾ�㴹ֱλ��
    Getdmo("Shade_Div").style.display = "none";
    Getdmo("Initiative_Call_Div").style.display = "none";
    Getdmo("SessionPrompt_Div").style.color = "White";
    Getdmo("SessionPrompt_Div").innerHTML = message;
    $("#SessionPrompt_Div").fadeTo("slow", 1);
        setTimeout("$('#SessionPrompt_Div').fadeTo('slow', 0 , BusyDivOut)", 2000);
    $("#SessionPrompt_Div").css("left", (document.body.offsetWidth - 600) / 2 + "px");
}
function BusyDivOut() {
    $("#SessionPrompt_Div").css("top", "500%");
}

// ���ɷ����б����
function UserGroupDiv(group_id, content) {
    var main_div = document.createElement("div"); // ��������һ���²�
    main_div.style.width = "290px";
    main_div.style.height = "35px";
    main_div.style.overflow = "hidden";
    Getdmo("GroupContent").appendChild(main_div);

    var group_div_left = document.createElement("div"); // �²�������󸡶��� �����û���ͼ��
    group_div_left.style.textAlign = "center";
    group_div_left.style.width = "85px";
    group_div_left.style.height = "27px";
    group_div_left.style.styleFloat = "left"; // IE
    group_div_left.style.cssFloat = "left"; // firefox
    main_div.appendChild(group_div_left);

    var left_image = document.createElement("img"); // ��� �û���ͼ��
    left_image.className = "GroupImg";
    if (group_id == 0) left_image.src = "./images/Hall/1.png";
    else left_image.src = "./images/Hall/2.png";
    group_div_left.appendChild(left_image);

    var group_div_right = document.createElement("div"); // �²�������Ҹ����� �����û�������
    if (group_id == 0) group_div_right.id = "Group0";
    else group_div_right.id = "Group" + group_id;
    group_div_right.className = "OnLineUserRow";
    group_div_right.onclick = function () {
        OpenGroup(group_id);
    }
    group_div_right.style.width = "127px";
    group_div_right.style.height = "27px";
    group_div_right.style.styleFloat = "left"; // IE
    group_div_right.style.cssFloat = "left"; // firefox
    group_div_right.innerHTML = content;
    main_div.appendChild(group_div_right);
}


//�������ߺ���ͼ��
function CreateUserImage(type) {
    Getdmo("UserListContent").innerHTML = "";
    var OnLineUserList = BRAC_GetUserFriends(); // ��ȡ���к���
    if (type == "whole") { // ���������û�
        DisplayOnLineUser(mSelfUserId); // �ڵ�һ��λ�ô����Լ���ͼ��
        for (var i = 0; i < OnLineUserList.length; i++) {
            if (OnLineUserList[i] != mSelfUserId && BRAC_GetFriendStatus(OnLineUserList[i]) != 0)	// ֻ��ʾ�����û�
                DisplayOnLineUser(OnLineUserList[i]);
        }
    }
    else { // ָ���û��������û�
        for (var i = 0; i < OnLineUserList.length; i++) {
            var UserGroupID = GetUserGroupIdByUserId(OnLineUserList[i]);
            if (UserGroupID == mCurrentGroupNum && BRAC_GetFriendStatus(OnLineUserList[i]) != 0)	// ֻ��ʾ�����û�
                DisplayOnLineUser(OnLineUserList[i]);
        }
    }
    StartScroll("UserListScroll", "UserListSlider", "UserListContent", "UserListBaseLine");
}
//��ʾ�����û�
function DisplayOnLineUser(userid) {
    var UserName = BRAC_GetUserName(userid); // �û�����
    var main_div = document.createElement("div");
    main_div.id = "UserID_" + userid;
    main_div.className = "OnLineUser_Div";

    var left_div = document.createElement("div");
    left_div.className = "UserImg";
    main_div.appendChild(left_div);

    var left_img = document.createElement("img");
    left_img.id = userid;
    left_img.src = GetUserImageAddrById(userid, 30);
    left_img.onclick = function () { ImgDBclick(userid); }
    left_img.className = "User";
    left_div.appendChild(left_img);

    var right_div = document.createElement("div");
    right_div.className = "UserMsg";
    main_div.appendChild(right_div);

    var right_a = document.createElement("a");
    right_a.className = "UserNameMsg";
    right_a.innerHTML = UserName;
    right_div.appendChild(right_a);

    Getdmo("UserListContent").appendChild(main_div);
}
//�û�������
function OpenGroup(GroupID) {
    for (var j = 0; j < mGroupList.length; j++) {//���������鰴ťΪδѡ��״̬
        Getdmo("Group" + mGroupList[j]).style.background = "url(./images/Hall/05.png)"; //��ӱ���ͼƬ
        Getdmo("Group" + mGroupList[j]).style.width = "127px";
    }
    Getdmo("Group0").style.background = "url(./images/Hall/05.png)"; //��ӱ���ͼƬ
    Getdmo("Group0").style.width = "127px";
    if (GroupID == 0) {//���ݴ�������GroupID���ø���Ϊѡ��״̬
        Getdmo("Group0").style.background = "url(./images/Hall/03.png)";
        Getdmo("Group0").style.width = "165px";
    }
    else {
        Getdmo("Group" + GroupID).style.background = "url(./images/Hall/03.png)";  //���ݴ�������GroupID���ø���Ϊѡ��״̬
        Getdmo("Group" + GroupID).style.width = "165px";
    }
    mCurrentGroupNum = GroupID; //���õ�ǰѡ����Ϊȫ�ֱ���  �����Ҫ�õ���ǰѡ����ID
    if (GroupID == 0) {//�����û�  ȫ����
        CreateUserImage("whole");
    }
    else {
        CreateUserImage("appoint");
    }
}