

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define STEP_INDEXING_COUNT 292
#define DOOR_XY_MENU_ID_BASE 3001


class CMapDeluxeApp : public CWinApp
{
public:
	CMapDeluxeApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMapDeluxeApp theApp;

CString GetProductVersion()  ;