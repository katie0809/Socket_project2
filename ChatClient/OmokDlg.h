#pragma once

#ifndef _OMOKDLG_H
#define _OMOKDLG_H

#include <afxwin.h>
#include "SocCom.h"
#include "Resource.h"

class OmokWnd;
class OmokDlg : public CDialog {
public:
	enum { IDD10 = IDD_OMOKWND };

	OmokWnd *omokWnd;
	CSocCom m_socCom;
	CString m_strSend;

	OmokDlg(CWnd *parent = NULL);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	

	void SendData(int type, CString strTmp);


protected:
	//afx_msg void OnStartButtonClicked();
	afx_msg void OnConnectButtonClicked();
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
   // afx_msg void OnClose();
	DECLARE_MESSAGE_MAP();
};

#endif
