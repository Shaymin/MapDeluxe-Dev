

#include "stdafx.h"
#include "MapDeluxe.h"
#include "DlgRomImage.h"
#include "CodeTran.h"

// CDlgRomImage 对话框

IMPLEMENT_DYNAMIC(CDlgRomImage, CDialog)

CDlgRomImage::CDlgRomImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRomImage::IDD, pParent)
{

}

CDlgRomImage::~CDlgRomImage()
{
}

void CDlgRomImage::PaintImage(CDC* pDC)
{
	CDC dcMem;
	CBitmap bm;
	dcMem.CreateCompatibleDC(pDC);
	bm.CreateCompatibleBitmap(pDC,240,160);
	dcMem.SelectObject(&bm);
	for(int y=0;y<160;y++)for(int x=0;x<240;x++)
	{
		dcMem.SetPixel(x,y,R5G5B5X1toR8G8B8X8(rom_image[x+y*240]));
	}
	pDC->BitBlt(0,0,240,160,&dcMem,0,0,SRCCOPY);
	dcMem.DeleteDC();
	bm.DeleteObject();
}

void CDlgRomImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRomImage, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_IMP_ROM_IMAGE, &CDlgRomImage::OnBnClickedButtonImpRomImage)
END_MESSAGE_MAP()


void CDlgRomImage::OnPaint()
{
	CPaintDC dc(this);
	PaintImage(&dc);
}

void CDlgRomImage::OnBnClickedButtonImpRomImage()
{
	//打开图像
	CFileDialog filedlg(TRUE,0,0,OFN_HIDEREADONLY,
		_T("图像|*.bmp;*.png;*.gif;*.jpg;*.jpeg||"),this);
	if(filedlg.DoModal()==IDCANCEL)return;
	CString filename=filedlg.GetPathName();
	CImage Img;
	if(FAILED(Img.Load(filename.GetBuffer())))
	{
		MessageBox(_T("未能打开图片！"));
		return;
	}
	for(int y=0;y<160;y++)for(int x=0;x<240;x++)
	{
		if(x<Img.GetWidth() && y<Img.GetHeight())
		{
			rom_image[x+y*240]=R8G8B8X8toR5G5B5X1(Img.GetPixel(x,y));
		}
		else
		{
			rom_image[x+y*240]=0;
		}
	}

	CDC *pDC=GetDC();
	PaintImage(pDC);
	ReleaseDC(pDC);
}
