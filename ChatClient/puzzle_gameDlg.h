#pragma once


// puzzle_gameDlg 대화 상자입니다.

class puzzle_gameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(puzzle_gameDlg)

public:
	puzzle_gameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~puzzle_gameDlg();
	CBitmap m_bitMain;
	CBitmap m_hint;
	int m_time;
	bool start_game;
	void EndingView();
	void Shuffle();
//	BOOL OnInitDialog();
	void InitGame();
	void ViewPic(int iRow, int iCol, int iNum);
	bool FindEmptyIndex(int iRow, int iCol, int &iEmptyRow, int &iEmptyCol);
	bool PosToIndex(CPoint pnt, int & iRow, int & iCol);
	void MovePic(int iRow, int iCol, int iEmptyRow, int iEmptyCol);
	void ClearPic(int iRow, int iCol);
	int m_iGame[5][5];
// 대화 상자 데이터입니다.

	enum { IDD = IDD_DIALOG2 };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	BOOL InitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	bool isGameEnd();
	void solve();
	afx_msg void OnBnClickedEnd();
};
