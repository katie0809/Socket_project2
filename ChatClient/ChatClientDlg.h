
// ChatClientDlg.h : ��� ����
//

#pragma once


// CChatClientDlg ��ȭ ����
class CChatClientDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CChatClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	
	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	class CMyDialog *m_dialog;
	class puzzle_gameDlg *m_dialog2;
	virtual void OnOK();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonOmok();
	//afx_msg void OnBnClickedButtonConnect(); // ���� �����ư

};
