
//��Ƶ��������������
var code_rate_txt = ["����ģʽ", "40Kbps", "60Kbps", "100Kbps", "150Kbps", "200Kbps", "300Kbps", "400Kbps", "500Kbps", "600Kbps", "800Kbps", "1000Kbps", "1200Kbps", "1500Kbps"]; // ����������
var quality_txt = ["��ͨ��Ƶ����", "�е���Ƶ����", "�Ϻ���Ƶ����"]; // ����������
var distinguishability_txt = ["176x144", "320x240", "352x288", "640x480", "720x480", "720x576", "800x600", "960x720", "1024x576", "1280x720", "1280x1024", "1920x1080"]; // �ֱ���������
var frame_rate_txt = ["5 FPS", "8 FPS", "12 FPS", "15 FPS", "20 FPS", "25 FPS", "30 FPS"]; // ֡��������
var preinstall_txt = ["1", "2", "3", "4", "5"]; // Ԥ��������
var speakmode_txt = ["����ģʽ(Ĭ��)", "�Ÿ�ģʽ", "����OKģʽ", "��·����ģʽ"]; // ��Ƶģʽ������

var code_rate_value = [0, 40000, 60000, 100000, 150000, 200000, 300000, 400000, 500000, 600000, 800000, 1000000, 1200000, 1500000]; // ����ֵ
var quality_value= [2, 3, 4]; // ��������ֵ
var frame_rate_value= [5, 8, 12, 15 , 20 , 25 , 30 ]; // ֡��ֵ
var preinstall_value= [1, 2, 3, 4, 5]; // Ԥ�����ֵ
var speakmode_value= [0, 1, 2, 3]; // ��Ƶģʽֵ

//���������ֵ
function filltheselect(id, txtArray,valueArray) {
    GetID(id).options.length = 0;
    for (var j = 0; j < txtArray.length; j++) {
        var option = document.createElement("option");
        GetID(id).appendChild(option);
        option.value = valueArray[j];
        option.text = txtArray[j];
    }
}
// ��ʼ���߼����ý������пؼ� ���и�ֵ
function InitAdvanced() {
    filltheselect("code_rate", code_rate_txt,code_rate_value); // �������������
    filltheselect("quality", quality_txt,quality_value); // �������������
    filltheselect("distinguishability", distinguishability_txt,distinguishability_txt); // ���ֱ���������
    filltheselect("frame_rate", frame_rate_txt,frame_rate_value); // ���֡��������
    filltheselect("preinstall", preinstall_txt,preinstall_value); // ���Ԥ��������
    filltheselect("Speak_Mode", speakmode_txt,speakmode_value); // ����ģʽ������
    filltheselect("DeviceType_VideoCapture", BRAC_EnumDevices(BRAC_DEVICE_VIDEOCAPTURE), BRAC_EnumDevices(BRAC_DEVICE_VIDEOCAPTURE)); // ��Ƶ�ɼ��豸������ֵ
    filltheselect("DeviceType_AudioCapture", BRAC_EnumDevices(BRAC_DEVICE_AUDIOCAPTURE),BRAC_EnumDevices(BRAC_DEVICE_AUDIOCAPTURE)); // ��Ƶ�ɼ��豸������ֵ
    filltheselect("DeviceType_AudioPlayBack", BRAC_EnumDevices(BRAC_DEVICE_AUDIOPLAYBACK), BRAC_EnumDevices(BRAC_DEVICE_AUDIOPLAYBACK)); // ��Ƶ�����豸������ֵ
    SetThePos();
    initControlSelected();
}
// ����Ƶ�豸 ��ť����Ч��
function SettingBtnMouseout(id) {
    if (GetID(id).getAttribute("clickstate") == "false") // û�б�����İ�ť�ı䱳��ɫ
        GetID(id).style.backgroundColor = "#9CAAC1";
}
// ����Ƶ�豸 ��ť����Ч��
function SettingBtnMouseover(id, dd) {
    // �������в�������
    GetID("Device_Interface").style.display = "none";
    GetID("Video_Parameter_Interface").style.display = "none";
    GetID("Sound_Parameter_Interface").style.display = "none";
    GetID("Other_Parameter_Interface").style.display = "none";
    // ����ĸ���ť  ���³�ʼ��
    var btn = GetID("advanceset_div_Div_Btn").getElementsByTagName("div");
    for (var i = 0; i < btn.length; i++) {
        btn[i].style.backgroundColor = "#9CAAC1"; // ���ð�ť��ɫ
        btn[i].setAttribute("clickstate", "false"); // ���ð�ť���״̬Ϊδ���
    }
    GetID(dd).setAttribute("clickstate", "true"); // ���ñ�����İ�ť״̬Ϊ�����
    GetID(dd).style.backgroundColor = "White"; // ���ð�ť����ɫ
    GetID(id).style.display = "block"; // ��ʾ��ť��Ӧ�Ĳ�������
}

// �������¼�
function GetTheValue(id) {
    var value = GetID(id).options[GetID(id).selectedIndex].value;
    switch (id) {
        case "DeviceType_VideoCapture": // ��Ƶ�ɼ��豸
            BRAC_SelectVideoCapture(BRAC_DEVICE_VIDEOCAPTURE, value);
            break;
        case "DeviceType_AudioCapture": // ��Ƶ�ɼ��豸
            BRAC_SelectVideoCapture(BRAC_DEVICE_AUDIOCAPTURE, value);
            break;
        case "DeviceType_AudioPlayBack": // ��Ƶ�����豸
            BRAC_SelectVideoCapture(BRAC_DEVICE_AUDIOPLAYBACK, value);
            break;
        case "quality": // ����
            BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_QUALITYCTRL, parseInt(value));
            break;
        case "code_rate": // ����
			BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_BITRATECTRL, parseInt(value));
            break;
        case "distinguishability": // �ֱ���
            var resolution = value.split('x');
            BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_WIDTHCTRL, parseInt(resolution[0]));
            BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_HEIGHTCTRL, parseInt(resolution[1]));
            //GetID("current_resolution").innerHTML = BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_WIDTHCTRL) + "x" + BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_HEIGHTCTRL) + ")";
			break;
        case "frame_rate": // ֡��
            BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_FPSCTRL, parseInt(value));
            break;
        case "preinstall": // Ԥ��
            BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_PRESETCTRL, parseInt(value));
            break;
        case "Speak_Mode": // ��Ƶ����ģʽ
            BRAC_SetSDKOption(BRAC_SO_AUDIO_CAPTUREMODE, parseInt(value));
            break;
    }
}
//  ��ѡ���¼�
function ChangeTheResult(id) {
    switch (id) {
        case "ServerSetting": // ���������ò�����ť
            var GetAControl = GetID("advanceset_div_Tab").getElementsByTagName("a");
            var SelectTag = GetID("Video_Parameter_Interface").getElementsByTagName("select"); // ȡ�� ��Ƶ�������� ���� ����select��ǩ
            if (GetID("ServerSetting").checked == true) { // ��ǩ����¼�
                for (var i = 0; i < SelectTag.length; i++) { // ѭ����ǩ
                    SelectTag[i].disabled = "disabled";
                }
                for (var j = 0; j < GetAControl.length; j++)
                    GetAControl[j].style.color = "#999999";
            }
            else {
                for (var i = 0; i < SelectTag.length; i++) 
                        SelectTag[i].disabled = "";
                for (var j = 0; j < GetAControl.length; j++)
                        GetAControl[j].style.color = "Black";
            }
            break;
        case "Checkbox_P2P":
            if (GetID(id).checked == true)
				BRAC_SetSDKOption(BRAC_SO_NETWORK_P2PPOLITIC, 1);
            else 
				BRAC_SetSDKOption(BRAC_SO_NETWORK_P2PPOLITIC, 0);
            break;
        case "audio_vadctrl": // �������
            if (GetID(id).checked == true)
				BRAC_SetSDKOption(BRAC_SO_NETWORK_P2PPOLITIC, 1);
            else 
				BRAC_SetSDKOption(BRAC_SO_NETWORK_P2PPOLITIC, 0);
            break;
        case "audio_echoctrl": // ��������
            if (GetID(id).checked == true)
				BRAC_SetSDKOption(BRAC_SO_AUDIO_ECHOCTRL, 1);
            else 
				BRAC_SetSDKOption(BRAC_SO_AUDIO_ECHOCTRL, 0);
            break;
        case "audio_nsctrl": // ��������
            if (GetID(id).checked == true)
			 	BRAC_SetSDKOption(BRAC_SO_AUDIO_NSCTRL, 1);
            else 
				BRAC_SetSDKOption(BRAC_SO_AUDIO_NSCTRL, 0);
            break;
        case "audio_agcctrl": // �Զ�����
            if (GetID(id).checked == true)
				 BRAC_SetSDKOption(BRAC_SO_AUDIO_AGCCTRL, 1);
            else
				 BRAC_SetSDKOption(BRAC_SO_AUDIO_AGCCTRL, 0);
            break;
    }
}

// ��ȡ��ǰ����ֵ
function initControlSelected() {
    GetIndex("DeviceType_VideoCapture", BRAC_EnumDevices(BRAC_DEVICE_VIDEOCAPTURE),BRAC_GetCurrentDevice(BRAC_DEVICE_VIDEOCAPTURE),"combobox"); // ��ǰʹ�õ���Ƶ�ɼ���
    GetIndex("DeviceType_AudioCapture", BRAC_EnumDevices(BRAC_DEVICE_AUDIOCAPTURE),BRAC_GetCurrentDevice(BRAC_DEVICE_AUDIOCAPTURE),"combobox"); // ��ǰʹ�õ���Ƶ�ɼ���
	GetIndex("DeviceType_AudioPlayBack", BRAC_EnumDevices(BRAC_DEVICE_AUDIOPLAYBACK),BRAC_GetCurrentDevice(BRAC_DEVICE_AUDIOPLAYBACK),"combobox"); // ��ǰʹ�õ���Ƶ������
	GetIndex("quality", quality_value,BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_QUALITYCTRL),"combobox"); // ��ǰʹ�õ���������
	GetIndex("code_rate", code_rate_value,BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_BITRATECTRL),"combobox"); // ��ǰʹ�õ����ʲ���
    GetIndex("distinguishability",distinguishability_txt, BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_WIDTHCTRL) + "x" + 			BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_HEIGHTCTRL), "combobox"); // ��ǰʹ�õĵķֱ���
    GetIndex("frame_rate",frame_rate_value, BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_FPSCTRL), "combobox"); // ��ǰʹ�õĵ�֡�ʲ���
    GetIndex("preinstall", preinstall_value,BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_PRESETCTRL), "combobox"); // ��ǰʹ�õ�Ԥ�����
    GetIndex("Speak_Mode",speakmode_value, BRAC_GetSDKOptionInt(BRAC_SO_AUDIO_CAPTUREMODE), "combobox"); // ��ǰʹ�õ���Ƶ����ģʽ
	
    GetIndex("audio_vadctrl", null,BRAC_GetSDKOptionInt(BRAC_SO_AUDIO_VADCTRL), "checkbox");// ��ǰʹ�õľ������
    GetIndex("audio_echoctrl", null,BRAC_GetSDKOptionInt(BRAC_SO_AUDIO_ECHOCTRL), "checkbox"); // ��ǰʹ�õĻ�������
    GetIndex("audio_nsctrl", null,BRAC_GetSDKOptionInt(BRAC_SO_AUDIO_NSCTRL), "checkbox"); // ��ǰʹ�õ���������
    GetIndex("audio_agcctrl", null,BRAC_GetSDKOptionInt(BRAC_SO_AUDIO_AGCCTRL), "checkbox"); // ��ǰʹ�õ��Զ�����
    GetIndex("Checkbox_P2P", null,BRAC_GetSDKOptionInt(BRAC_SO_AUDIO_VADCTRL), "checkbox") ;// P2P
    GetID("current_resolution").innerHTML = BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_WIDTHCTRL) + "x" + BRAC_GetSDKOptionInt(BRAC_SO_LOCALVIDEO_HEIGHTCTRL) + ")";



}


// ���ÿؼ���ʼֵ
function GetIndex(control_id,valueArray,value,type) {
    if (type == "combobox") { // ������
		var valueIndex=0;
		for(var i=0;i<valueArray.length;i++)
		{
			if(value==valueArray[i])
			{
				valueIndex=i;
				break;
			}
		}
		GetID(control_id).selectedIndex=valueIndex;
    }
    else { // ��ѡ��
        if (value == 1) // 1Ϊ�� 
            GetID(control_id).checked = true;
        else
            GetID(control_id).checked = false;
    }
}

// ���� �߼����ý��� x����
function SetThePos() {
    var TheBodyWidth = document.body.offsetWidth;
    GetID("advanceset_div").style.marginLeft = (TheBodyWidth - 464) / 2 + 87 + "px";
}
// ��ʾ �������  ����
function BtnAdjust() {
    BRAC_ShowLVProperty("");
}
// Ӧ������
function BtnApply() {
    BRAC_SetSDKOption(BRAC_SO_LOCALVIDEO_APPLYPARAM, 1);
    setTimeout(initControlSelected, 500);
}