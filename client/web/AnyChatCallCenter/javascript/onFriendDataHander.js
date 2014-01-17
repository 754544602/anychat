// JavaScript Document
var mSelfUserId = -1; 							// �����û�ID
var mTargetUserId = 0;							// Ŀ���û�ID�������˶Է�������Ƶ��

var USER_ONLINE_STATUS=1;//�û�����
var USER_OFFLINE_STATUS=0;//�û�����

var USERINFO_NAME=1;//�û��ǳ�ѶϢ
var USERINFO_IP=2;//�û�IP��ַѶϢ
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
    var UserName = BRAC_GetUserInfo(userid,USERINFO_NAME); // �û�����
	var UserIp=BRAC_GetUserInfo(userid,USERINFO_IP); // �û�IP��ַ;
    var main_div = document.createElement("div");
    main_div.id = "UserID_" + userid;
    main_div.className = "OnLineUser_Div";

    var left_div = document.createElement("div");
    left_div.className = "UserImg_Holder";
    main_div.appendChild(left_div);

    var left_img = document.createElement("img");
    left_img.id = userid;
	var img_value=Math.abs(userid)%10;
    left_img.src = "./images/head/"+img_value+".gif";
    left_img.className = "UserImg";
    left_div.appendChild(left_img);

    var right_div = document.createElement("div");
    right_div.className = "UserInfo_Holder";
    main_div.appendChild(right_div);

    var right_name= document.createElement("div");
    right_name.className = "UserInfo";
    right_name.innerHTML = UserName;
    right_div.appendChild(right_name);
	
	var right_ip = document.createElement("div");
    right_ip.className = "UserInfo";
    right_ip.innerHTML = UserIp;
    right_div.appendChild(right_ip);
	
	var right_userId= document.createElement("div");
    right_userId.className = "UserInfo";
    right_userId.innerHTML = userid;
    right_div.appendChild(right_userId);
    
	   //���ز����ť��껮�뻮��ʱ��
    main_div.onmouseover = function () {
       main_div.style.backgroundColor = "#8FBC8B";
    }
    main_div.onmouseout = function () {
       main_div.style.backgroundColor = "#FAFADD";
    }
	main_div.onclick=function () { VideoCallRequest(userid); }
    Getdmo("UserListContent").appendChild(main_div);
}

//��������û�
function removeOfflineUser(dwUserId)
{
	Getdmo("UserListContent").removeChild(Getdmo("UserID_" + dwUserId));
}