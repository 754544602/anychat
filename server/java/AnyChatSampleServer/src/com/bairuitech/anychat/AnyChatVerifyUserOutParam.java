package com.bairuitech.anychat;		// �����޸İ�������


public class AnyChatVerifyUserOutParam{
	private int userid;
	private int userlevel;
	private String nickname;
	
	public int GetUserId()	{ return userid; }
	public void SetUserId(int id) {	userid = id; }
	
	public int GetUserLevel() {	return userlevel;	}
	public void SetUserLevel(int level)	{	userlevel = level;	}
	
	public String GetNickName()	{	return nickname;	}
	public void SetNickName(String name)	{	nickname = name;	}
}

