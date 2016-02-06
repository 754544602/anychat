$(document).ready(function () {
    // ��ⷢ�Ϳ�������Ϣ�߶��Ƿ���Ҫ��ʾ������
    $("#SendMsg").keyup(function (event) {
        var ImportText = Getdmo("SendMsg");
        var VisiblHeight = ImportText.offsetHeight; // ���Ϳ�ɼ��߶�
        var ActualHeight = ImportText.scrollHeight; // ���Ϳ�������߶�
        if (ActualHeight > VisiblHeight) { // �������߶ȸ��ڿɼ��߶�
            Getdmo("SendMsg").style.overflowY = "scroll"; // ��ʾ������
            Getdmo("SendMsg").scrollTop = Getdmo("SendMsg").scrollHeight; // �������Զ�����
        }
        else Getdmo("SendMsg").style.overflowY = "hidden"; // �ɼ��߶�С�ڹ������߶� ����������
        var message = $("#SendMsg").html();
        Expression(message, 0); // ��鷢�Ϳ��Ƿ��б���  �о���ʾ����ͼƬ
    });
    // �س���������Ϣ
    $("#SendMsg").keypress(function (event) {
        if (event.keyCode == 13) {
            if ($("#SendMsg").html() == "") return false; // ��������Ϊ��  �����κζ���
            SendMessage(); // ������Ϣ
            $("#SendMsg").html(""); // ��շ��Ϳ�
        }
    });
    // ��ʾ�߼����ý���
    $("#AdvanceBtn").click(function () {
        $("#advanceset_div").toggle(); // �߼����ý�������
    });
    // ������Ƶ Զ����Ƶ �л�
    $("#RightSide_MiddleButton img:last").click(function () {
        if ($("#RightSide_MiddleButton a").attr("longrange") == "true") { // ����Ƶ��ΪԶ����Ƶ
            ChangeVideoShow("Video_Show_Target", "Video_Show_MySelf", "Video_Show_Target", "Video_Show_MySelf"); // ����Ƶ������Ϊ������Ƶ
            $("#DialogueDiv_RightSide a:first").html("������Ƶ"); // ��Ӧ������ʾ
            $("#DialogueDiv_RightSide a:last").html("Զ����Ƶ"); // ��Ӧ������ʾ
            $("#RightSide_MiddleButton a").attr("longrange", "false"); // ��ʶ����Ƶ��Ϊ������Ƶ
        }
        else {
            ChangeVideoShow("Video_Show_MySelf", "Video_Show_Target", "Video_Show_Target", "Video_Show_MySelf"); // ����Ƶ������ΪԶ����Ƶ
            $("#DialogueDiv_RightSide a:first").html("Զ����Ƶ"); // ��Ӧ������ʾ
            $("#DialogueDiv_RightSide a:last").html("������Ƶ"); // ��Ӧ������ʾ
            $("#RightSide_MiddleButton a").attr("longrange", "true"); // ��ʶ����Ƶ��ΪԶ����Ƶ
        }
    });
    // ����Ƶ�� ���ذ�ť
    $("#RightSide_MiddleButton img:first").click(function () {
        if ($("#RightSide_MiddleButton a:first").attr("longrange") == "true") // ����Ƶ��ΪԶ����Ƶ
            OperateVideo("#RightSide_MiddleButton img:first", mTargetUserId); // ��ť����Ŀ��ID��Ƶ
        else // ����Ƶ��Ϊ������Ƶ
            OperateVideo("#RightSide_MiddleButton img:first", mSelfUserId); // ��ť��������ID��Ƶ
    });
    // С��Ƶ�� ���ذ�ť
    $("#DialogueDiv_RightSide img:last").click(function () {
        if ($("#RightSide_MiddleButton a:first").attr("longrange") == "true") // ����Ƶ��ΪԶ����Ƶ
            OperateVideo("#DialogueDiv_RightSide img:last", mSelfUserId); // С��ť��������ID��Ƶ
        else // ����Ƶ��ΪԶ����Ƶ
            OperateVideo("#DialogueDiv_RightSide img:last", mTargetUserId); // С��ť����Ŀ��ID��Ƶ
    });
    // ���ز���
    function OperateVideo(element, target_id) {
        if ($(element).attr("videostate") == "true") { // ����Ƶ��Ϊ����״̬
            BRAC_UserCameraControl(target_id, 0); // �ر���Ƶ
            $(element).attr("videostate", "false"); // ����״̬����Ϊ�ر�
        }
        else { // ����Ƶ��Ϊ�ر�״̬
            BRAC_UserCameraControl(target_id, 1); // ������Ƶ
            $(element).attr("videostate", "true"); // ����״̬����Ϊ����
        }
    }
});
//����������Ϣ
function SendMessage() {
    $(".Phiz").each(function () { $(this).replaceWith("[" + $(this).attr("alt") + "]"); }); // ��������ʾ�ڷ��Ϳ���
    var txt = $("#SendMsg").html();
    var len = txt.length;
    if (len > 0) { // �жϷ�����Ϣ�Ƿ�Ϊ��
        MessageFormat();
        var mFontFamily = Getdmo("SendMsg").style.fontFamily;
        var mFontColor = Getdmo("SendMsg").style.color;
        var mFontSize = Getdmo("SendMsg").style.fontSize;
        txt = "<div style=' color:" + mFontColor + "; font-size:" + mFontSize + "; font-family:" + mFontFamily + ";'>" + txt + "</div>";
        if (compute(txt) < 1000) { // �ж���Ϣ�����Ƿ�̫��
            var msg_div = document.createElement("div");
            msg_div.style.width = "360px";
            msg_div.style.textAlign = "left";
            msg_div.style.color = mFontColor;
            msg_div.style.fontSize = mFontSize;
            msg_div.style.fontFamily = mFontFamily;
            msg_div.style.margin = "0px 0px 0px 10px";
            msg_div.innerHTML = Expression(txt, 1);
            Getdmo("ReceiveMsg").appendChild(msg_div); // ������Ϣ����ʾ����������Ϣ����
            BRAC_SendTextMessage(mTargetUserId, 1, txt); // ������Ϣ
            $("#SendMsg").html(""); // ��շ��Ϳ�
            ReceiveMsgBoxScroll(); // �жϽ��տ��Ƿ���Ҫ��ʾ������
            $("#SendMsg").focus(); // ���Ϳ��ý���
            $("#SendMsg").css("overflowY", "hidden"); // ���Ϳ����������
        }
        else $("#ReceiveMsg").append("<div class='StandardSize' style='color:#999999'>����ʧ��,������Ϣ����...</div>");
    }
    else Getdmo("SendMsg").focus();
}
//������Ϣ����
function compute(str) {
    var i = 0;
    for (var j = 0; j < str.length; j++) {
        if (str.charCodeAt(j) < 0 || str.charCodeAt(j) > 255) i += 2;
        else i++;
    }
    return i;
}
// �����Ựѯ��
function FinishDialogue() {
    Getdmo("Enquire_Div").innerHTML = ""; // ��ս����Ự�� ��������
    var ask_div = document.createElement("div");
    ask_div.className = "AskInfo";
    ask_div.innerHTML = "�Ƿ�������λỰ";
    Getdmo("Enquire_Div").appendChild(ask_div);
    var finish_btn = document.createElement("div"); // �����Ự��ť
    finish_btn.className = "AskButtonImg";
    finish_btn.style.margin = "10px 10px 0px 20px";
    finish_btn.innerHTML = "����";
    finish_btn.onclick = function () {
        mWhetherMainForm = false;  // ���ûỰ��״̬����Ϊfalse
        BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_FINISH,mTargetUserId,0,0,0,""); 
        BackToHall(); // �˻ش���
    }
    Getdmo("Enquire_Div").appendChild(finish_btn);
    var cancel_btn = document.createElement("div");
    cancel_btn.className = "AskButtonImg";
    cancel_btn.innerHTML = "ȡ��"; // ȡ�������Ự��ť
    cancel_btn.style.margin = "10px 10px 0px 0px";
    cancel_btn.onclick = function () {
        $("#Enquire_Div").hide(); // ����ѯ�ʿ�
    }
    Getdmo("Enquire_Div").appendChild(cancel_btn);
    $("#Enquire_Div").show(); // ��ʾѯ�ʿ�
}
//�ı��Ի�����������
function SetFontFamily() {
    var SelectFamily = Getdmo("fontfamily");
    var FontStyle = SelectFamily.options[SelectFamily.selectedIndex].text;
    $("#SendMsg").css("font-family", FontStyle);
}
//�ı��Ի��������С����
function SetFontSize() {
    var SelectSize = Getdmo("fontsize");
    var FontSize = SelectSize.options[SelectSize.selectedIndex].text + "px";
    $("#SendMsg").css("fontSize", FontSize);
}
// ��ɫ���ͱ���������¼�
function CorrespondingDiv(ShowID, HideID) {
    $("#" + HideID).hide();
    $("#" + ShowID).toggle();
}
//���ļ����ͽ���
function GetFilePath() {
    var guid = GUID();
    var GetTheResult = ICS_FileTransRequest(mTargetUserId, guid, "");
    if (GetTheResult == 0) { //���ʹ�������
        MessageFormat();
        var main_div = document.createElement("div");
        main_div.id = "SendFileMsg" + guid;
        main_div.className = "SendFileMsg";
        main_div.style.height = "54px";
        Getdmo("ReceiveMsg").appendChild(main_div);

        var prompt_div = document.createElement("div");
        prompt_div.style.fontSize = "12px";
        prompt_div.innerHTML = "&nbsp&nbsp�����ļ�: " + ViewFileDetail(guid) + " <br />&nbsp&nbsp�ȴ��Է�����......";
        main_div.appendChild(prompt_div);

        var cancel_div = document.createElement("div");
        cancel_div.id = "Discontinue" + guid;
        cancel_div.className = "CancelTransmit";
        cancel_div.setAttribute("Belong", guid);

        var cancel_btn = document.createElement("div");
        cancel_btn.style.fontSize = "12px";
        cancel_btn.style.color = "blue";
        cancel_btn.style.cursor = "pointer";
        cancel_btn.style.marginLeft = "10px";
        cancel_btn.innerHTML = "ȡ��";
        cancel_btn.style.height = "20px";
        cancel_btn.onclick = function () {
            ICS_FileTransReply(guid, ICS_RETCODE_FILETRANS_CANCEL);
            ReplyMsgStyle("SendFileMsg" + guid, "&nbsp&nbsp��ȡ�����ļ�: " + ViewFileDetail(guid) + "�ķ���<br />&nbsp&nbsp�ļ�����ʧ��...", true);
            ReceiveMsgBoxScroll();
        }
        cancel_div.appendChild(cancel_btn);
        main_div.appendChild(cancel_div);
    }
    ReceiveMsgBoxScroll();
} 
//�ڹ�괦�������
function InsertExpression(html) {
    var sel, range;
    document.getElementById("SendMsg").focus();
    if (window.getSelection) {
        sel = window.getSelection();
        if (sel.getRangeAt && sel.rangeCount) {
            range = sel.getRangeAt(0);
            range.deleteContents();
            var el = document.createElement("div");
            el.innerHTML = html;
            var frag = document.createDocumentFragment(), node, lastNode;
            while ((node = el.firstChild)) {
                lastNode = frag.appendChild(node);
            }
            range.insertNode(frag);
            if (lastNode) {
                range = range.cloneRange();
                range.setStartAfter(lastNode);
                range.collapse(true);
                sel.removeAllRanges();
                sel.addRange(range);
            }
        }
    } else if (document.selection && document.selection.type != "Control") {
        document.selection.createRange().pasteHTML(html);
    }
}
//�ж���Ϣ���տ��Ƿ���Ҫ��ʾ������
function ReceiveMsgBoxScroll() {
    var ReceiveMsg = Getdmo("ReceiveMsg");
    var VisiblHeight = ReceiveMsg.offsetHeight;
    var ActualHeight = ReceiveMsg.scrollHeight;
    if (ActualHeight > VisiblHeight) {
        Getdmo("ReceiveMsg").style.overflowY = "scroll";
        Getdmo("ReceiveMsg").scrollTop = Getdmo("ReceiveMsg").scrollHeight;
    }
    else
        Getdmo("ReceiveMsg").style.overflowY = "hidden";
}
//���ü�����Ϣ��ʾ��ʽ
function MessageFormat() {
    var TimeFormat = new Date();
    var ShowTime = TimeFormat.toLocaleTimeString();
    var txt_div = document.createElement("div");
    txt_div.id = "SendMsgStyle";
    txt_div.innerHTML = mSelfUserName + "  " + ShowTime;
    Getdmo("ReceiveMsg").appendChild(txt_div);
}
// ����һ��GUID
function G() {
    return (((1 + Math.random()) * 0x10000) | 0).toString(16).substring(1)
}
function GUID() {
    var guid = (G() + G() + "-" + G() + "-" + G() + "-" + G() + "-" + G() + G() + G()).toUpperCase();
    return guid;
}
//��ʽ: XXX.txt(122MB)
function ViewFileDetail(Guid) {
    return ICS_FileTransGetFileName(Guid) + "(" + UnitConversion(ICS_FileTransGetFileSize(Guid), "MB", "KB", "B", 1) + ")";
}
//��λת��
function UnitConversion(Values, M, KB, B, Type) {
    var unit;
    if (Type == 0)
        Values = Values / 8;
    if (Values > 1024) {
        Values = (Values / 1024);
        if (Values > 1024) {
            Values = (Values / 1024);
            unit = M;
        }
        else {
            Values = Values;
            unit = KB;
        }
    }
    else {
        Values = Values;
        unit = B;
    }
    return Math.floor(Values * 100) / 100 + unit;
}
////���� ������
//var mCoordinateX;
//function OnLoad(GetSlider) {
//    var slider = document.getElementById(GetSlider);
//    slider.onmousedown = function () {
//        slider.onmousemove = function () {
//            slider.setCapture();
//            if (event.clientX > mCoordinateX && event.clientX < mCoordinateX + 140) {
//                slider.style.marginLeft = event.clientX - mCoordinateX + "px";
//            }
//        }
//    }
//    slider.onmouseup = function () {
//        slider.onmousemove = "";
//        slider.releaseCapture();
//    }
//}
function ClickEvent(Div_ID) {
    var element = Getdmo(Div_ID);
    var CoordinateX = event.clientX;
    var CoordinateY = event.clientY;
    var MinelementX = element.offsetLeft;
    var MinelementY = element.offsetTop;
    var MaxelementX = element.offsetLeft + element.offsetWidth;
    var MaxelementY = element.offsetTop + element.offsetHeight;
    $(document).click(function () {
        if (CoordinateX < MinelementX && CoordinateX > MaxelementX && MinelementY < MinelementY && MinelementY > MaxelementY)
            Div_ID.hide();
    });
}
//��̬���ɿ�����Ƶ����
function VideoContainer() {
    Getdmo("VideoShowDiv").innerHTML = "";
    // �����Ƶ��
    var upper_video1 = document.createElement("div");
    upper_video1.id = "videoshow1";
    upper_video1.className = "videoshow";
    Getdmo("VideoShowDiv").appendChild(upper_video1);
    // �ұ���Ƶ��
    var upper_video2 = document.createElement("div");
    upper_video2.id = "videoshow2";
    upper_video2.className = "videoshow";
    upper_video2.style.marginLeft = "6px";
    Getdmo("VideoShowDiv").appendChild(upper_video2);
    // ��ʾ��������
    var upper_othername = document.createElement("div");
    upper_othername.className = "ShowName";
    upper_othername.innerHTML = BRAC_GetUserName(mTargetUserId);
    Getdmo("VideoShowDiv").appendChild(upper_othername);
    // ��ʾ�Է�����
    var upper_myname = document.createElement("div");
    upper_myname.className = "ShowName";
    upper_myname.innerHTML = BRAC_GetUserName(mSelfUserId);
    Getdmo("VideoShowDiv").appendChild(upper_myname);
    // �Ҷ� ��ť
    var under_finish = document.createElement("div");
    under_finish.id = "finishprivate";
    under_finish.onmouseout = function () {
        $("#finishprivate").css("background", "url('./images/dialog/btnfalse_move.png')");
    }
    under_finish.onmouseover = function () {
        $("#finishprivate").css("background", "url('./images/dialog/btnfalse_over.png')");
    }
    under_finish.onclick = function () {
		BRAC_VideoCallControl(BRAC_VIDEOCALL_EVENT_FINISH,mTargetUserId,0,0,0,""); 	// �Ҷ�
    }
    Getdmo("VideoShowDiv").appendChild(under_finish);
    // �л����� ��ť
    var under_change = document.createElement("div");
    under_change.id = "SwappingDiv";
    under_change.className = "Buttons";
    under_change.onmouseout = function () {
        $("#SwappingDiv").css("background", "url('./images/dialog/btntrue_move.png')");
    }
    under_change.onmouseover = function () {
        $("#SwappingDiv").css("background", "url('./images/dialog/btntrue_over.png')");
    }
    under_change.onclick = function () {
        SwappingVideo(true);
    }
    under_change.innerHTML = "����ģʽ";
    Getdmo("VideoShowDiv").appendChild(under_change);
}
// ��Ƶ�Ựģʽ�л�
function SwappingVideo(type) {
    mWhetherMainForm = type; // ��һ�����汻��ʾ
    if (type) { // �л�������ģʽ
        clearInterval(mFlickerNews);  // ȡ������Ϣ��˸��ʾ
        $("#VideoShowDiv").hide(); //����������Ƶ��
        $("#DialogueDiv").show();
        Getdmo("SendMsg").focus(); //���Ϳ��ȡ����
        ChangeVideoShow("Video_Show_MySelf", "Video_Show_Target", "videoshow2", "videoshow1");
        mRefreshVolumeTimer = setInterval(CurrentVolume, 200); // ��ȡ��ʾ��������
    }
    else { // �л�����Ƶģʽ
        $("#VideoShowDiv").show(); //��ʾ������Ƶ��
        $("#DialogueDiv").hide();
        ChangeVideoShow("videoshow2", "videoshow1", "Video_Show_MySelf", "Video_Show_Target");
        clearInterval(mRefreshVolumeTimer); // �ر�����������ʾ
    }
}
// ˫��������
function CurrentVolume() {
    Getdmo("Mine_Volume").style.width = Getdmo("Video_Show_MySelf").offsetWidth / 100 * BRAC_QueryUserStateInt(mSelfUserId, BRAC_USERSTATE_SPEAKVOLUME) + "px";
    Getdmo("Target_Volume").style.width = Getdmo("Video_Show_Target").offsetWidth / 100 * BRAC_QueryUserStateInt(mTargetUserId, BRAC_USERSTATE_SPEAKVOLUME) + "px";
}
// ������ʾ��Ƶλ��
function ChangeVideoShow(firVideo, secVideo, ClearfirVideo, ClearsecVideo) {
    Getdmo(ClearfirVideo).innerHTML = "";
    Getdmo(ClearsecVideo).innerHTML = "";
    BRAC_SetVideoPos(mSelfUserId, Getdmo(firVideo), "ANYCHAT_VIDEO_LOCAL");
    BRAC_SetVideoPos(mTargetUserId, Getdmo(secVideo), "ANYCHAT_VIDEO_REMOTE");
}
// �л�������ģʽ
function ToWideScreen() {
    SwappingVideo(false);
}