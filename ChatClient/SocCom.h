#pragma once

// CSocCom ��� ����Դϴ�.

#ifndef _SOCCOM_H
#define _SOCCOM_H

#include <afxsock.h>
class CSocCom : public CSocket
{
public:
	CSocCom();
	virtual ~CSocCom();
	HWND m_hWnd;
	void Init(HWND hWnd);
	virtual void OnReceive(int nErrorCode);
};


#endif //_SOCCOM_H