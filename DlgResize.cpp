//修改尺寸窗口
//编写:wwylele

#include "stdafx.h"
#include "MapDeluxe.h"
#include "DlgResize.h"


// CDlgResize 对话框

IMPLEMENT_DYNAMIC(CDlgResize, CDialog)

CDlgResize::CDlgResize(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResize::IDD, pParent)
{

}

CDlgResize::~CDlgResize()
{
}

void CDlgResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_EditWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_EditHeight);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_StaticSize);
}


BEGIN_MESSAGE_MAP(CDlgResize, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgResize::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgResize 消息处理程序

BOOL CDlgResize::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckRadioButton(0,65535,IDC_RADIO_LEFT_TOP);
	CString str;
	str.Format(_T("当前尺寸:%d×%d"),width,height);
	m_StaticSize.SetWindowText(str.GetBuffer());

	twidth=(s16)width;
	theight=(s16)height;

	return TRUE;  
}

void CDlgResize::OnBnClickedOk()
{
	CString str;
	m_EditWidth.GetWindowText(str);
	width=_ttoi(str.GetBuffer());
	m_EditHeight.GetWindowText(str);
	height=_ttoi(str.GetBuffer());
	if(width>1000 ||width<5 ||height>1000||height<5)
	{
		MessageBox(_T("请输入5~1000之间的整数作为尺寸！"));
		return;
	}
	if(width*height>5400)
	{
		MessageBox(_T("请将总格子数保持在5400以下！否则将导致游戏运行时内存溢出！\n")
			_T("*注:5400一数为目前测试数值，实际上限可能比这个还要小。\n")
			_T("***:氧气已用完，一秒后超度一生。")
			);
		return;
	}
	switch(this->GetCheckedRadioButton(0,65535))
	{
	case IDC_RADIO_LEFT_TOP:dx=dy=0;break;//left=true;top=true;break;
	case IDC_RADIO_RIGHT_TOP:dx=twidth-width;dy=0;break;//left=false;top=true;break;
	case IDC_RADIO_LEFT_BUTTON:dx=0;dy=theight-height;break;//left=true;top=false;break;
	case IDC_RADIO_RIGHT_BUTTON:dx=twidth-width;dy=theight-height;break;//left=false;top=false;break;
	}
	OnOK();
}
