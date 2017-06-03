

#include "OmokWnd.h"
#include "OmokDlg.h"

BEGIN_MESSAGE_MAP(OmokWnd,CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

OmokWnd::OmokWnd() {
	this->x = -1;
	this->y = -1;
	this->turn = 0;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			getDoll[i][j] = 0;
		}
	}
	this->whoWin =0;
}

void OmokWnd::OnLButtonUp(UINT nFlags, CPoint point) {
	this->x = point.x;
	this->y = point.y;
	if (x >= 50 && x <= 625 && y >= 50 && y <= 625) {
		
		OmokInner();
	}

	this->Invalidate();

}

void OmokWnd::Reset() {

	this->x = -1;
	this->y = -1;
	this->turn = 0;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			getDoll[i][j] = 0;
		}
	}
	this->whoWin = 0;

}

void OmokWnd::OnPaint() {
	PAINTSTRUCT ps;
	CDC *pDC = BeginPaint(&ps);
	char getNumber[100];
	CBrush brush, brush2, brush3;
	CBrush *oldBrush;
	
	
	
	//바둑판 그리기
	brush.CreateSolidBrush(RGB(240,160,60));
	brush2.CreateSolidBrush(RGB(0, 0, 0));
	brush3.CreateSolidBrush(RGB(255, 255, 255));
	oldBrush=pDC->SelectObject(&brush);
	pDC->Rectangle(50, 50, 650, 650);
	//바둑판 선그리기
	for (int i = 0; i <= 12; i++) {
		pDC->MoveTo(50 + 50 * i, 50);
		pDC->LineTo(50 + 50 * i, 650);
	}
	for (int i = 0; i <= 12; i++) {
		pDC->MoveTo(50, 50 + 50 * i);
		pDC->LineTo(650, 50 + 50 * i);
	}

	
	// 임시 확인용 좌표
	sprintf_s(getNumber, "(%d, %d)", x,y);
	pDC->TextOut(10, 0,getNumber );
	sprintf_s(getNumber, "%d번째",turn);
	pDC->TextOut(100, 0, getNumber);


	// 바둑알
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (getDoll[i][j] == 1) {
				pDC->SelectObject(&brush2);
				pDC->Ellipse(i * 50 + 75, j * 50 + 75, i * 50 + 125, j * 50 + 125);
				}
			else if (getDoll[i][j] == 2) {
				pDC->SelectObject(&brush3);
				pDC->Ellipse(i * 50 + 75, j * 50 + 75, i * 50 + 125, j * 50 + 125);
			}
		}
	}
	int result;
	if (whoWin != 0) {
		if (whoWin == 1) {
			pDC->TextOut(400, 0, "흑 승리");
			AfxMessageBox(_T("흑 승리입니다."), MB_OK);

		}
		else if (whoWin == 2) {
			pDC->TextOut(400, 0, "백 승리");
			AfxMessageBox(_T("백 승리입니다."), MB_OK);

		}
	}
	


	//마무리 정리
	pDC->SelectObject(oldBrush);

	brush.DeleteObject();
	brush2.DeleteObject();
	brush3.DeleteObject();
	EndPaint(&ps);
}

void OmokWnd::OmokInner() {
	int tempX, tempY;
	
	if (getDoll[((x - 75) / 50)][((y - 75) / 50)] == 0) {
		turn++;
		tempX = ((x - 75) / 50);
		tempY = ((y - 75) / 50);


		// 흑:1  백:2  빈공간:0
		if (turn % 2 == 1) { //흑
			getDoll[tempX][tempY] = 1;
			this->sendState = 1;
		}
		else {//백
			getDoll[tempX][tempY] = 2;
			this->sendState = 2;
		}

		CString temp;
		temp.Format(L"%d~%d~%d", this->x, this->y, this->sendState);

		this->parent->SendData(0, temp);

		// 게임이 끝났는지 확인
		
		// 가로세로확인
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 7; j++) {
				if ((getDoll[i][j] == 1 && getDoll[i][j + 1] == 1 && getDoll[i][j + 2] == 1 && getDoll[i][j + 3] == 1 && getDoll[i][j + 4] == 1 )||
					(getDoll[j][i] == 1 && getDoll[j + 1][i] == 1 && getDoll[j + 2][i] == 1 && getDoll[j + 3][i] == 1 && getDoll[j + 4][i] == 1 )) {
					whoWin = 1;
				}
				if ((getDoll[i][j] == 2 && getDoll[i][j + 1] == 2 && getDoll[i][j + 2] == 2 && getDoll[i][j + 3] == 2 && getDoll[i][j + 4] == 2 ) ||
					(getDoll[j][i] == 2 && getDoll[j + 1][i] == 2 && getDoll[j + 2][i] == 2 && getDoll[j + 3][i] == 2 && getDoll[j + 4][i] == 2 )) {
					whoWin = 2;
				}
			}
		}
		//대각선 확인
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++) {
				if ((getDoll[i][j] == 1 && getDoll[i + 1][j + 1] == 1 && getDoll[i + 2][j + 2] == 1 && getDoll[i + 3][j + 3] == 1 && getDoll[i + 4][j + 4] == 1) ||
					(getDoll[j + 4][i] == 1 && getDoll[j + 3][i+1] == 1 && getDoll[j + 2][i+2] == 1 && getDoll[j + 1][i+3] == 1 && getDoll[j][i+4] == 1)) {
					whoWin = 1;
				}
				if ((getDoll[i][j] == 2 && getDoll[i + 1][j + 1] == 2 && getDoll[i + 2][j + 2] == 2 && getDoll[i + 3][j + 3] == 2 && getDoll[i + 4][j + 4] == 2) ||
					(getDoll[j + 4][i] == 2 && getDoll[j + 3][i + 1] == 2 && getDoll[j + 2][i + 2] == 2 && getDoll[j + 1][i + 3] == 2 && getDoll[j][i + 4] == 2)) {
					whoWin = 2;
				}
			}
		}



	}
	

}

int OmokWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	this->parent = (OmokDlg*)CWnd::FindWindow(_T("#32770"), _T("오목 클라이언트"));
	return 0;
}

void OmokWnd::OnClose() {
	CWnd::OnClose();
}
