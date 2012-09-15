#pragma once
#include "afxcmn.h"
#include "MapDataType.h"

// CDlgDoor 对话框

class CDlgDoor : public CDialog
{
	DECLARE_DYNAMIC(CDlgDoor)

public:
	CDlgDoor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDoor();

// 对话框数据
	enum { IDD = IDD_DOOR };

	u16 *pcount;
	DOOR_DATA ** ppdata;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListDoor;
	virtual BOOL OnInitDialog();
	void UpdateList();
	afx_msg void OnBnClickedDoorDelete();
	afx_msg void OnBnClickedDoorAdd();
	CMenu m_MenuXY;
};
