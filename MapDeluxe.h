

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define STEP_INDEXING_COUNT 292
#define DOOR_XY_MENU_ID_BASE 3001


class CMapDeluxeApp : public CWinApp
{
public:
	CMapDeluxeApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMapDeluxeApp theApp;

CString GetProductVersion()  ;