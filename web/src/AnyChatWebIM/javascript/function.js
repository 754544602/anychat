var mSelfUserName = ""; //�˺�
var mSelfPassWord = ""; //����
var mTargetUserId = "";//��Ƶ�Ự �Է�ID
var mSelfUserId; //����Լ���ID
var mRefreshVolumeTimer; // ʵʱ������С��ʱ��
var mCurrentGroupNum = 0; //��õ�ǰѡ�е��û�����ID
var mGroupList = new Array(); //������Ϣ�б�
var mTransTimer; // ��ʱ��
var mMissionQuantity;  // �ļ�������������
var mFlickerNews; // ����Ϣ��˸��ʾ
var mWhetherMainForm = false;

function Getdmo(element) {
    if (document.getElementById) {
        return document.getElementById(element);
    } else if (window[element]) {
        return window[element];
    }
    return null;
}
$(document).ready(function () {
    $(document).keydown(function (event) {//����Tab��  �����κζ���
        if (event.keyCode == 9) return false;
    });
    $("#mDefaultServerAddr").attr("value", mDefaultServerAddr);
    $("#mDefaultServerPort").attr("value", mDefaultServerPort);
    // ��������ť�¼�
    $("#mSearchTarget").keyup(function (event) {
        Getdmo("UserListContent").innerHTML = "";
        var lower = $(this).val();
        BtnSearch(lower.toLocaleLowerCase());
    });
});
//����Textarea�س������й���   ʹ�÷�����Ϣ��ݼ�ʱ�Ų����Զ�����
function NewLine(event) {
    event = event || window.event;
    if (event.keyCode == 13) {
        event.returnValue = false; return false;
    }
}
// ҳ��������ִ�к���
function LogicInit() {
    setTimeout(function () {
        //����Ƿ�װ�˲��	
        var NEED_ANYCHAT_APILEVEL = "0"; 					// ����ҵ�����Ҫ��AnyChat API Level
        var errorcode = BRAC_InitSDK(NEED_ANYCHAT_APILEVEL); 	//��ʼ�����
        if (errorcode == GV_ERR_SUCCESS)
			$("#BodyDIV").show();
        else
			$("#prompt_div").show(); // û�а�װ��������ǲ���汾̫�ɣ���ʾ������ؽ���
    }, 500);
    var VHeight = document.body.offsetHeight; //  (�������ߵĿ�)
    var VWidth = document.body.offsetWidth; //  (�������ߵĿ�)
    
    document.body.style.backgroundColor = "#FFFFFF";

    getLoginInfo();    
}
//��ʼ�����н���λ�� �� �������ֵ
function initialize() {
    var mBrowserWidth = document.body.offsetWidth; // ��ҳ�ɼ������
    var mBrowserHeight = document.documentElement.clientHeight; //  ��ҳ�ɼ������
    CenterPosition(mBrowserWidth, mBrowserHeight, "Initiative_Call_Div", 300, 170); //�������в㴹ֱˮƽ����
    CenterPosition(mBrowserWidth, mBrowserHeight, "BeCalls_Div", 300, 170); //�����в㴹ֱˮƽ����
    CenterPosition(mBrowserWidth, mBrowserHeight, "InquityEnd", 300, 170); //�����Ựѯ�ʲ㴹ֱˮƽ����
    CenterPosition(mBrowserWidth, mBrowserHeight, "HallDiv", 770, 650); //�����Ựѯ�ʲ㴹ֱˮƽ����
    if (mBrowserHeight < 650) $("#HallDiv").css("top", "0px");
    Getdmo("fontfamily").options.length = 0; // �������������
    for (var i = 0; i < mFontsArray.length; i++) {
        var option = document.createElement("option");
        Getdmo("fontfamily").appendChild(option);
        option.value = i;
        option.text = mFontsArray[i];
    }
    Getdmo("fontsize").options.length = 0; // ��������С������
    for (var sizes = 8; sizes < 23; sizes++) {
        var option = document.createElement("option");
        Getdmo("fontsize").appendChild(option);
        option.value = sizes - 8;
        option.text = sizes;
    }
}
//��¼��ť
function LoginToHall() {
    var the_date = new Date("December 31, 2020");
    var expiresDate = the_date.toGMTString(); //ת���� GMT ��ʽ��
    mSelfUserName = Getdmo('txtUserName').value;
    mSelfPassWord = Getdmo('txtPassWord').value;
    if (mSelfUserName != "") {
    	setLoginInfo();
        prompt("��������...<br /><img src='./images/others/LoadImg.gif' style='width:145px;height:30px' />");
        if (Getdmo("normal_login").checked){
        	if (Getdmo("AppGuid") && Getdmo("AppGuid").value.length)				// ����Ӧ��ID
            	BRAC_SetSDKOption(BRAC_SO_CLOUD_APPGUID, Getdmo("AppGuid").value.toString());

			var errorcode = BRAC_Connect(Getdmo("mDefaultServerAddr").value, parseInt(Getdmo("mDefaultServerPort").value)); //���ӷ�����

			errorcode = BRAC_Login(mSelfUserName, mSelfPassWord, 0);
        }
        
        if (Getdmo("sign_login").checked){
            if (Getdmo("AppGuid") && Getdmo("AppGuid").value.length){
            	getSign("http://demo.anychat.cn:8930/", -1, mSelfUserName, Getdmo("AppGuid").value);
            }
        }        
        document.cookie = mSelfUserName + "Getdmo" + mSelfPassWord + "Getdmo" + mDefaultServerAddr + "Getdmo; expires=" + expiresDate; //д��Cookie ���ù���ʱ��
    }
    else prompt("<div style='color:red;'>�˺Ų���Ϊ��.</div>");
}
// ��¼��ʾ��Ϣ
function prompt(message) {
    Getdmo("prompt_box_div").innerHTML = message;
    Getdmo("prompt_box_div").style.display = "block";
}
//���÷�����IP
function DisplaySettingDiv() {
    $("#mUser_Info_Div").hide();
    $("#mServer_Info_Div").show();
}
//�������� ��ť
function SaveServerInfo() {
    mDefaultServerAddr = $("#mDefaultServerAddr").val();
    mDefaultServerPort = $("#mDefaultServerPort").val();
    $("#mServer_Info_Div").hide();
    $("#mUser_Info_Div").show();
}
//�����û�˫��
function ImgDBclick(ID) {
    if (mSelfUserId == ID) ForSession("���ܺ����Լ�...");
    else {
        mTargetUserId = ID;
		BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REQUEST,mTargetUserId,0,0,0,"");  // ��ָ�����û����ͻỰ����
		var GetUserName = BRAC_GetUserName(mTargetUserId);
        GetUserName = GetUserName.fontcolor("Red");
        Getdmo("Initiative_Call_Div_Content").innerHTML = "���û�" + GetUserName + "��������<br /><img src='./images/others/LoadImg.gif' style='width: 145px;height:30px;' />";
        Getdmo("Shade_Div").style.display = "block";
        Getdmo("Initiative_Call_Div").style.display = "block";
    }
}
//ȡ����������
function CancelCall() {
    $("#Shade_Div").hide();
    $("#Initiative_Call_Div").hide();
	BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REPLY,mTargetUserId,ICS_RETCODE_SESSION_CANCEL,0,0,"");  
    ForSession("ȡ������...");
}
//ͬ��Ự
function OnAcceptRequestBtnClick() {
	BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REPLY,mTargetUserId,0,0,0,"");  
    $("#BeCalls_Div").hide();
}
//�ܾ��Ự
function OnRejectRequestBtnClick() {
	BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_REPLY,mTargetUserId,ICS_RETCODE_SESSION_REJECT,0,0,"");  
    $("#Shade_Div").hide();
    $("#BeCalls_Div").hide();
    ForSession("�ܾ��Է�����...");
}
//����������ť
function BtnSearch(Values){
    var re = /^[1-9]+[0-9]*]*$/;   //�ж��Ƿ�����
    var UserList = BRAC_GetUserFriends(); // ��ȡ���������û�
	var SearchResult = false;
	if (!re.test(Values)) {//���Ǵ�����  ������û���
	    for (var i = 0; i < UserList.length; i++) {
	        var UserName = BRAC_GetUserName(UserList[i]);
	        UserName = UserName.toLocaleLowerCase();
	        if (UserName.indexOf(Values) > -1) {
	            DisplayResult(i);
	            SearchResult = true;
	        }
	    }
	}
	else {//����ID
	    for (var i = 0; i < UserList.length; i++) {
	        if (UserList[i] == (Values)) {
	            DisplayResult(i);
	            SearchResult = true;
	            //break;
	        }
	    }
	}
	if (!SearchResult) {//���Ҳ���
	    DisplayResult("NotFound");
	}
}
//��ʾ�������
function DisplayResult(No) {
    Getdmo("Group0").style.background = "url(./images/hall/05.png)"; // �����û��鲻��ѡ��
    Getdmo("Group0").style.width = "127px"; // �����û�����
    for (var j = 0; j < mGroupList.length; j++) { // ���������鲻��ѡ�� 
        Getdmo("Group" + mGroupList[j]).style.background = "url(./images/hall/05.png)";
        Getdmo("Group" + mGroupList[j]).style.width = "127px";
    }
    if (No != "NotFound") { // ��������
        var UserList = BRAC_GetUserFriends(); // ��ȡ���������û�
        DisplayOnLineUser(UserList[No]); // ��ʾ���������������û�
    }
}
// DIV�㴹ֱ���к�ˮƽ����  ���������������  �������������߶�  DIV��ID DIV��ĸ߶� DIV��Ŀ��
function CenterPosition(VWidth, VHeight, DivID, DivWidth, DivHeight) {
    $("#" + DivID).css("left", (VWidth - DivWidth) / 2 + "px"); // ����X����
    $("#" + DivID).css("top", (VHeight - DivHeight) / 2 + "px"); // ����Y����
}
// ��ʼ��Ƶ�Ự��һ���� ������Ƶ
function CreateWideScreen() {
    //��ҳ�ɼ������
    var VWidth = document.body.offsetWidth; // (�������ߵĿ�) 
    //��ҳ�ɼ�����ߣ�
    var VHeight = document.body.offsetHeight; //  (�������ߵĸ�)
    CenterPosition(VWidth, 730, "VideoShowDiv", 1020, 450);
    VideoContainer(); // ��̬���ɿ�����Ƶ����
    ChangeVideoShow("videoshow2", "videoshow1", "Video_Show_MySelf", "Video_Show_Target");
}
//�������ͺ�����������
function StartScroll(BackGround, Scroll, Content, Line) {
    var scrollheight = document.getElementById(Content).scrollHeight; // �б�ʵ�ʸ߶�
    var offsetheight = document.getElementById(Content).offsetHeight; // �б�ɼ��߶�
    var back = document.getElementById(BackGround); // ������������
    var line = document.getElementById(Line);
    var ScrollSlid = document.getElementById(Scroll); // �����
    var backgroundheight = back.offsetHeight; // ������������߶�
    if (scrollheight > offsetheight) { // �ж��Ƿ���Ҫ��ʾ������
        ScrollSlid.style.display = "block";
        line.style.display = "block";
        ScrollSlid.onmousedown = function () {
            ScrollSlid.onmousemove = function () {
               // ScrollSlid.setCapture();
                if (event.clientY - 150 < backgroundheight && event.clientY > 170) {
                    ScrollSlid.style.marginTop = event.clientY - 170 + "px";
                    document.getElementById(Content).scrollTop = (event.clientY - 170) * ((scrollheight - offsetheight) / 460);
                }
            }
        }
        ScrollSlid.onmouseup = function () {
            ScrollSlid.onmousemove = "";
            ScrollSlid.releaseCapture();
        }
    }
    else {
        ScrollSlid.style.display = "none";
        line.style.display = "none";
    }
}

// ��ʾ����
function GoToHall() {
    $("#BodyDIV").hide(); // ���ص�¼����
    $("#HallDiv").show(); // ��ʾ��������
    $("#LoginHead").hide(); // ���ض���������
}

// ���ص�¼����
function GoToLogin() {
	ForSession("���������ѶϿ�..."); // ��ʾ��
	// �������н���
	$("HallDiv").hide(); // ���ش���
    $("DialogueDiv").hide(); // ���ػỰ��
    $("Shade_Div").hide(); // �������ֲ�
    $("DivColor").hide(); // ������ɫ��
    $("GetExpression").hide(); // ���ر����
    $("advanceset_div").hide(); // ���ظ߼����ò�
	
	Getdmo("GroupContent").innerHTML = "";
	Getdmo("mSelfInfo").innerHTML = "";
	Getdmo("UserListContent").innerHTML = "";
	
	// ��ʾ��¼����
	$("BodyDIV").show();
	$("#LoginHead").show();
	LogicInit();
}

// �����Ự���ش���
function BackToHall() {
	BRAC_LeaveRoom(-1);
    Getdmo("HallDiv").style.display = "block"; // ��ʾ����
    Getdmo("DialogueDiv").style.display = "none"; // ���ػỰ��
    Getdmo("Shade_Div").style.display = "none"; // �������ֲ�
    Getdmo("DivColor").style.display = "none"; // ������ɫ��
    Getdmo("GetExpression").style.display = "none"; // ���ر����
    Getdmo("advanceset_div").style.display = "none"; // ���ظ߼����ò�
    $("#SendMsg").html(""); // ��շ�����Ϣ������
    $("#Enquire_Div").hide(); // ���ؽ����Ựѯ�ʿ�
    $("#VideoShowDiv").hide(); // ���ؿ�Ƶ��
    ForSession("�Ự����..."); // ��ʾ��
    clearInterval(mRefreshVolumeTimer); // �ر�����������ʾ
}
//��˸��ʾ
function MessageFlicker() {
    var GetImg = true; // ͼƬ��ʾ
    if (!mWhetherMainForm) { // ��ʾ����Ϊ��Ƶ��
        mFlickerNews = setInterval(function () {
            if (GetImg) {
                $("#SwappingDiv").css("background-image", "url('../images/dialog/flicker.png')");
                GetImg = false;
            }
            else {
                $("#SwappingDiv").css("background-image", "url('../images/dialog/btntrue_move.png')");
                GetImg = true;
            }
        }, 500);
    }
}
// �ļ�������Ϣ�ظ���ʾ
function ReplyMsgStyle(id, Msg, type) {
    Getdmo(id).innerHTML = Msg;
    if (type) // ������Ϣ�ĸ߶�
        $("#" + id).css({ "color": "#999999", "height": "35px", "font-size": "12px" });
    else // ������Ϣʱ�ĸ߶�
        $("#" + id).css({ "color": "#999999", "height": "65px", "font-size": "12px" });
}

//���õ�¼��Ϣ�������û�����������IP���������˿ڡ�Ӧ��ID
function setLoginInfo() {
	var loginType = 0;
    setCookie('username',Getdmo("txtUserName").value,30);
    setCookie('ServerAddr',Getdmo("mDefaultServerAddr").value,30);
    setCookie('ServerPort',Getdmo("mDefaultServerPort").value,30);   
    setCookie('AppGuid',Getdmo("AppGuid").value,30);
    if (Getdmo("normal_login").checked){
    	loginType = Getdmo("normal_login").value;
    }
    if (Getdmo("sign_login").checked){
    	loginType = Getdmo("sign_login").value;
    }
    setCookie('loginType',loginType,30);
}

//��ȡ��¼��Ϣ
function getLoginInfo() {
    Getdmo("txtUserName").value = getCookie("username");
    var serverIP = getCookie("ServerAddr");
	Getdmo("mDefaultServerAddr").value = (serverIP != "") ? serverIP : mDefaultServerAddr;        
    var serverPort = getCookie("ServerPort");    
	Getdmo("mDefaultServerPort").value = (serverPort != "") ? serverPort : mDefaultServerPort;        
    Getdmo("AppGuid").value = getCookie("AppGuid");
    var loginType = getCookie("loginType");
    if (loginType == Getdmo("normal_login").value){
    	Getdmo("normal_login").checked = true;
    }
	if (loginType == Getdmo("sign_login").value){
    	Getdmo("sign_login").checked = true;
    }    
}

//��ȡcookie���cookieֵ
function getCookie(c_name) {
    if (document.cookie.length > 0) {
        c_start = document.cookie.indexOf(c_name + "=");
        if (c_start != -1) {
            c_start = c_start + c_name.length + 1;
            c_end = document.cookie.indexOf(";", c_start);
            if (c_end == -1) c_end = document.cookie.length;
            return document.cookie.substring(c_start, c_end);
        }
    }
    return "";
}

//����cookie
function setCookie(c_name, value, expiredays){
����var exdate=new Date();
����exdate.setDate(exdate.getDate() + expiredays);
����document.cookie=c_name+ "=" + value + ((expiredays==null) ? "" : ";expires="+exdate.toGMTString());
}

//ȥ���ַ������߿ո�
String.prototype.trim = function () {
    return this.replace(/(^\s*)|(\s*$)/g, "");
}

//��ȡǩ��
function getSign(signUrl, userId, strUser, appId) {
	var retVal = null;
	var parameter = "userid=" + userId + "&struserid=" + strUser + "&appid=" + appId ;
    var requestURL = signUrl;
    xmlHttpReq = createXMLHttpRequest();
        xmlHttpReq.open("POST",requestURL,true);
        xmlHttpReq.setRequestHeader("Content-Type","application/x-www-form-urlencoded;");
        xmlHttpReq.send(encodeURI(encodeURI(parameter)));
    
    xmlHttpReq.onreadystatechange = function(){
        if(xmlHttpReq.readyState == 4){
            switch(xmlHttpReq.status){
                case 200:
                    responseText = xmlHttpReq.responseText;
                    retVal = JSON.parse(responseText);
                    
                    if (retVal.errorcode == 0){
            			var signStr = retVal.sigStr;
            			var signTimestamp = retVal.timestamp;

						//���ӷ�����
			            var errorcode = BRAC_Connect(Getdmo("mDefaultServerAddr").value, parseInt(Getdmo("mDefaultServerPort").value)); //���ӷ�����
			            //AddLog("BRAC_Connect(" + Getdmo("mDefaultServerPort").value + "," + Getdmo("mDefaultServerPort").value + ")=" + errorcode, LOG_TYPE_API);

						//ǩ����¼
	                   	errorcode = BRAC_LoginEx(strUser, -1, strUser, appId, signTimestamp, signStr, "");
	            	    //AddLog("BRAC_LoginEx(" + strUser + ")=" + errorcode, LOG_TYPE_API);            			
            		}

                    break;
                case 400:
                    alert("���������\nError Code:400!");
                    break;
                case 403:
                    alert("�ܾ�����\nError Code:403!");
                    break;
                case 404:
                    alert("�����ַ�����ڣ�\nError Code:404!");
                    break;
                case 500:
                    alert("�ڲ�����\nError Code:500!");
                    break;
                case 503:
                    alert("���񲻿��ã�\nError Code:503!");
                    break;
                default:
                    alert("���󷵻��쳣��\nError Code:"+xmlHttpReq.status);
                    break;
            }
        }
    }
    
}

// ����XMLHttpRequest����
function createXMLHttpRequest() {
    if (window.XMLHttpRequest) {// IE 7.0�����ϰ汾�ͷ�IE�������
        xmlHttpReq = new XMLHttpRequest();
    } else {// IE 6.0�����°汾
        try {
            xmlHttpReq = new ActiveXObject("MSXML2.XMLHTTP");
        }catch (e) {
            try {
                xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
            }catch (e) {}
        }
    }
    if (!xmlHttpReq) {
        alert("��ǰ�������֧��!");
        return null;
    }
    return xmlHttpReq;
}