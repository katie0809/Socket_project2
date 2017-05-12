#pragma once
#include "SocCom.h"
#include "afxwin.h"

// CMyDialog 대화 상자입니다.

class CMyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialog)

public:
	CMyDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMyDialog();
	CSocCom m_socCom;
	bool m_gameTurn;

	//퍼즐게임을 위한 데이터 선언부입니다.
	CBitmap m_bitMain;
	CBitmap m_hint;
	int m_time;
	bool start_game;
	void EndingView();
	void Shuffle();
	bool isGameEnd();
	void solve();
	//	BOOL OnInitDialog();
	void InitGame();
	void ViewPic(int iRow, int iCol, int iNum);
	bool FindEmptyIndex(int iRow, int iCol, int &iEmptyRow, int &iEmptyCol);
	bool PosToIndex(CPoint pnt, int & iRow, int & iCol);
	void MovePic(int iRow, int iCol, int iEmptyRow, int iEmptyCol);
	void ClearPic(int iRow, int iCol);
	int m_iGame[7][7];




// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SendData(int type, CString strTmp);
	void NumToIndex(int iNum, int & iRow, int & iCol);
	CListBox m_list;
	afx_msg void OnBnClickedButton2();
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
	CString m_strSend;
	virtual void OnOK();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
