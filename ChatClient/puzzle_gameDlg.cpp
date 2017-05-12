// puzzle_gameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChatClient.h"
#include "puzzle_gameDlg.h"
#include "afxdialogex.h"

#include <stdio.h>
#pragma warning(disable:4996)
using namespace std;

// puzzle_gameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(puzzle_gameDlg, CDialogEx)

puzzle_gameDlg::puzzle_gameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

puzzle_gameDlg::~puzzle_gameDlg()
{
}

void puzzle_gameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(puzzle_gameDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, &puzzle_gameDlg::OnBnClickedStart)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_END, &puzzle_gameDlg::OnBnClickedEnd)
END_MESSAGE_MAP()




// puzzle_gameDlg 메시지 처리기입니다.

void puzzle_gameDlg::InitGame() {

	int k = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			m_iGame[i][j] = k++;
		}
	}
	//게임에 사용할 그림 인덱스 초기화. 1부터 25까지 차례대로 넣어준다.
	//즉, 얘네들이 각 그림조각의 인덱스인 iNum이다
	//빈칸 인덱스는 -1
	m_iGame[4][4] = -1; //빈칸(=마지막칸)은 -1로

						//다이얼로그 화면을 초기화해준다.
						//전체그림을 그려준다.
	CClientDC pDC(this);//현재 다이얼로그의 타이틀바를 제외한 영역을 얻는다.
	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	CPen *p = pDC.SelectObject(&pen);
	CBrush *b = pDC.SelectObject(&brush);

	pDC.Rectangle(15, 20, 15 + 5 * 61, 20 + 5 * 61);
	pDC.SelectObject(p);
	pDC.SelectObject(b);

	CDC memDC;

	memDC.CreateCompatibleDC(&pDC);//CDC와 CClinetDC를 연결해주는 구문

	CBitmap *oldbm = memDC.SelectObject(&m_hint);
	pDC.BitBlt(340, 30, 160, 160, &memDC, 0, 0, SRCCOPY);

	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < 5; k++) {
			ViewPic(j, k, m_iGame[j][k]);
		}
	}
}

void puzzle_gameDlg::ViewPic(int iRow, int iCol, int iNum) {

	//25개의 그림 조각을 그려주는 함수
	//iRow, iCol위치에 iNum번째 그림조각을 그려준다.

	if (iNum == -1) {
		//해당 조각이 빈칸일경우
		return;
	}

	int iSize = 60;
	// 300/5
	int iX = (iNum % 5);
	int iY = (iNum / 5);

	CClientDC pDC(this);
	CDC memDC;

	memDC.CreateCompatibleDC(&pDC);

	CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
	pDC.BitBlt(15 + (iCol*(iSize + 1)), 20 + (iRow*(iSize + 1)), iSize, iSize, &memDC, (iX*iSize), (iY*iSize), SRCCOPY);
	memDC.SelectObject(&oldbm);
}

bool puzzle_gameDlg::FindEmptyIndex(int iRow, int iCol, int &iEmptyRow, int &iEmptyCol) {

	//해당 인덱스의 상하좌우 4방향을 검사해서
	//그림이 움직일 수 있는 빈칸을 찾는다.
	//못찾으면 false 리턴

	if (m_iGame[iRow][iCol] == -1) {
		return false;
	}
	if (m_iGame[iRow + 1][iCol] == -1) {
		iEmptyRow = iRow + 1;
		iEmptyCol = iCol;
		return true;
	}
	if (m_iGame[iRow][iCol + 1] == -1) {
		iEmptyCol = iCol + 1;
		iEmptyRow = iRow;
		return true;
	}
	if (m_iGame[iRow - 1][iCol] == -1) {
		iEmptyRow = iRow - 1;
		iEmptyCol = iCol;
		return true;
	}
	if (m_iGame[iRow][iCol - 1] == -1) {
		iEmptyRow = iRow;
		iEmptyCol = iCol - 1;
		return true;
	}
	return false;


}

bool puzzle_gameDlg::PosToIndex(CPoint pnt, int &iRow, int &iCol) {
	//현재 커서가 클릭된 위치를 받아와서(OnLBUttonDown)
	//iRow와 iCol을 정해준다.
	int i = 0;
	int j = 0;

	for (j = 0; j < 5; j++) {
		if ((pnt.x > 15 + j * 61 - 1) && (pnt.x < 15 + (j + 1) * 61 - 1))
			break;
	}
	//iCol결정
	for (i = 0; i < 5; i++) {
		if ((pnt.y > 20 + i * 61 - 1) && (pnt.y < 20 + (i + 1) * 61 - 1))
			break;
	}
	//iRow결정


	if (i == 5 || j == 5) return false;
	//그림 바깥족을 클릭한경우
	else {
		iCol = j;
		iRow = i;
		return true;
	}

}

void puzzle_gameDlg::MovePic(int iRow, int iCol, int iEmptyRow, int iEmptyCol) {

	//emptyRow, emptyCol로 그림조각을 옮겨준다.
	m_iGame[iEmptyRow][iEmptyCol] = m_iGame[iRow][iCol];
	ViewPic(iEmptyRow, iEmptyCol, m_iGame[iRow][iCol]);
	m_iGame[iRow][iCol] = -1;
	ClearPic(iRow, iCol);
}

void puzzle_gameDlg::ClearPic(int iRow, int iCol) {

	//해당 위치의 그림을 지운다.
	//즉, 다이얼로그와 같은 색깔로 해당위치에 60*60크기의 사각형을 그려준다.

	CClientDC dc(this);

	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	CPen *p = dc.SelectObject(&pen);
	CBrush *b = dc.SelectObject(&brush);

	dc.Rectangle(15 + iCol * 61, 20 + iRow * 61, 15 + iCol * 61 + 60, 20 + iRow * 61 + 60);
	//dc handler, left, top, right, bottom 즉, 왼쪽 위 꼭지점과 오른쪽 아래 꼭지점 좌표를 넣어준다.
	dc.SelectObject(p);
	dc.SelectObject(b);

}

void puzzle_gameDlg::EndingView() {

	//전체그림을 그려준다.
	CClientDC pDC(this);//현재 다이얼로그의 타이틀바를 제외한 영역을 얻는다.
	CDC memDC;
	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	CPen *p = pDC.SelectObject(&pen);
	CBrush *b = pDC.SelectObject(&brush);

	pDC.Rectangle(15, 20, 15 + 5 * 61, 20 + 5 * 61);
	pDC.SelectObject(p);
	pDC.SelectObject(b);

	memDC.CreateCompatibleDC(&pDC);//CDC와 CClinetDC를 연결해주는 구문

	CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
	pDC.BitBlt(17, 23, 300, 300, &memDC, 0, 0, SRCCOPY);
	//bitblt함수를 사용하여 실제 bmp그림파일을 화면에 출력한다.
	//좌표 10,10위치에 300*300의 크기로 그림을 그린다.
	//원본그림의 왼쪽 위 포인트를 0,0으로 설정한다.
	//bmp파일을 사용하므로 모든 출력은 픽셀을 기준으로 한다.
}

void puzzle_gameDlg::Shuffle() {


	int iRow = -1;
	int iCol = -1;
	int iEmptyRow = -1;
	int iEmptyCol = -1;

	int count = 1000;
	//1000번 섞는다

	srand((unsigned)time(NULL));
	while (count--) {
		bool b = false;
		iRow = rand() % 5;
		iCol = rand() % 5;
		//인덱스인 iRow와 iCol을 랜덤하게 선택한다.
		b = FindEmptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
		if (iRow < 0 || iCol < 0 || iEmptyCol < 0 || iEmptyRow < 0) continue;
		if (iRow > 4 || iCol > 4 || iEmptyCol > 4 || iEmptyRow > 4) continue;
		//잘못된 인덱스가 선택되는 경우 continue;
		if (b == true) {
			MovePic(iRow, iCol, iEmptyRow, iEmptyCol);
			//			Sleep(10); //섞이는 과정을 보여준다.
		}
	}

}

void puzzle_gameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int iRow = -1;
	int iCol = -1;
	int iEmptyRow = -1;
	int iEmptyCol = -1;

	bool b = PosToIndex(point, iRow, iCol);

	if (b == true) {
		//클릭하는 경우
		b = FindEmptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
		//주변의 empty index를 찾는다
		if (b == true) {
			//주변에 움직일 수 있는 인덱스가 있는경우
			MovePic(iRow, iCol, iEmptyRow, iEmptyCol);
			//해당 위치로 그림을 옮긴다.

		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void puzzle_gameDlg::OnBnClickedStart()
{	//조각그림을 섞어준다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (start_game) {
		InitGame();
		Shuffle();
		SetTimer(0, 1000, NULL);
		start_game = false;
	}//start버튼 두번 이상 누를경우 응답하지 않도록한다.
	else return;
}


void puzzle_gameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString clock = NULL;
	m_time++;
	clock.Format(_T("%02d:%02d:%02d"), m_time / 3600, m_time / 60 % 60, m_time % 60);
	SetDlgItemText(IDC_TIMER, clock);
	UpdateData(false);
	CDialogEx::OnTimer(nIDEvent);
}


BOOL puzzle_gameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}

		m_bitMain.LoadBitmapW(IDB_MAIN);
		//m_bitMain이라는 비트맵형 변수에 IDB_MAIN이라는 리소스를 사용하도록 한다
		m_hint.LoadBitmapW(IDB_HINT);
		//m_hint라는 비트맵형 변수에 IDB_HINT라는 리소스를 사용하도록한다.
	}

	InitGame();
	m_time = 0;
	start_game = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void puzzle_gameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	EndingView();

}

bool puzzle_gameDlg::isGameEnd() {

	int k = 0;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (m_iGame[i][j] == k) continue;
			else return false;
		}
	}
	return true;
}
void puzzle_gameDlg::solve() {

	/*----------------------------------------------------------------------------*/
	//solve()기능은 아직 구현되지 않았습니다.
	//단지 보여주는 방식을 시각적으로 설명하기 위한
	//임시 코드입니다.
	//shuffle을 500번 시행합니다.
	/*----------------------------------------------------------------------------*/

	int iRow = -1;
	int iCol = -1;
	int iEmptyRow = -1;
	int iEmptyCol = -1;

	int count = 500;
	//500번 섞는다

	srand((unsigned)time(NULL));
	while (count--) {
		bool b = false;
		iRow = rand() % 5;
		iCol = rand() % 5;
		//인덱스인 iRow와 iCol을 랜덤하게 선택한다.
		b = FindEmptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
		if (iRow < 0 || iCol < 0 || iEmptyCol < 0 || iEmptyRow < 0) continue;
		if (iRow > 4 || iCol > 4 || iEmptyCol > 4 || iEmptyRow > 4) continue;
		//잘못된 인덱스가 선택되는 경우 continue;
		if (b == true) {
			MovePic(iRow, iCol, iEmptyRow, iEmptyCol);
			Sleep(100); //섞이는 과정을 보여준다.
		}
	}
	Sleep(1000);
	EndingView();
	Sleep(1000);
	return;
}

void puzzle_gameDlg::OnBnClickedEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*----------------------------------------------------------------------------------------------------------------------*/
	//
	//랭킹 기능을 추가하는 소스입니다.
	//
	//FILE *file;
	//file = fopen("ranking.txt", "a+");
	//랭킹 파일을 열어준다.
	//fprintf(file, "%d-%d-%d %d:%d:%d\n", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	//fclose(file);
	//
	/*----------------------------------------------------------------------------------------------------------------------*/


	CTime t;
	KillTimer(0);

	if (isGameEnd()) {
		//게임을 클리어한경우
		AfxMessageBox(L"Congratulation, you clear the game");
	}
	else {
		//게임을 클리어하지 못한경우
		//AfxMessageBox(L"Game is not cleared=(");
		CString help;
		help.Format(_T("Sorry, you failed to finish the game"));
		SetDlgItemText(IDC_HELP, help);
		Sleep(1600);
		help.Format(_T("Now we'll show you the solution=D"));
		SetDlgItemText(IDC_HELP, help);
		Sleep(1600);
		solve();//솔루션을 보여준다.
		Sleep(1000);
	}
	CDialogEx::OnOK();
}
