#pragma once
#include "afxwin.h"


// CDlgResize �Ի���

class CDlgResize : public CDialog
{
	DECLARE_DYNAMIC(CDlgResize)

public:
	CDlgResize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResize();

// �Ի�������
	enum { IDD = IDD_RESIZE };

	s16 twidth,theight;
	u16 width,height;
	//bool left,top;
	s16 dx,dy;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditWidth;
	CEdit m_EditHeight;
	CStatic m_StaticSize;
	afx_msg void OnBnClickedOk();
};
