// DlgObjParam.cpp : 实现文件
//

#include "stdafx.h"
#include "MapDeluxe.h"
#include "DlgObjParam.h"
#include "CodeTran.h"


// CDlgObjParam 对话框

IMPLEMENT_DYNAMIC(CDlgObjParam, CDialog)

CDlgObjParam::CDlgObjParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjParam::IDD, pParent)
{

}

CDlgObjParam::~CDlgObjParam()
{
}

void CDlgObjParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OBJ_P1, m_StaticP1);
	DDX_Control(pDX, IDC_STATIC_OBJ_P2, m_StaticP2);
	DDX_Control(pDX, IDC_COMBO_OBJ_CLASS, m_ComboClass);
	DDX_Control(pDX, IDC_EDIT_OBJ_P1, m_EditP1);
	DDX_Control(pDX, IDC_EDIT_OBJ_P2, m_EditP2);
	DDX_Control(pDX, IDC_EDIT_OBJ_P3, m_EditP3);
	DDX_Control(pDX, IDC_EDIT_OBJ_P4, m_EditP4);
	DDX_Control(pDX, IDC_CHECK_OBJ_FLOAT, m_CheckObjFloat);
}


BEGIN_MESSAGE_MAP(CDlgObjParam, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ_CLASS, &CDlgObjParam::OnCbnSelchangeComboObjClass)
	ON_BN_CLICKED(IDOK, &CDlgObjParam::OnBnClickedOk)
END_MESSAGE_MAP()



BOOL CDlgObjParam::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(addobj)SetWindowText(_T("新建物件"));
	CString str;
	for(u16 i=0;i<4096;i++)
	{
		if(*CodeTran_Obj((i>>8)|((i&0xFF)<<8))!='?')
		{
			str.Format(_T("[%02X-%02X]%s"),i>>8,i&0xFF,CodeTran_Obj((i>>8)|((i&0xFF)<<8)));
			m_ComboClass.AddString(str.GetBuffer());
			if(obj.class_id==((i>>8)|((i&0xFF)<<8)))m_ComboClass.SelectString(-1,str.GetBuffer());
		}
	}
	ResetParam();
	switch(obj.class_id&0xFF)
	{
	case 1:
		str.Format(_T("%d"),obj.paramA>>4);
		m_EditP1.SetWindowText(str.GetBuffer());
		str.Format(_T("%d"),obj.paramA&0xF);
		m_EditP2.SetWindowText(str.GetBuffer());
		str.Format(_T("%d"),obj.paramB>>5);
		m_EditP3.SetWindowText(str.GetBuffer());
		str.Format(_T("%d"),obj.paramB&0x1F);
		m_EditP4.SetWindowText(str.GetBuffer());
		break;
	case 3:
		break;
	case 4:
		str.Format(_T("%d"),obj.paramB);
		m_EditP1.SetWindowText(str.GetBuffer());
		m_CheckObjFloat.SetCheck(obj.paramA?TRUE:FALSE);
		break;
	case 5:case 8:
		str.Format(_T("%d"),obj.paramA);
		m_EditP1.SetWindowText(str.GetBuffer());
		break;
	}
	return TRUE;  
}
void CDlgObjParam::ResetParam()
{
	u32 ca,cb;
	CString str,stra,strb;
	m_ComboClass.GetWindowText(str);
	//stra=str.Mid(1,2);
	//stra=str.Mid(4,2);
	_stscanf_s(str.GetBuffer(),_T("[%X-%X]"),&ca,&cb);
	//_stscanf_s(stra.GetBuffer(),_T("%x"),&ca);
	m_EditP1.SetWindowText(_T("0"));
	m_EditP2.SetWindowText(_T("0"));
	m_EditP3.SetWindowText(_T("0"));
	m_EditP4.SetWindowText(_T("0"));
	m_CheckObjFloat.SetCheck(FALSE);
	switch(ca)
	{
	case 1:
		m_StaticP1.SetWindowText(_T("颜色编号(0~15)"));
		m_StaticP2.SetWindowText(_T("行为编号(0~15)"));
		m_EditP1.EnableWindow();
		m_EditP2.EnableWindow();
		m_EditP3.EnableWindow();
		m_EditP4.EnableWindow();
		m_CheckObjFloat.EnableWindow(FALSE);
		break;
	case 3:
		m_StaticP1.SetWindowText(_T(""));
		m_StaticP2.SetWindowText(_T(""));
		m_EditP1.EnableWindow(FALSE);
		m_EditP2.EnableWindow(FALSE);
		m_EditP3.EnableWindow(FALSE);
		m_EditP4.EnableWindow(FALSE);
		m_CheckObjFloat.EnableWindow(FALSE);
		break;
	case 4:
		m_StaticP1.SetWindowText(_T("未知值"));
		m_StaticP2.SetWindowText(_T(""));
		m_EditP1.EnableWindow();
		m_EditP2.EnableWindow(FALSE);
		m_EditP3.EnableWindow(FALSE);
		m_EditP4.EnableWindow(FALSE);
		m_CheckObjFloat.EnableWindow();
		break;
	case 5:case 8:
		m_StaticP1.SetWindowText(_T("编号"));
		m_StaticP2.SetWindowText(_T(""));
		m_EditP1.EnableWindow();
		m_EditP2.EnableWindow(FALSE);
		m_EditP3.EnableWindow(FALSE);
		m_EditP4.EnableWindow(FALSE);
		m_CheckObjFloat.EnableWindow(FALSE);
		break;
	}
}
void CDlgObjParam::OnCbnSelchangeComboObjClass()
{
	ResetParam();
}

void CDlgObjParam::OnBnClickedOk()
{
	u32 ca,cb;
	CString str;
	m_ComboClass.GetWindowText(str);
	_stscanf_s(str.GetBuffer(),_T("[%X-%X]"),&ca,&cb);
	obj.class_id=(u16)(ca|(cb<<8));
	switch(obj.class_id&0xFF)
	{
	case 1:
		m_EditP1.GetWindowText(str);
		_stscanf_s(str.GetBuffer(),_T("%d"),&ca);
		m_EditP2.GetWindowText(str);
		_stscanf_s(str.GetBuffer(),_T("%d"),&cb);
		obj.paramA=(u8)((ca<<4)|(cb&0xF));
		m_EditP3.GetWindowText(str);
		_stscanf_s(str.GetBuffer(),_T("%d"),&ca);
		m_EditP4.GetWindowText(str);
		_stscanf_s(str.GetBuffer(),_T("%d"),&cb);
		obj.paramB=(u8)((ca<<5)|(cb&0x1F));
		break;
	case 3:
		obj.paramA=obj.paramB=0;
		break;
	case 4:
		obj.paramA=m_CheckObjFloat.GetCheck()?1:0;
		m_EditP1.GetWindowText(str);
		_stscanf_s(str.GetBuffer(),_T("%d"),&ca);
		obj.paramB=(u8)ca;
		break;
	case 5:case 8:
		m_EditP1.GetWindowText(str);
		_stscanf_s(str.GetBuffer(),_T("%d"),&ca);
		obj.paramA=(u8)ca;
		obj.paramB=0;
		break;
	}
	OnOK();
}
