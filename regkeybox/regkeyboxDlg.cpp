// regkeyboxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "regkeybox.h"
#include "regkeyboxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CregkeyboxDlg dialog



CregkeyboxDlg::CregkeyboxDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CregkeyboxDlg::IDD, pParent)
    , smoosh(false)
    , sPort(_T(""))
    , sRN(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CregkeyboxDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RADIO3, somegroup);
    DDX_Control(pDX, IDC_EDIT3, port);
    DDX_Radio(pDX, IDC_RADIO3, smoosh);
    DDX_Text(pDX, IDC_EDIT3, sPort);
    DDX_Text(pDX, IDC_EDIT1, sRN);
    DDX_Control(pDX, IDC_EDIT1, cRN);
}

BEGIN_MESSAGE_MAP(CregkeyboxDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
//  ON_NOTIFY(NM_OUTOFMEMORY, IDC_HOTKEY1, OnNMOutofmemoryHotkey1)
ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
//ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CregkeyboxDlg message handlers

BOOL CregkeyboxDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    pFileName = "temp.ini";
    //somegroup.CheckRadioButton(IDC_RADIO3,IDC_RADIO4,IDC_RADIO3);
    //smoosh = true;
    if( !f.Open( pFileName, CFile::modeCreate | CFile::modeWrite, &e ) ){
        OnCancel();
        return FALSE;
    }
    // --------- load registry variables -----
    CString g_sCustID;
    CSysRegKey* rkRNSI1 = new CSysRegKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\T Systems\\PMA", KEY_ALL_ACCESS);
    CSysRegKey* rkRNSI2 = new CSysRegKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\T Systems\\ELA", KEY_ALL_ACCESS);
    CSysRegKey* rkRNSI3 = new CSysRegKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\T Systems\\SCA", KEY_ALL_ACCESS);
    // if at least one has succeeded
    if ((rkRNSI1->OpenState() && rkRNSI1->GetStringValue("CustID",&g_sCustID)) ||
        (rkRNSI1->OpenState() && rkRNSI1->GetStringValue("CustID",&g_sCustID)) ||
        (rkRNSI1->OpenState() && rkRNSI1->GetStringValue("CustID",&g_sCustID)) ){
        cRN.SetWindowText(g_sCustID);
    }
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CregkeyboxDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CregkeyboxDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//void CregkeyboxDlg::OnNMOutofmemoryHotkey1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//  // TODO: Add your control notification handler code here
//  *pResult = 0;
//}

void CregkeyboxDlg::OnBnClickedRadio3()
{
    port.EnableWindow(false);// TODO: Add your control notification handler code here
    smoosh=false;
}

void CregkeyboxDlg::OnBnClickedRadio4()
{
    port.EnableWindow(true);// TODO: Add your control notification handler code here
    smoosh=true;
}

void CregkeyboxDlg::OnBnClickedCancel()
{
    char *msg="[REGBOX]\nStatus = Canceled";
    f.Write(msg,strlen(msg));
    f.Close();
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CregkeyboxDlg::OnBnClickedOk()
{
    char data[100];
    cRN.GetWindowText(sRN);
    if (!Trust(sRN)){
        MessageBox("Invalid registration number","Error");
        return;
    }
    port.GetWindowText(sPort);
    if (!smoosh) // port not selected
        sprintf(data,"[REGBOX]\nStatus = OK\nPort = 33376%s\nRegNum = %s\n",sPort,sRN);
    else if (sPort.IsEmpty() || atoi((LPCSTR)sPort) < 0 || atoi((LPCSTR)sPort) > 65535) {
        MessageBox("Invalid port selected","Error");
        return;
    } else {
        sprintf(data,"[REGBOX]\nStatus = OK\nPort = %s\nRegNum = %s\n",sPort,sRN);
    }
    f.Write(data,strlen(data));
    f.Close();
    OnOK();
}
// check the registration key
bool CregkeyboxDlg::Trust(CString& g_sCustID)
{
    char *cRKey = new char[g_sCustID.GetLength()+1];
    strcpy(cRKey, g_sCustID);
    // push the key in working from right to left for bits
    bitstream bsStream;
    int iBitNum=BITSTREAMLENGTH-1,iByteNum=0;
    char cByte;
    for (int k=0; k < SECTIONS; k++){ // for every section
        for (int i=0; i < CHARSPERSECTION; i++){ // for every character
            cByte = cRKey[iByteNum++];
            // deflate it to 5 bits
            if (cByte > '1' && cByte < '8')
                cByte -= '2'; //lowest values (0-5)
            else if (cByte >= 'A' && cByte <= 'Z')
                cByte -= 'A' - 6; // top values (6-31)
            else
                return false;
            for (int j=BITSPERCHAR-1; j >= 0; j--){ // for every bit
                if ((cByte >> j) & 1)
                        bsStream.set(iBitNum);
                iBitNum--;
            }
        }
        iByteNum++; //skip the delimiter
    }
    // load
    byte pbData[BYTESTREAMLENGTH+1];
    for (int i=0;i < BYTESTREAMLENGTH;i++)
        pbData[i] = getbyte(bsStream,BITSTREAMLENGTH-(i+1)*8);//reverse filling
    pbData[BYTESTREAMLENGTH]=0;
    // Unencrypt
    byte bCRC=0;
    byte bCRCReal=pbData[BYTESTREAMLENGTH-1]; // do not unencrypt the crc
    for (int i=1;i < BYTESTREAMLENGTH;i++){
        bCRC = bCRC-pbData[i-1]; // crc for the unencrypted data
        pbData[i] = pbData[i]^pbData[i-1];
    }
    if (bCRC != bCRCReal)
        return false;
    // unpack the data
    //g_sCSIP.Format("%d.%d.%d.%d",pbData[5],pbData[6],pbData[7],pbData[8]);
    // zero string termination
    return true;
}
byte CregkeyboxDlg::getbyte(bitstream& bs,int i){
    return (getbit(bs,i+7)<<7)+(getbit(bs,i+6)<<6)+(getbit(bs,i+5)<<5)+(getbit(bs,i+4)<<4)+
           (getbit(bs,i+3)<<3)+(getbit(bs,i+2)<<2)+(getbit(bs,i+1)<<1)+(getbit(bs,i));
}
byte CregkeyboxDlg::getbit(bitstream& bs,int i){
    return bs.test(i) ? 1:0;
}
