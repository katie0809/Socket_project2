#include "stdafx.h"
#include "OmokDlg.h"
#include "OmokWnd.h"
#include "afxdialogex.h"
#include "SocCom.h"


BEGIN_MESSAGE_MAP(OmokDlg, CDialog)
	ON_BN_CLICKED(IDC_CONNECTBUTTON, OnConnectButtonClicked)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_WM_CLOSE()
END_MESSAGE_MAP()





OmokDlg::OmokDlg(CWnd *parent)
	:CDialog(OmokDlg::IDD10, parent) {

}

BOOL OmokDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	// 메모리 할당하기
	this->omokWnd = new OmokWnd;

	this->omokWnd->Create(NULL, _T("오목"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 750, 650),this,10000);

	return FALSE;
}


void OmokDlg::OnConnectButtonClicked() {

	//서버에 접속합니다
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString str;
	m_socCom.Create();

	if (m_socCom.Connect(m_strSend, 5000)) {
		str.Format(L"서버 접속성공");
		AfxMessageBox(str, MB_OK);
	}
	m_socCom.Init(this->m_hWnd);

}

LPARAM OmokDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	char pTmp[256];

	// 데이터를 pTmp에 받는다.
	m_socCom.Receive(pTmp, 256);

	CString temp;
	temp.Format(_T("%s"), pTmp);

	CString x;
	CString y;
	

	AfxExtractSubString(x, temp, 0, '~');
	AfxExtractSubString(y, temp, 1, '~');
	

	this->omokWnd->x = _ttoi(x);
	this->omokWnd->y = _ttoi(y);
	this->omokWnd->OmokInner();
	this->Invalidate();

	
	return LPARAM();
}

void OmokDlg::SendData(int type, CString strTmp) {

	//send data
	UpdateData(TRUE);
	int len = strTmp.GetLength() + 1;
	LPTSTR stmp = new TCHAR[len];
	_tcscpy_s(stmp, len, strTmp);
	m_socCom.Send(stmp, 256);

}

void OmokDlg::OnClose() {
	if (this->omokWnd != NULL) {
		this->omokWnd->DestroyWindow();
		delete this->omokWnd;
	}
	EndDialog(0);
}