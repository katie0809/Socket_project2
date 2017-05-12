#pragma once
#include "SocCom.h"
// CSocServer 명령 대상입니다.

class CSocServer : public CSocket
{
public:
	CSocServer();
	virtual ~CSocServer();
	virtual void OnAccept(int nErrorCode);

	void Init(HWND hWnd);
	HWND m_hWnd; // 메인 윈도우 핸들
	CSocCom m_socCom; // 접속 요청이 왔을때 연결한 소켓

	CSocCom * GetAcceptSocCom()
	{
		// 통신소켓을 리턴한다.
		// 반환되는 통신 소켓은 클라이언트와 연결된다.
		return &m_socCom;
	}
};


