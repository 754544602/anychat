package com.bairuitech.demo;

import java.awt.Color;
import java.awt.Font;
import java.awt.Insets;
import java.awt.LayoutManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

import com.bairuitech.anychat.AnyChatCoreSDK;

public class LoginJFrame extends JFrame implements ActionListener {
	public JPanel mPanlLogin1;
	public JPanel mPanlLogin2;
	public JPanel mPanlLogin3;
	public JPanel mPanlLogin4;
	public JLabel mLabelLogin;
	public JLabel mLabelName;
	public JLabel mLabelPWD;
	public JTextField mTxtName;
	public JTextField mTxtServeIp;
	public JTextField mTxtServePort;
	public JPasswordField mTxtPwd;
	public JButton mBtnLogin;
	public JCheckBox mCheckAnnymous;
	public JButton mBtnSeting;
	private AnyChatCoreSDK anychat;
	public static final String SERVER_IP = "demo.anychat.cn";
	public static final int SERVER_PORT = 8906;

	public LoginJFrame(AnyChatCoreSDK anychat) {
		mPanlLogin1 = new JPanel();
		mPanlLogin2 = new JPanel();
		mPanlLogin3 = new JPanel();
		mPanlLogin4 =new JPanel();
		mLabelLogin = new JLabel();
		mLabelName = new JLabel();
		mLabelPWD = new JLabel();
		mTxtName = new JTextField();
		mTxtServeIp = new JTextField(SERVER_IP);
		mTxtServePort= new JTextField(String.valueOf(SERVER_PORT));
		mTxtPwd = new JPasswordField();
		mBtnLogin = new JButton();
		mBtnSeting = new JButton();
		mCheckAnnymous=new JCheckBox();
		this.anychat = anychat;
		initJFrame();

	}
	public void initJFrame() {
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);// ���ùرտ�ܵ�ͬʱ��������
		this.setSize(350, 250);// ���ÿ�ܴ�СΪ��300,��200
		this.setResizable(false);// ���ÿ�ܲ����Ըı��С
		this.setTitle("�û���¼");// ���ÿ�ܱ���
		this.mPanlLogin1.setLayout(null);// ������岼�ֹ���
		this.mPanlLogin1.setBackground(Color.cyan);// ������屳����ɫ
		this.mPanlLogin2.setBackground(Color.blue);// ������屳����ɫ
		this.mPanlLogin3.setBackground(Color.blue);// ������屳����ɫ
		this.mPanlLogin4.setBackground(Color.yellow);// ������屳����ɫ
		this.mPanlLogin2.setLayout(new BoxLayout(mPanlLogin2, BoxLayout.Y_AXIS));
		this.mLabelLogin.setText("�û���¼");// ���ñ�ǩ����
		this.mLabelLogin.setFont(new Font("����", Font.BOLD | Font.ITALIC, 14));// ���ñ�ǩ����
		this.mLabelLogin.setForeground(Color.RED);// ���ñ�ǩ������ɫ
		this.mLabelName.setText("�û���:");
		this.mLabelPWD.setText("��    ��:");
		this.mBtnLogin.setText("��¼");
		mBtnLogin.addActionListener(this);
		mBtnSeting.addActionListener(this);
		this.mBtnSeting.setText("����");
		mCheckAnnymous.setText("�ο͵�½");
		this.mLabelLogin.setBounds(120, 15, 60, 20);// ���ñ�ǩx����120,y����15,��60,��20
		this.mLabelName.setBounds(50, 55, 60, 20);
		this.mLabelPWD.setBounds(50, 85, 60, 25);
		this.mTxtName.setBounds(110, 55, 120, 20);
		this.mTxtPwd.setBounds(110, 85, 120, 20);
		this.mCheckAnnymous.setBounds(150, 115, 80, 20);
		this.mPanlLogin1.add(mLabelLogin);// ���ر�ǩ�����
		this.mPanlLogin1.add(mLabelName);
		this.mPanlLogin1.add(mLabelPWD);
		this.mPanlLogin1.add(mTxtName);
		this.mPanlLogin1.add(mTxtPwd);
		this.mPanlLogin1.add(mCheckAnnymous);
		this.mPanlLogin3.add(mBtnSeting);
		this.mPanlLogin3.add(mBtnLogin);
		mPanlLogin4.add(new JLabel("��������ַ:"));
		this.mPanlLogin4.add(mTxtServeIp);
		mPanlLogin4.add(new JLabel("�������˿�:"));
		this.mPanlLogin4.add(mTxtServePort);
		this.mPanlLogin2.add(mPanlLogin3);
		this.mPanlLogin2.add(mPanlLogin4);
		this.add(mPanlLogin1, "Center");// ������嵽���
		this.add(mPanlLogin2, "South");// ������嵽���
		mPanlLogin4.setVisible(false);
		this.setVisible(true);// ���ÿ�ܿ���
	}

	public void hideLogin() {
		this.setVisible(false);
		this.dispose();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		if (e.getSource() == mBtnLogin) {
			
			String strName = mTxtName.getText();
			String strPasswd = String.valueOf(mTxtPwd.getPassword());
			String strServerIp=mTxtServeIp.getText();
			String strServerPort=mTxtServePort.getText();
			if(strServerIp.length()==0&&strServerPort.length()==0)
				return;
			anychat.Connect(strServerIp, Integer.valueOf(strServerPort));
			boolean bAnnyous=mCheckAnnymous.isSelected();
			if(bAnnyous)
			{
				anychat.Login("java", "");
				return;
			}
			if (strName.length() == 0) {
				JOptionPane.showMessageDialog(null, "�˺Ų���Ϊ��", "����",
						JOptionPane.ERROR_MESSAGE);
			} else if (strPasswd.length() == 0) {
				JOptionPane.showMessageDialog(null, "���벻��Ϊ��", "����",
						JOptionPane.ERROR_MESSAGE);
			} else {
				anychat.Login(strName, strPasswd);
			}
		}
		if (e.getSource() == mBtnSeting) {
			
			System.out.println("btnset");
			if(mPanlLogin4.isVisible())
			{
				mPanlLogin4.setVisible(false);
				mBtnSeting.setText("����");
			}
			else 
			{
				mPanlLogin4.setVisible(true);
				mBtnSeting.setText("����");
			}
		}
	}
}
