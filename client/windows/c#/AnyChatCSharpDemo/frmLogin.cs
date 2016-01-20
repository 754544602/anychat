using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Xml;
using System.Threading;

namespace AnyChatCSharpDemo
{
    public partial class frmLogin : Form
    {

        #region �������

        
        
        /// <summary>
        /// �û���¼�˺�
        /// </summary>
        public static string m_UserName = "";
        /// <summary>
        /// ��Ƶ������IP
        /// </summary>
        public static string m_VideoServerIP = "211.155.25.89";
       
        /// <summary>
        /// ������Ƶ�ɼ����
        /// </summary>
        public static int m_LocalVideoWidth = 320;
        /// <summary>
        /// ������Ƶ�ɼ��߶�
        /// </summary>
        public static int m_LocalVideoHeight = 240;
        /// <summary>
        /// ������TCP��IP
        /// </summary>
        public static int m_VideoTcpPort = 8906;//8906
        /// <summary>
        /// Ӧ��ID
        /// </summary>
        public static string m_AppGuid = string.Empty;

        string mPath = Application.StartupPath + "/record.xml";

        XmlDocument mXmlDoc = new XmlDocument();

        #endregion

        #region ���캯��

        public frmLogin()
        {
            InitializeComponent();
        }

        #endregion

        #region ��¼

        private void btn_login_Click(object sender, EventArgs e)
        {
            string m_User = txt_username.Text.Trim();
            string m_Pass = txt_password.Text.Trim();
            if (m_User.Length == 0)
            {
                MessageBox.Show("�˺Ų���Ϊ��", "��ʾ");
                return;
            }


            if (m_UserName == "")
            {
                m_UserName = m_User;
            }
            m_VideoServerIP = txt_serverip.Text.Trim();


            try
            {
                m_VideoTcpPort = Convert.ToInt32(tb_port.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("�˿ں�������");
            }

            this.Hide();
            frmRoom m_FR = new frmRoom();
            m_FR.Show();

            RecordLoginTrace();

        }

        #endregion

        #region �˳�

        private void btn_canel_Click(object sender, EventArgs e)
        {
            System.Environment.Exit(0);
        }

        #endregion

       
        #region �������

        private void frmLogin_Load(object sender, EventArgs e)
        {
            lbl_status.Text = string.Empty;

            if (File.Exists(mPath))
            {
                mXmlDoc.Load(mPath);
                LoadRecordTrace();
            }
            else
            {
                txt_serverip.Text = "demo.anychat.cn";
                tb_port.Text = "8906";                
            }
        }

        #endregion


        #region ��¼��Ϣ���洦��

        /// <summary>
        /// �����ļ�ȱʧʱ�Զ������ļ�
        /// </summary>
        public void CreateXMLDoc()
        {
            if (!File.Exists(mPath))
            {
                XmlDocument xmldoc = new XmlDocument();

                //����XML����������
                xmldoc.AppendChild(xmldoc.CreateXmlDeclaration("1.0", "UTF-8", null));
                XmlElement rMainNode = xmldoc.CreateElement("", "settings", "");
                rMainNode.IsEmpty = false;
                xmldoc.AppendChild(rMainNode);

                XmlElement rPreviousRecord = xmldoc.CreateElement("", "previousrecord", "");
                rPreviousRecord.IsEmpty = false;
                rMainNode.AppendChild(rPreviousRecord);

                xmldoc.Save(mPath);
            }
        }


        /// <summary>
        /// �����¼��Ϣ
        /// </summary>
        private void RecordLoginTrace()
        {
            try
            {
                if (!File.Exists(mPath))
                {
                    CreateXMLDoc();
                    mXmlDoc.Load(mPath);
                }

                PreviousRecordValue("previousrecord", "ip", txt_serverip.Text);
                PreviousRecordValue("previousrecord", "port", tb_port.Text);
                PreviousRecordValue("previousrecord", "userName", txt_username.Text);
                PreviousRecordValue("previousrecord", "appGuid", txt_appGuid.Text);

            }
            catch (Exception ex)
            {
                
            }
        }

        private void PreviousRecordValue(string rAttribute, string rN, string rValue)
        {
            XmlNode rMainNode = mXmlDoc.SelectSingleNode("settings");
            XmlNode rNode = rMainNode.SelectSingleNode(rAttribute);
            XmlNodeList rList = rNode.ChildNodes;
            bool bExists = false;
            XmlElement rElem = null;
            foreach (XmlNode x in rList)
            {
                rElem = (XmlElement)x;
                string rVal = rElem.GetAttribute("value");
                if (rElem.Name.Equals(rN))
                {
                    bExists = true;
                    break;
                }
            }
            if (bExists && rElem != null)
            {

                rElem.SetAttribute("value", rValue);
            }
            else
            {
                XmlElement rElement = mXmlDoc.CreateElement(rN);
                rElement.SetAttribute("value", rValue);
                rNode.AppendChild(rElement);
            }

            mXmlDoc.Save(mPath);
        }

        private void LoadRecordTrace()
        {
            string[] record = getPreviousRecord("previousrecord");
            if (record != null)
            {
                txt_serverip.Text = record[0];
                tb_port.Text = record[1];
                txt_username.Text = record[2];
                txt_appGuid.Text = record[3];
            }

        }

        private String[] getPreviousRecord(string rAttribute)
        {
            string[] record = new string[4];
            XmlNode rMainNode = mXmlDoc.SelectSingleNode("settings");
            XmlNode rNode = rMainNode.SelectSingleNode(rAttribute);
            XmlNodeList rList = rNode.ChildNodes;
            bool bExists = false;
            XmlElement rElem = null;
            foreach (XmlNode x in rList)
            {
                bExists = true;
                rElem = (XmlElement)x;
                string rVal = rElem.GetAttribute("value");
                switch (rElem.Name)
                {
                    case "ip":
                        record[0] = rVal;
                        break;
                    case "port":
                        record[1] = rVal;
                        break;
                    case "userName":
                        record[2] = rVal;
                        break;
                    case "appGuid":
                        record[3] = rVal;
                        break;

                }
            }
            if (bExists)
                return record;
            return null;
        }



        #endregion

    }
}