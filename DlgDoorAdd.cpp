//添加传送门窗口
//编写:wwylele


#include "stdafx.h"
#include "MapDeluxe.h"
#include "DlgDoorAdd.h"


// CDlgDoorAdd 对话框

IMPLEMENT_DYNAMIC(CDlgDoorAdd, CDialog)

CDlgDoorAdd::CDlgDoorAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDoorAdd::IDD, pParent)
{

}

CDlgDoorAdd::~CDlgDoorAdd()
{
}

void CDlgDoorAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DOORCLASS, m_ComboDoorClass);
	DDX_Control(pDX, IDC_STATIC_DOOR_P0, m_StaticDoorP0);
	DDX_Control(pDX, IDC_STATIC_DOOR_P1, m_StaticDoorP1);
	DDX_Control(pDX, IDC_STATIC_DOOR_P2, m_StaticDoorP2);
	DDX_Control(pDX, IDC_STATIC_DOOR_P3, m_StaticDoorP3);
	DDX_Control(pDX, IDC_STATIC_DOOR_P4, m_StaticDoorP4);
	DDX_Control(pDX, IDC_EDIT_DOOR_P0, m_EditDoorP0);
	DDX_Control(pDX, IDC_EDIT_DOOR_P1, m_EditDoorP1);
	DDX_Control(pDX, IDC_EDIT_DOOR_P2, m_EditDoorP2);
	DDX_Control(pDX, IDC_EDIT_DOOR_P3, m_EditDoorP3);
	DDX_Control(pDX, IDC_EDIT_DOOR_P4, m_EditDoorP4);
}


BEGIN_MESSAGE_MAP(CDlgDoorAdd, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DOORCLASS, &CDlgDoorAdd::OnCbnSelchangeComboDoorclass)
END_MESSAGE_MAP()

void CDlgDoorAdd::UpdateClass()
{
	//677*8可
	//678*8不可

	CString str;
	m_ComboDoorClass.GetWindowText(str);
	switch(m_ComboDoorClass.FindString(-1,str.GetBuffer()))
	{
	case 0:
		m_StaticDoorP0.SetWindowText(_T("目标Step"));
		m_StaticDoorP1.SetWindowText(_T("横坐标"));
		m_StaticDoorP2.SetWindowText(_T("纵坐标"));
		m_StaticDoorP3.SetWindowText(_T("目标横坐标（像素）"));
		m_StaticDoorP4.SetWindowText(_T("目标纵坐标（像素）"));
		m_EditDoorP0.EnableWindow(TRUE);
		m_EditDoorP1.EnableWindow(TRUE);
		m_EditDoorP2.EnableWindow(TRUE);
		m_EditDoorP3.EnableWindow(TRUE);
		m_EditDoorP4.EnableWindow(TRUE);
		m_EditDoorP0.SetWindowText(_T("0"));
		m_EditDoorP1.SetWindowText(_T("0"));
		m_EditDoorP2.SetWindowText(_T("0"));
		m_EditDoorP3.SetWindowText(_T("0"));
		m_EditDoorP4.SetWindowText(_T("0"));
		break;
	case 1:
		m_StaticDoorP0.SetWindowText(_T(""));
		m_StaticDoorP1.SetWindowText(_T("横坐标"));
		m_StaticDoorP2.SetWindowText(_T("纵坐标"));
		m_StaticDoorP3.SetWindowText(_T(""));
		m_StaticDoorP4.SetWindowText(_T(""));
		m_EditDoorP0.EnableWindow(FALSE);
		m_EditDoorP1.EnableWindow(TRUE);
		m_EditDoorP2.EnableWindow(TRUE);
		m_EditDoorP3.EnableWindow(FALSE);
		m_EditDoorP4.EnableWindow(FALSE);
		m_EditDoorP0.SetWindowText(_T("8888"));
		m_EditDoorP1.SetWindowText(_T("0"));
		m_EditDoorP2.SetWindowText(_T("0"));
		m_EditDoorP3.SetWindowText(_T("0"));
		m_EditDoorP4.SetWindowText(_T("0"));
		break;
	case 2:
		m_StaticDoorP0.SetWindowText(_T(""));
		m_StaticDoorP1.SetWindowText(_T("目标Step"));
		m_StaticDoorP2.SetWindowText(_T(""));
		m_StaticDoorP3.SetWindowText(_T("1"));
		m_StaticDoorP4.SetWindowText(_T("2"));
		m_EditDoorP0.EnableWindow(FALSE);
		m_EditDoorP1.EnableWindow(TRUE);
		m_EditDoorP2.EnableWindow(FALSE);
		m_EditDoorP3.EnableWindow(TRUE);
		m_EditDoorP4.EnableWindow(TRUE);
		m_EditDoorP0.SetWindowText(_T("7777"));
		m_EditDoorP1.SetWindowText(_T("0"));
		m_EditDoorP2.SetWindowText(_T("0"));
		m_EditDoorP3.SetWindowText(_T("0"));
		m_EditDoorP4.SetWindowText(_T("0"));
		break;
	case 3:
		m_StaticDoorP0.SetWindowText(_T(""));
		m_StaticDoorP1.SetWindowText(_T("目标Step"));
		m_StaticDoorP2.SetWindowText(_T("1"));
		m_StaticDoorP3.SetWindowText(_T("2"));
		m_StaticDoorP4.SetWindowText(_T("3"));
		m_EditDoorP0.EnableWindow(FALSE);
		m_EditDoorP1.EnableWindow(TRUE);
		m_EditDoorP2.EnableWindow(TRUE);
		m_EditDoorP3.EnableWindow(TRUE);
		m_EditDoorP4.EnableWindow(TRUE);
		m_EditDoorP0.SetWindowText(_T("6666"));
		m_EditDoorP1.SetWindowText(_T("0"));
		m_EditDoorP2.SetWindowText(_T("0"));
		m_EditDoorP3.SetWindowText(_T("0"));
		m_EditDoorP4.SetWindowText(_T("0"));
		break;
	
	case 4:
		m_StaticDoorP0.SetWindowText(_T(""));
		m_StaticDoorP1.SetWindowText(_T(""));
		m_StaticDoorP2.SetWindowText(_T("x"));
		m_StaticDoorP3.SetWindowText(_T(""));
		m_StaticDoorP4.SetWindowText(_T(""));
		m_EditDoorP0.EnableWindow(FALSE);
		m_EditDoorP1.EnableWindow(FALSE);
		m_EditDoorP2.EnableWindow(TRUE);
		m_EditDoorP3.EnableWindow(FALSE);
		m_EditDoorP4.EnableWindow(FALSE);
		m_EditDoorP0.SetWindowText(_T("6666"));
		m_EditDoorP1.SetWindowText(_T("65535"));
		m_EditDoorP2.SetWindowText(_T("0"));
		m_EditDoorP3.SetWindowText(_T("0"));
		m_EditDoorP4.SetWindowText(_T("0"));
		break;
	
	case 5:
		m_StaticDoorP0.SetWindowText(_T(""));
		m_StaticDoorP1.SetWindowText(_T("横坐标"));
		m_StaticDoorP2.SetWindowText(_T("纵坐标"));
		m_StaticDoorP3.SetWindowText(_T(""));
		m_StaticDoorP4.SetWindowText(_T(""));
		m_EditDoorP0.EnableWindow(FALSE);
		m_EditDoorP1.EnableWindow(TRUE);
		m_EditDoorP2.EnableWindow(TRUE);
		m_EditDoorP3.EnableWindow(FALSE);
		m_EditDoorP4.EnableWindow(FALSE);
		m_EditDoorP0.SetWindowText(_T("5555"));
		m_EditDoorP1.SetWindowText(_T("0"));
		m_EditDoorP2.SetWindowText(_T("0"));
		m_EditDoorP3.SetWindowText(_T("0"));
		m_EditDoorP4.SetWindowText(_T("0"));
		break;
	}
}

BOOL CDlgDoorAdd::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ComboDoorClass.AddString(_T("传送门"));
	m_ComboDoorClass.AddString(_T("终点"));
	m_ComboDoorClass.AddString(_T("特殊传送"));
	m_ComboDoorClass.AddString(_T("特殊传送转接"));
	m_ComboDoorClass.AddString(_T("特殊传送终止"));
	m_ComboDoorClass.AddString(_T("返回点"));
	m_ComboDoorClass.SelectString(0,_T("传送门"));
	UpdateClass();

	return TRUE; 
}


void CDlgDoorAdd::OnCbnSelchangeComboDoorclass()
{
	UpdateClass();
}

void CDlgDoorAdd::OnOK()
{
	CString str;
	m_EditDoorP0.GetWindowText(str);
	newdoor.door.to_step=_ttoi(str.GetBuffer());
	m_EditDoorP1.GetWindowText(str);
	newdoor.door.x=_ttoi(str.GetBuffer());
	m_EditDoorP2.GetWindowText(str);
	newdoor.door.y=_ttoi(str.GetBuffer());
	m_EditDoorP3.GetWindowText(str);
	newdoor.door.to_x=_ttoi(str.GetBuffer());
	m_EditDoorP4.GetWindowText(str);
	newdoor.door.to_y=_ttoi(str.GetBuffer());
	newdoor.door.unk=0;
	CDialog::OnOK();
}
