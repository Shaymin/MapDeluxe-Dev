#pragma once
#include "afxwin.h"
#include "MapDataType.h"

// CDlgDoorAdd 对话框

class CDlgDoorAdd : public CDialog
{
	DECLARE_DYNAMIC(CDlgDoorAdd)

public:
	CDlgDoorAdd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDoorAdd();

// 对话框数据
	enum { IDD = IDD_DOOR_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboDoorClass;
	CStatic m_StaticDoorP0;
	CStatic m_StaticDoorP1;
	CStatic m_StaticDoorP2;
	CStatic m_StaticDoorP3;
	CStatic m_StaticDoorP4;
	CEdit m_EditDoorP0;
	CEdit m_EditDoorP1;
	CEdit m_EditDoorP2;
	CEdit m_EditDoorP3;
	CEdit m_EditDoorP4;
	CMenu* pdxymenu;
	virtual BOOL OnInitDialog();
	void UpdateClass();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeComboDoorclass();
	afx_msg void OnXYMenu(UINT id);
	DOOR_DATA newdoor;
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonDoorXy();
	CButton m_ButtonXY;
};
