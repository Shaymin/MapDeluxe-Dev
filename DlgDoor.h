#pragma once
#include "afxcmn.h"
#include "MapDataType.h"

// CDlgDoor �Ի���

class CDlgDoor : public CDialog
{
	DECLARE_DYNAMIC(CDlgDoor)

public:
	CDlgDoor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDoor();

// �Ի�������
	enum { IDD = IDD_DOOR };

	u16 *pcount;
	DOOR_DATA ** ppdata;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListDoor;
	virtual BOOL OnInitDialog();
	void UpdateList();
	afx_msg void OnBnClickedDoorDelete();
	afx_msg void OnBnClickedDoorAdd();
};
