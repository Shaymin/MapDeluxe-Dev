#pragma once
#include "MapDataType.h"
#include "afxwin.h"

// CDlgObjParam 对话框

class CDlgObjParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjParam)

public:
	CDlgObjParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgObjParam();

// 对话框数据
	enum { IDD = IDD_OBJ_PARAM };
	bool addobj;
	OBJ_DATA obj;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_StaticP1;
	CStatic m_StaticP2;
	CComboBox m_ComboClass;
	CEdit m_EditP1;
	CEdit m_EditP2;
	CEdit m_EditP3;
	CEdit m_EditP4;
	virtual BOOL OnInitDialog();
	void ResetParam();
	CButton m_CheckObjFloat;
	afx_msg void OnCbnSelchangeComboObjClass();
	afx_msg void OnBnClickedOk();
};
