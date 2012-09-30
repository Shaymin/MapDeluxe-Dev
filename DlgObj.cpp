// DlgObj.cpp : 实现文件
//

#include "stdafx.h"
#include "MapDeluxe.h"
#include "DlgObj.h"
#include "DlgObjParam.h"
#include "CodeTran.h"


// CDlgObj 对话框

IMPLEMENT_DYNAMIC(CDlgObj, CDialog)

CDlgObj::CDlgObj(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObj::IDD, pParent)
{

}

CDlgObj::~CDlgObj()
{
}

void CDlgObj::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_HSCROLL, m_HScroll);
	DDX_Control(pDX, IDC_OBJ_VSCROLL, m_VScroll);
	DDX_Control(pDX, IDC_CHECK_GRID_ALIGN, m_CheckGridAlign);
	DDX_Control(pDX, IDC_CHECK_OBJ_VERTICAL, m_CheckObjVertical);
}


BEGIN_MESSAGE_MAP(CDlgObj, CDialog)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



BOOL CDlgObj::OnInitDialog()
{
	CDialog::OnInitDialog();


	CDC* pDC=GetDC();
	
	moving=false;
	copymov=false;
	
	cur_obj=0xFFFF;
	cur_subobj=0xFFFF;

	m_CheckObjVertical.SetCheck(*pobj_vertical?TRUE:FALSE);


	//设置滚动条
	RECT hsrc,vsrc;
	int hsrg,vsrg;
	m_HScroll.GetWindowRect(&hsrc);
	m_VScroll.GetWindowRect(&vsrc);
	hsrg=width-(hsrc.right-hsrc.left);
	if(hsrg<1)hsrg=1;
	vsrg=height-(vsrc.bottom-vsrc.top);
	if(vsrg<1)vsrg=1;
	SCROLLINFO scrinfo={0};
	scrinfo.cbSize=sizeof(SCROLLINFO);
	scrinfo.fMask=SIF_PAGE|SIF_RANGE|SIF_POS;
	scrinfo.nPage=hsrc.right-hsrc.left;
	scrinfo.nMin=0;scrinfo.nMax=hsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_HScroll.SetScrollInfo(&scrinfo);
	scrinfo.nPage=vsrc.bottom-vsrc.top;
	scrinfo.nMin=0;scrinfo.nMax=vsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_VScroll.SetScrollInfo(&scrinfo);

	//PresentObj(pDC);
	ReleaseDC(pDC);

	return TRUE;
}
void CDlgObj::DrawObj(u16 x,u16 y)
{
	m_pTmpDC->MoveTo(x-8,y);
	m_pTmpDC->LineTo(x+8,y);
	m_pTmpDC->MoveTo(x,y-8);
	m_pTmpDC->LineTo(x,y+8);
	m_pTmpDC->Ellipse(x-4,y-4,x+5,y+5);
}
void CDlgObj::PresentObj(CDC* pDC)
{
	CString str;

	m_pTmpDC->SelectObject(&m_BmpOut);
	m_pTmpDC2->SelectObject(&m_BmpBck);
	m_pTmpDC->BitBlt(0,0,width,height,m_pTmpDC2,0,0,SRCCOPY);
	static CPen PenCur(PS_SOLID,1,RGB(255,0,0));
	CPen PenObj[9];
	PenObj[1].CreatePen(PS_SOLID,3,RGB(255,0,0));//敌人
	PenObj[3].CreatePen(PS_SOLID,3,RGB(255,0,255));//BOSS
	PenObj[4].CreatePen(PS_SOLID,3,RGB(0,0,255));//道具
	PenObj[5].CreatePen(PS_SOLID,3,RGB(255,128,0));//特殊物件
	PenObj[6].CreatePen(PS_SOLID,3,RGB(0,255,0));
	PenObj[7].CreatePen(PS_SOLID,3,RGB(0,255,128));
	PenObj[8].CreatePen(PS_SOLID,3,RGB(128,0,255));//梅塔军队

	PenObj[0].CreatePen(PS_SOLID,3,RGB(0,0,0));
	PenObj[2].CreatePen(PS_SOLID,3,RGB(255,255,0));//高亮
	CBrush BrushNull;
	BrushNull.CreateStockObject(NULL_BRUSH);
	m_pTmpDC->SelectObject(&BrushNull);
	OBJ_DATA odt;
	for(u16 i=0;i<*pcount;i++)
	{
		if((*pobjlist)[i].is_folder==0)
		{
			odt=(*pobjlist)[i].o[0];
			m_pTmpDC->SelectObject(&PenObj[odt.class_id&0xFF]);
			if(i==cur_obj)m_pTmpDC->SelectObject(&PenObj[2]);
			DrawObj(odt.x,odt.y);
		}
		else
		{
			int folderpen=0;
			if(i==cur_obj && cur_subobj==0xFFFF)folderpen=2;
			m_pTmpDC->SelectObject(&PenObj[folderpen]);
			DrawObj((*pobjlist)[i].fx,(*pobjlist)[i].fy);
			for(u16 j=0;j<(*pobjlist)[i].len;j++)
			{
				odt=(*pobjlist)[i].o[j];
				m_pTmpDC->SelectObject(&PenObj[folderpen]);
				m_pTmpDC->MoveTo((*pobjlist)[i].fx,(*pobjlist)[i].fy);
				m_pTmpDC->LineTo(odt.x,odt.y);
				m_pTmpDC->SelectObject(&PenObj[odt.class_id&0xFF]);
				if(i==cur_obj && cur_subobj==j)m_pTmpDC->SelectObject(&PenObj[2]);
				DrawObj(odt.x,odt.y);
			}
		}
	}
	m_pTmpDC->SetBkMode(TRANSPARENT);
	CFont   MyFont; 
	MyFont.CreateFont( 
		12,6,
        0,0,FW_BOLD, 
        0,0,0, 
        DEFAULT_CHARSET, 
		OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_DONTCARE, 
		_T("宋体")); 
	m_pTmpDC->SelectObject(MyFont); 
	for(u16 i=0;i<*pcount;i++)
	{
		if((*pobjlist)[i].is_folder==0)
		{
			str.Format(_T("%s"),CodeTran_Obj((*pobjlist)[i].o[0].class_id));
			m_pTmpDC->TextOut((*pobjlist)[i].o[0].x,(*pobjlist)[i].o[0].y,str.GetBuffer(),str.GetLength());
		}
	}
	
	if(x!=0xFFFF)
	{
		m_pTmpDC->SelectObject(&BrushNull);
		m_pTmpDC->SelectObject(&PenCur);
		m_pTmpDC->Rectangle((x/16)*16,(y/16)*16,(x/16)*16+16,(y/16)*16+16);
		
	}
	
	RECT hsrc,vsrc;
	int hsp=m_HScroll.GetScrollPos();
	int vsp=m_VScroll.GetScrollPos();
	m_HScroll.GetWindowRect(&hsrc);
	m_VScroll.GetWindowRect(&vsrc);
	ScreenToClient(&hsrc);
	ScreenToClient(&vsrc);
	pDC->BitBlt(hsrc.left,vsrc.top,
		hsrc.right-hsrc.left,vsrc.bottom-vsrc.top,m_pTmpDC,
		hsp,vsp,SRCCOPY);
}
void CDlgObj::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos;
   int maxpos;
   pScrollBar->GetScrollRange(&minpos, &maxpos); 
   maxpos = pScrollBar->GetScrollLimit();
   int curpos = pScrollBar->GetScrollPos();
   switch (nSBCode)
   {
   case SB_LEFT:curpos = minpos;break;
   case SB_RIGHT:curpos = maxpos;break;
   case SB_LINELEFT:
      if (curpos > minpos)
         curpos--;
      break;
   case SB_LINERIGHT:
      if (curpos < maxpos)
         curpos++;
      break;
   case SB_PAGELEFT:
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;
   case SB_PAGERIGHT:
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;
   case SB_THUMBPOSITION: 
      curpos = nPos;     
      break;
   case SB_THUMBTRACK:   
      curpos = nPos;     
      break;
   }
   pScrollBar->SetScrollPos(curpos);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	CDC* pDC=GetDC();
	PresentObj(pDC);
	ReleaseDC(pDC);
}

void CDlgObj::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos;
   int maxpos;
   pScrollBar->GetScrollRange(&minpos, &maxpos); 
   maxpos = pScrollBar->GetScrollLimit();
   int curpos = pScrollBar->GetScrollPos();
   switch (nSBCode)
   {
   case SB_TOP:      
      curpos = minpos;
      break;
   case SB_BOTTOM:    
      curpos = maxpos;
      break;
   case SB_LINEUP:     
      if (curpos > minpos)
         curpos--;
      break;
   case SB_LINEDOWN:
      if (curpos < maxpos)
         curpos++;
      break;
   case SB_PAGEUP: 
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;
   case SB_PAGEDOWN: 
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;
   case SB_THUMBPOSITION: 
      curpos = nPos;
      break;
   case SB_THUMBTRACK:  
      curpos = nPos;
      break;
   }
   pScrollBar->SetScrollPos(curpos);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

	CDC* pDC=GetDC();
	PresentObj(pDC);
	ReleaseDC(pDC);
}
void CDlgObj::OnPaint()
{
	CPaintDC dc(this); 
	PresentObj(&dc);
}

void CDlgObj::OnMouseMove(UINT nFlags, CPoint point)
{
	RECT hsrc,vsrc;
	int hsp=m_HScroll.GetScrollPos();
	int vsp=m_VScroll.GetScrollPos();
	m_HScroll.GetWindowRect(&hsrc);
	m_VScroll.GetWindowRect(&vsrc);
	ScreenToClient(&hsrc);
	ScreenToClient(&vsrc);

	x=(int)(point.x+hsp-hsrc.left);
	y=(int)(point.y+vsp-vsrc.top);
	if(moving && cur_obj!=0xFFFF)
	{
		x-=mdx;
		y-=mdy;
		if(x<0)x=0;
		if((u16)x>=width)x=width-1;
		if(y<0)y=0;
		if((u16)y>=height)y=height-1;
		if(m_CheckGridAlign.GetCheck())
		{
			x=(x+4)/8*8;
			y=(y+4)/8*8;
		}
		if(cur_subobj!=0xFFFF)
		{
			(*pobjlist)[cur_obj].o[cur_subobj].x=(u16)(x);
			(*pobjlist)[cur_obj].o[cur_subobj].y=(u16)(y);
		}
		else
		{
			(*pobjlist)[cur_obj].fx=(u16)(x-mdx);
			(*pobjlist)[cur_obj].fy=(u16)(y-mdy);
		}
		goto fin;
	}

	if(point.x<hsrc.right && point.x>hsrc.left &&
		point.y<vsrc.bottom && point.y>vsrc.top)
	{
		
		for(u16 i=0;i<*pcount;i++)
		{
			if((*pobjlist)[i].is_folder==0)
			{
				mdx=x-(*pobjlist)[i].o[0].x;
				mdy=y-(*pobjlist)[i].o[0].y;
				if(abs(mdx)<9 && abs(mdy)<9)
				{
					cur_obj=i;
					cur_subobj=0;
					goto fin;
				}
			}
			else
			{
				mdx=x-(*pobjlist)[i].fx;
				mdy=y-(*pobjlist)[i].fy;
				if(abs(mdx)<9 && abs(mdy)<9)
				{
					cur_obj=i;
					cur_subobj=0xFFFF;
					goto fin;
				}
				for(u16 j=0;j<(*pobjlist)[i].len;j++)
				{
					mdx=x-(*pobjlist)[i].o[j].x;
					mdy=y-(*pobjlist)[i].o[j].y;
					if(abs(mdx)<9 && abs(mdy)<9)
					{
						cur_obj=i;
						cur_subobj=j;
						goto fin;
					}
				}
			}
		}
		cur_obj=0xFFFF;
	}
	else
	{
		cur_obj=0xFFFF;
	}

fin:

	CDC* pDC=GetDC();
	PresentObj(pDC);
	ReleaseDC(pDC);
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgObj::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(cur_obj!=0xFFFF)
	{
		if(copymov && !(*pobjlist)[cur_obj].is_folder)
		{
			(*pcount)++;
			OBJ_DATA_EX* newdata=new OBJ_DATA_EX[*pcount];
			memcpy(newdata,*pobjlist,(*pcount-1)*sizeof(OBJ_DATA_EX));
			delete[]*pobjlist;
			*pobjlist=newdata;
			(*pobjlist)[*pcount-1]=(*pobjlist)[cur_obj];
		}
		moving=true;
		SetCapture();
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgObj::OnLButtonUp(UINT nFlags, CPoint point)
{
	moving=false;
	ReleaseCapture();

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgObj::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(moving)return;
	if(cur_obj==0xFFFF)return;
	if((*pobjlist)[cur_obj].is_folder && cur_subobj!=0xFFFF)
	{
		(*pobjlist)[cur_obj].len--;
		if(cur_subobj==0)
		{
			(*pobjlist)[cur_obj].o[0]=(*pobjlist)[cur_obj].o[1];
			(*pobjlist)[cur_obj].o[1]=(*pobjlist)[cur_obj].o[2];
		}
		else if(cur_subobj==1)
		{
			(*pobjlist)[cur_obj].o[1]=(*pobjlist)[cur_obj].o[2];
		}
	}
	else
	{
		(*pcount)--;
		if(*pcount)
		{
			OBJ_DATA_EX* newdata=new OBJ_DATA_EX[*pcount];
			if(cur_obj)memcpy(newdata,
				*pobjlist,
				cur_obj*sizeof(OBJ_DATA_EX));
			if(cur_obj<*pcount)memcpy(newdata+cur_obj,
				*pobjlist+cur_obj+1,
				(*pcount-cur_obj)*sizeof(OBJ_DATA_EX));
			delete[] *pobjlist;
			*pobjlist=newdata;
		}
		else
		{
			delete[] *pobjlist;
			*pobjlist=0;
		}
	}
	cur_obj=0xFFFF;

	CDC* pDC=GetDC();
	PresentObj(pDC);
	ReleaseDC(pDC);

	CDialog::OnRButtonDown(nFlags, point);
}



BOOL CDlgObj::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_CONTROL)
		{	
			copymov=true;
			return true;
		}
		break;
	case WM_KEYUP:
		if(pMsg->wParam==VK_CONTROL)
		{	
			copymov=false;
			return true;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgObj::OnDestroy()
{
	CDialog::OnDestroy();

	*pobj_vertical=m_CheckObjVertical.GetCheck()?1:0;

	ObjSort(true,true,*pobj_vertical?true:false);


}
u32 OBJ_DATA_EX::GetPosCode(bool l2r,bool u2d,bool vert)
{
	u16 tx=is_folder?fx:o[0].x;
	u16 ty=is_folder?fy:o[0].y;
	if(!l2r)tx=~tx;
	if(!u2d)ty=~ty;
	if(vert)return (ty<<16)|tx;else return (tx<<16)|ty;
}
void CDlgObj::ObjSort(bool l2r,bool u2d,bool vert)
{
	if(!*pcount)return;
	OBJ_DATA_EX tdata;
	for(u16 i=0;i<*pcount-1;i++)for(u16 j=i+1;j<*pcount;j++)
	{
		if((*pobjlist)[i].GetPosCode(l2r,u2d,vert)>(*pobjlist)[j].GetPosCode(l2r,u2d,vert))
		{
			tdata=(*pobjlist)[i];
			(*pobjlist)[i]=(*pobjlist)[j];
			(*pobjlist)[j]=tdata;
		}
	}
}
void CDlgObj::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDlgObjParam dlg;
	if(cur_obj!=0xFFFF){
		if((*pobjlist)[cur_obj].is_folder)return;
		dlg.obj=(*pobjlist)[cur_obj].o[0];

		dlg.addobj=false;
		if(dlg.DoModal()==IDCANCEL)return;

		(*pobjlist)[cur_obj].o[0]=dlg.obj;

		CDC* pDC=GetDC();
		PresentObj(pDC);
		ReleaseDC(pDC);

		
	}
	else{
		RECT hsrc,vsrc;
		int hsp=m_HScroll.GetScrollPos();
		int vsp=m_VScroll.GetScrollPos();
		m_HScroll.GetWindowRect(&hsrc);
		m_VScroll.GetWindowRect(&vsrc);
		ScreenToClient(&hsrc);
		ScreenToClient(&vsrc);

		int x=(int)(point.x+hsp-hsrc.left);
		int y=(int)(point.y+vsp-vsrc.top);
		if(point.x<hsrc.right && point.x>hsrc.left &&
			point.y<vsrc.bottom && point.y>vsrc.top && x<(s16)width && y<(s16)height)
		{
			OBJ_DATA_EX newobj;
			newobj.is_folder=0;
			newobj.len=0;
			newobj.fx=newobj.fy=0;
			newobj.o[0].x=x;
			newobj.o[0].y=y;
			newobj.o[0].class_id=0x0001;
			newobj.o[0].paramA=0;
			newobj.o[0].paramB=0;
			dlg.obj=newobj.o[0];
			dlg.addobj=true;
			if(dlg.DoModal()==IDCANCEL)return;
			newobj.o[0]=dlg.obj;
			(*pcount)++;
			OBJ_DATA_EX* newdata=new OBJ_DATA_EX[*pcount];
			if(*pcount-1){
				memcpy(newdata,*pobjlist,(*pcount-1)*sizeof(OBJ_DATA_EX));
				delete[]*pobjlist;
			}
			*pobjlist=newdata;
			(*pobjlist)[*pcount-1]=newobj;

			CDC* pDC=GetDC();
			PresentObj(pDC);
			ReleaseDC(pDC);
		}
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}
