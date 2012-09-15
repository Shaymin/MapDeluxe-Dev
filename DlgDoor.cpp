//传送门窗口
//编写:wwylele

#include "stdafx.h"
#include "MapDeluxe.h"
#include "DlgDoor.h"
#include "DlgDoorAdd.h"


// CDlgDoor 对话框

IMPLEMENT_DYNAMIC(CDlgDoor, CDialog)

CDlgDoor::CDlgDoor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDoor::IDD, pParent)
{
	m_MenuXY.CreatePopupMenu();//构造时立即创建以使窗口调用者能够往里面灌入选项
}

CDlgDoor::~CDlgDoor()
{
}

void CDlgDoor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DOOR, m_ListDoor);
}


BEGIN_MESSAGE_MAP(CDlgDoor, CDialog)
	ON_BN_CLICKED(IDC_DOOR_DELETE, &CDlgDoor::OnBnClickedDoorDelete)
	ON_BN_CLICKED(IDC_DOOR_ADD, &CDlgDoor::OnBnClickedDoorAdd)
END_MESSAGE_MAP()


void CDlgDoor::UpdateList()
{
	m_ListDoor.DeleteAllItems();
	LVITEM lvi={0};
	lvi.mask=LVIF_TEXT;
	CString str;
	for(u16 i=0;i<*pcount;i++)
	{
		lvi.iItem=i;
		if((*ppdata)[i].door.to_step==8888)
		{
			lvi.pszText=_T("终点");
			m_ListDoor.InsertItem(&lvi);
			str.Format(_T("(%d,%d)"),(*ppdata)[i].goal.x,(*ppdata)[i].goal.y);
			m_ListDoor.SetItem(i,1,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
		}
		else if((*ppdata)[i].door.to_step==7777)
		{
			lvi.pszText=_T("特殊传送");
			m_ListDoor.InsertItem(&lvi);
			str.Format(_T("传送至Step%02d,b=%d,c=%d"),(*ppdata)[i].spc.to_step,
				(*ppdata)[i].spc.x2,(*ppdata)[i].spc.x3);
			m_ListDoor.SetItem(i,2,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
		}
		else if((*ppdata)[i].door.to_step==6666)
		{
			lvi.pszText=_T("特殊传送接收");
			m_ListDoor.InsertItem(&lvi);
			if((*ppdata)[i].spc_rec.to_step==0xFFFF)
			{
				str.Format(_T("终止点,a=%d,b=%d,c=%d"),(*ppdata)[i].spc_rec.x1,
					(*ppdata)[i].spc_rec.x2,(*ppdata)[i].spc_rec.x3);
			}
			else
			{
				str.Format(_T("传送至Step%02d,a=%d,b=%d,c=%d"),(*ppdata)[i].spc_rec.to_step,
					(*ppdata)[i].spc_rec.x1,(*ppdata)[i].spc_rec.x2,(*ppdata)[i].spc_rec.x3);
			}
			m_ListDoor.SetItem(i,2,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
		}
		else if((*ppdata)[i].door.to_step==5555)
		{
			lvi.pszText=_T("返回点");
			m_ListDoor.InsertItem(&lvi);
			str.Format(_T("(%d,%d)"),(*ppdata)[i].ret.x,(*ppdata)[i].ret.y);
			m_ListDoor.SetItem(i,1,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
		}
		else
		{
			lvi.pszText=_T("传送门");
			m_ListDoor.InsertItem(&lvi);
			str.Format(_T("(%d,%d)"),(*ppdata)[i].door.x,(*ppdata)[i].door.y);
			m_ListDoor.SetItem(i,1,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
			str.Format(_T("传送至Step%02d(%d,%d)"),(*ppdata)[i].door.to_step,
				(*ppdata)[i].door.to_x,(*ppdata)[i].door.to_y);
			m_ListDoor.SetItem(i,2,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
		}
	}
}
BOOL CDlgDoor::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ListDoor.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_ListDoor.InsertColumn(0,_T("类别"),0,96);
	m_ListDoor.InsertColumn(1,_T("坐标"),0,96);
	m_ListDoor.InsertColumn(2,_T("其它参数"),0,192);
	UpdateList();
	return TRUE;  
}

void CDlgDoor::OnBnClickedDoorDelete()
{
	int curi;
	for(curi=0;curi<m_ListDoor.GetItemCount();curi++)
	{
		if(m_ListDoor.GetItemState(curi,LVIS_SELECTED))break;
	}if(curi==m_ListDoor.GetItemCount())return;
	if(curi>=*pcount)return;
	m_ListDoor.DeleteItem(curi);
	DOOR_DATA *newdata=0;
	(*pcount)--;
	if(*pcount>0)
	{
		newdata=new DOOR_DATA[*pcount];
		if(curi)memcpy(newdata,*ppdata,curi*sizeof(DOOR_DATA));
		if(curi<*pcount)memcpy(newdata+curi,*ppdata+curi+1,(*pcount-curi)*sizeof(DOOR_DATA));
	}
	delete[] *ppdata;

	*ppdata=newdata;
}

void CDlgDoor::OnBnClickedDoorAdd()
{
	if(*pcount>69)
	{
		MessageBox(_T("已达到传送门数量上限(70)！"));
		return;
	}
	CDlgDoorAdd dlg;
	dlg.pdxymenu=&m_MenuXY;
	if(dlg.DoModal()==IDCANCEL)return;
	(*pcount)++;
	DOOR_DATA *newdata=new DOOR_DATA[*pcount];
	if(*pcount>1)
	{
		memcpy(newdata,*ppdata,(*pcount-1)*sizeof(DOOR_DATA));
		delete[] *ppdata;
	}
	newdata[*pcount-1]=dlg.newdoor;
	*ppdata=newdata;
	UpdateList();
}
