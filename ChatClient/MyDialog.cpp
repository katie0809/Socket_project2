// MyDialog.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "ChatClient.h"
#include "MyDialog.h"
#include "afxdialogex.h"
#include "SocCom.h"
#pragma warning (disable : 4996)

// CMyDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMyDialog, CDialogEx)

CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_strSend(_T(""))
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_strSend);
}


// CMyDialog 메시지 처리기입니다.

void CMyDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnBnClickedButton2();
	//CDialogEx::OnOK();
}


void CMyDialog::OnBnClickedButton1()
{
	//서버에 접속합니다
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString str;
	m_socCom.Create();

	if (m_socCom.Connect(m_strSend, 5000)) {
		str.Format(L"서버 접속성공");
		SetDlgItemText(IDC_STATIC_CONNECT, str);
	}
	m_socCom.Init(this->m_hWnd);
	m_gameTurn = false;

	InitGame();
	Shuffle();

}

/*----------------------------------------------*/
// 퍼즐게임을 위한 소스입니다.
// 이전 파일과 동일한 부분입니다.
/*----------------------------------------------*/

BOOL CMyDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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

		m_bitMain.LoadBitmapW(IDB_350);
		//m_bitMain이라는 비트맵형 변수에 IDB_MAIN이라는 리소스를 사용하도록 한다

	}

	if (!AfxSocketInit()) {
		//		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return false;
	}

	InitGame();
	m_time = 0;
	start_game = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMyDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.

					   //	EndingView();		// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

void CMyDialog::InitGame() {

	int k = 0;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			m_iGame[i][j] = k++;
		}
	}
	//게임에 사용할 그림 인덱스 초기화. 1부터 25까지 차례대로 넣어준다.
	//즉, 얘네들이 각 그림조각의 인덱스인 iNum이다
	//빈칸 인덱스는 -1
	m_iGame[6][6] = -1; //빈칸(=마지막칸)은 -1로

						//다이얼로그 화면을 초기화해준다.
						//전체그림을 그려준다.
	CClientDC pDC(this);//현재 다이얼로그의 타이틀바를 제외한 영역을 얻는다.
	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	CPen *p = pDC.SelectObject(&pen);
	CBrush *b = pDC.SelectObject(&brush);

	pDC.Rectangle(15, 20, 15 + 7 * 50, 20 + 7 * 50);
	pDC.SelectObject(p);
	pDC.SelectObject(b);

	CDC memDC;

	memDC.CreateCompatibleDC(&pDC);//CDC와 CClinetDC를 연결해주는 구문

								   //	CBitmap *oldbm = memDC.SelectObject(&m_hint);
								   //	pDC.BitBlt(340, 30, 160, 160, &memDC, 0, 0, SRCCOPY);

	for (int j = 0; j < 7; j++) {
		for (int k = 0; k < 7; k++) {
			ViewPic(j, k, m_iGame[j][k]);
		}
	}
}

void CMyDialog::ViewPic(int iRow, int iCol, int iNum) {

	//25개의 그림 조각을 그려주는 함수
	//49개
	//iRow, iCol위치에 iNum번째 그림조각을 그려준다.

	if (iNum == -1) {
		//해당 조각이 빈칸일경우
		return;
	}

	int iSize = 50;
	// 350/7
	int iX = (iNum % 7);
	int iY = (iNum / 7);

	CClientDC pDC(this);
	CDC memDC;

	memDC.CreateCompatibleDC(&pDC);

	CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
	pDC.BitBlt(15 + (iCol*(iSize + 1)), 20 + (iRow*(iSize + 1)), iSize, iSize, &memDC, (iX*iSize), (iY*iSize), SRCCOPY);
	memDC.SelectObject(&oldbm);
}

bool CMyDialog::FindEmptyIndex(int iRow, int iCol, int &iEmptyRow, int &iEmptyCol) {

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

bool CMyDialog::PosToIndex(CPoint pnt, int &iRow, int &iCol) {
	//현재 커서가 클릭된 위치를 받아와서(OnLBUttonDown)
	//iRow와 iCol을 정해준다.
	int i = 0;
	int j = 0;

	for (j = 0; j < 7; j++) {
		if ((pnt.x > 15 + j * 51 - 1) && (pnt.x < 15 + (j + 1) * 51 - 1))
			break;
	}
	//iCol결정
	for (i = 0; i < 7; i++) {
		if ((pnt.y > 20 + i * 51 - 1) && (pnt.y < 20 + (i + 1) * 51 - 1))
			break;
	}
	//iRow결정


	if (i == 7 || j == 7) return false;
	//그림 바깥족을 클릭한경우
	else {
		iCol = j;
		iRow = i;
		return true;
	}

}

void CMyDialog::MovePic(int iRow, int iCol, int iEmptyRow, int iEmptyCol) {

	//emptyRow, emptyCol로 그림조각을 옮겨준다.
	m_iGame[iEmptyRow][iEmptyCol] = m_iGame[iRow][iCol];
	ViewPic(iEmptyRow, iEmptyCol, m_iGame[iRow][iCol]);
	m_iGame[iRow][iCol] = -1;
	ClearPic(iRow, iCol);
}

void CMyDialog::ClearPic(int iRow, int iCol) {

	//해당 위치의 그림을 지운다.
	//즉, 다이얼로그와 같은 색깔로 해당위치에 60*60크기의 사각형을 그려준다.

	CClientDC dc(this);

	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	CPen *p = dc.SelectObject(&pen);
	CBrush *b = dc.SelectObject(&brush);

	dc.Rectangle(15 + iCol * 51, 20 + iRow * 51, 15 + iCol * 51 + 50, 20 + iRow * 51 + 50);
	//dc handler, left, top, right, bottom 즉, 왼쪽 위 꼭지점과 오른쪽 아래 꼭지점 좌표를 넣어준다.
	dc.SelectObject(p);
	dc.SelectObject(b);

}

void CMyDialog::EndingView() {

	//전체그림을 그려준다.
	CClientDC pDC(this);//현재 다이얼로그의 타이틀바를 제외한 영역을 얻는다.
	CDC memDC;
	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	CPen *p = pDC.SelectObject(&pen);
	CBrush *b = pDC.SelectObject(&brush);

	pDC.Rectangle(15, 20, 15 + 7 * 50, 20 + 7 * 50);
	pDC.SelectObject(p);
	pDC.SelectObject(b);

	memDC.CreateCompatibleDC(&pDC);//CDC와 CClinetDC를 연결해주는 구문

	CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
	//	pDC.StretchBlt(17, 23, 300, 300, &memDC, 0, 0, 350, 350, SRCCOPY);
	pDC.BitBlt(17, 23, 350, 350, &memDC, 0, 0, SRCCOPY);
	//bitblt함수를 사용하여 실제 bmp그림파일을 화면에 출력한다.
	//좌표 10,10위치에 300*300의 크기로 그림을 그린다.
	//원본그림의 왼쪽 위 포인트를 0,0으로 설정한다.
	//bmp파일을 사용하므로 모든 출력은 픽셀을 기준으로 한다.
}

void CMyDialog::Shuffle() {

	int iRow = -1;
	int iCol = -1;
	int iEmptyRow = -1;
	int iEmptyCol = -1;

	int count = 2000;
	//1000번 섞는다

	srand((unsigned)time(NULL));
	while (count--) {
		bool b = false;
		iRow = rand() % 7;
		iCol = rand() % 7;
		//인덱스인 iRow와 iCol을 랜덤하게 선택한다.
		b = FindEmptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
		if (iRow < 0 || iCol < 0 || iEmptyCol < 0 || iEmptyRow < 0) continue;
		if (iRow > 6 || iCol > 6 || iEmptyCol > 6 || iEmptyRow > 6) continue;
		//잘못된 인덱스가 선택되는 경우 continue;
		if (b == true) {
			MovePic(iRow, iCol, iEmptyRow, iEmptyCol);
			//			Sleep(10); //섞이는 과정을 보여준다.
		}
	}
}

bool CMyDialog::isGameEnd() {

	int k = 0;

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			if (i == 5 && j == 5) break;

			if (m_iGame[i][j] == k) continue;
			else return false;
		}
	}
	return true;
}

void CMyDialog::solve() {

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
		iRow = rand() % 7;
		iCol = rand() % 7;
		//인덱스인 iRow와 iCol을 랜덤하게 선택한다.
		b = FindEmptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
		if (iRow < 0 || iCol < 0 || iEmptyCol < 0 || iEmptyRow < 0) continue;
		if (iRow > 6 || iCol > 6 || iEmptyCol > 6 || iEmptyRow > 6) continue;
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

void CMyDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int iRow = -1;
	int iCol = -1;
	int iEmptyRow = -1;
	int iEmptyCol = -1;

	bool b = PosToIndex(point, iRow, iCol);

	if (!isGameEnd() && m_gameTurn) {
		if (b == true) {
			//클릭하는 경우
			b = FindEmptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
			//주변의 empty index를 찾는다
			if (b == true) {
				//주변에 움직일 수 있는 인덱스가 있는경우
				CString str;
				str.Format(L"%d", m_iGame[iRow][iCol]);
				SendData(S_click, str);
				//데이터를 보낸다

				MovePic(iRow, iCol, iEmptyRow, iEmptyCol);
				//해당 위치로 그림을 옮긴다.

				KillTimer(0);
				m_gameTurn = false;
				SetDlgItemText(IDC_STATIC_CONNECT, L"Another player's turn");
			}
		}
	}
	else if(isGameEnd()) {
		//한쪽이 게임종료되었을 경우
		CString str;
		SendData(S_gameEnd, str);
		//게임종료 신호를 보낸다.
	}
	else {
		SetDlgItemText(IDC_STATIC_CONNECT, _T("Not your turn"));
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMyDialog::NumToIndex(int iNum, int &iRow, int &iCol) {

	//받은 숫자를 맞는 인덱스로 변환
	int i, j;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			if (iNum == m_iGame[i][j]) {
				iRow = i;
				iCol = j;
				break;
			}
		}
	}

}

void CMyDialog::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	SetDlgItemText(IDC_EDIT1, _T(""));
	CString str;
	int len = m_strSend.GetLength() + 1;
	LPTSTR tmp = new TCHAR[len];
	_tcscpy_s(tmp, len, m_strSend);
//	m_strSend.Format(_T(""));

	str.Format(_T("%s"), tmp);

	int i = m_list.GetCount();
	m_list.InsertString(i, str);
	//전송한 내용도 리스트박스에 보여준다.

//	str.Format(_T("%s"), "hello");


	SendData(S_chatTxt, str);
}

//서버에 데이터를 보내는 함수입니다.
void CMyDialog::SendData(int type, CString strTmp) {

	//send data
	UpdateData(TRUE);
	char tmp[256];

	memset(tmp, '\0', 256);
	sprintf(tmp, "%d%s", type, strTmp);

	//strTmp가 정답인 경우 S_gameEnd메시지 전송
	if (strTmp == "The answer is 3") {
		
		SetDlgItemText(IDC_QUIZ, _T("Congratulation, You Win!"));
		strTmp.Format(_T("%d"), S_gameEnd);
		int len = strTmp.GetLength() + 1;
		LPTSTR stmp = new TCHAR[len];
		_tcscpy_s(stmp, len, strTmp);

		m_socCom.Send(stmp, 256);

		Sleep(1600);
		CDialogEx::OnOK();
		//다이얼로그 종료
	}
	else {
		/*-----------------------------------------*/
		//stmp배열에 type과 strTmp를 받아 복사해준다.
		/*-----------------------------------------*/
		strTmp.Format(_T("%d$%s"), type, strTmp);
		//strtmp 스트링에 type을 붙여준다.

		int len = strTmp.GetLength() + 1;
		LPTSTR stmp = new TCHAR[len];
		//	memset(stmp, '\0', 256);

		_tcscpy_s(stmp, len, strTmp);

		m_socCom.Send(stmp, 256);

		if (type == S_gameEnd) {
			Sleep(1000);
			CDialogEx::OnOK();
		}//다이얼로그 종료
	}
}

LPARAM CMyDialog::OnReceive(UINT wParam, LPARAM lParam)
{	
	char pTmp[256];
	CString strTmp, strType;
	int type;
	memset(pTmp, '\0', 256);

	// 데이터를 pTmp에 받는다.
	m_socCom.Receive(pTmp, 256);

	//헤더 분리
	strType.Format(_T("%c"), pTmp[0]);
	type = atoi((CStringA)strType);
	strTmp.Format(_T("%s"), pTmp);

	if (type == S_chatTxt) {
		//채팅문자열인 경우
		CString token;
		AfxExtractSubString(token, strTmp, 1, '$');
		int i = m_list.GetCount();
		m_list.InsertString(i, token);
		//받은 내용도 리스트박스에 보여준다.
	}
	else if (type == S_click) {

		//타이머 시작
		m_time = 20;
		SetTimer(0, 1000, NULL);

		//내 차례로 바꿔준다.
		m_gameTurn = true;
		SetDlgItemText(IDC_STATIC_CONNECT, L"Your turn");
		CString strIndex;
		int index;
		int iRow, iCol, iEmptyRow, iEmptyCol = -1;

		AfxExtractSubString(strIndex, strTmp, 1, '$');
		index = atoi((CStringA)strIndex);
		//인덱스 정수값으로 변환한다.
		if (index != -1) {
			NumToIndex(index, iRow, iCol);
			//받은 숫자에 알맞는 행,열 값 구한다
			FindEmptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
			//해당 행,열값에 알맞는 empty index 구한다
			MovePic(iRow, iCol, iEmptyRow, iEmptyCol);
			//해당 위치로 그림조각 옮기기
		}
	}
	else if (type == S_gameEnd) {
		//상대방이 먼저 게임을 끝낸경우
		SetDlgItemText(IDC_QUIZ, _T("Sorry, You Lose"));
		Sleep(1000);
		CDialogEx::OnOK();
	}
	else if (type == S_gameStart) {
		//게임이 시작되었다.
	}

	return LPARAM();
}

BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDialog::OnBnClickedButton2)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_WM_TIMER()
END_MESSAGE_MAP()



void CMyDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString clock = NULL;
	m_time--;
	clock.Format(_T("%02d"), m_time);
	SetDlgItemText(IDC_TIMER, clock);
	if (m_time == 0) {
		//시간 초과될경우 타이머 끝내고 상대방 차례로 넘어간다.
		KillTimer(0);
		m_gameTurn = false;
		SetDlgItemText(IDC_STATIC_CONNECT, L"Another player's turn");

		CString str; int i = -1;
		str.Format(L"%d", i);
		SendData(S_click, str);
		//데이터를 보낸다

	}
	UpdateData(false);
	CDialogEx::OnTimer(nIDEvent);
}
