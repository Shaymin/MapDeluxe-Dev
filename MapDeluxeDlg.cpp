//主窗口
//编写:wwylele
/**
*TODO-LIST:
（等待）1、撤销·恢复 //#当操作很多的时候会麻烦，而备份地图又会导致效率下降，纠结>_<
（完成）2、按右键清除当前方块//#清除的定义很模糊，干脆再弄一个背景色好了
（完成）3、按鼠标中键（即按下滚轮）选取方块//#吸管？
（弃坑）4、快速查找方块编号//#个人觉得用WASD控制更有效
（等待）5、掀开数据集体填补（比如按住ctrl再布置掀开数据的话可以一次把所有连在一起的掀开数据填好//#（不懂……）
（进行）6、绘图式布置方块（比如像画方块一样通过拖曳来一次填好一个方块阵）//#正在考虑
（弃坑）7、测试用选项（就是用来测试当前关卡效果什么的，不行的话就弄个“打开模拟器”的按钮）//#……无解
（等待）8、在设置传送门，初始坐标等等信息时有预览图//#……等待乐乐
（完成）9、加个最小化按钮= =//#……
（弃坑）10、按某个快捷键能拖曳移动方块（比如按住shift）//#可以用吸管代替=w=
*/

#include "stdafx.h"
#include "MapDeluxe.h"
#include "MapDeluxeDlg.h"
#include "CodeTran.h"
#include "LZ77.h"
#include "DlgResize.h"
#include "DlgDoor.h"
#include "DebugConsole.h"
#include "DlgRomImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CHECK_ROM_MAGIC

#define ROM_IMAGE_OFFSET 0x00CC0000

// CMapDeluxeDlg 对话框
COLORREF BitColor[8]=
{
	RGB(255,0,0),
	RGB(255,128,0),	
	RGB(255,255,0),
	RGB(0,255,0),
	RGB(0,255,255),
	RGB(0,0,255),
	RGB(255,0,255),
	RGB(255,0,128)
};



CMapDeluxeDlg::CMapDeluxeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapDeluxeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMapDeluxeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALLOW_KEY_CONTROL, m_AllowKeyControl);
	DDX_Control(pDX, IDC_EDIT_STEP_NUMBER, m_EditStepNumber);
	DDX_Control(pDX, IDC_CHECK_LZ77, m_CheckLz77);
	DDX_Control(pDX, IDC_COMBO_BGM, m_ComboBgm);
	DDX_Control(pDX, IDC_COMBO_SPCMAP, m_ComboSpcMap);
	DDX_Control(pDX, IDC_EDIT_CLIP_X, m_EditClipX);
	DDX_Control(pDX, IDC_EDIT_CLIP_Y, m_EditClipY);
	DDX_Control(pDX, IDC_EDIT_BEGIN_X, m_EditBeginX);
	DDX_Control(pDX, IDC_EDIT_BEGIN_Y, m_EditBeginY);
	DDX_Control(pDX, IDC_EDIT_BCK_MOV, m_EditBckMov);
	DDX_Control(pDX, IDC_EDIT_UNK36, m_EditX36);
	DDX_Control(pDX, IDC_EDIT_UNK54, m_EditX54);
	DDX_Control(pDX, IDC_CHECK_UNK3E, m_CheckX3E);
	DDX_Control(pDX, IDC_CHECK_UNK55, m_CheckX55);
	DDX_Control(pDX, IDC_CHECK_BCKROLL, m_CheckBckRoll);
	DDX_Control(pDX, IDC_EDIT_PLT_FRT, m_EditPltFrt);
	DDX_Control(pDX, IDC_EDIT_TILL_FRT, m_EditTillFrt);
	DDX_Control(pDX, IDC_EDIT_GRAMAP_FRT, m_EditGraMapFrt);
	DDX_Control(pDX, IDC_COMBO_GRA_FRT, m_ComboGraFrt);
	DDX_Control(pDX, IDC_EDIT_PLT_BCK, m_EditPltBck);
	DDX_Control(pDX, IDC_EDIT_TILL_BCK, m_EditTillBck);
	DDX_Control(pDX, IDC_EDIT_GRAMAP_BCK, m_EditGraMapBck);
	DDX_Control(pDX, IDC_COMBO_GRA_BCK, m_ComboGraBck);
	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFileName);
	DDX_Control(pDX, IDC_TREE_FILE, m_Tree);
	DDX_Control(pDX, IDC_EDIT_GRA_ANI, m_EditGraAni);
	DDX_Control(pDX, IDC_SCROLLBAR_MAPH, m_HScrollMap);
	DDX_Control(pDX, IDC_SCROLLBAR_MAPV, m_VScrollMap);
	DDX_Control(pDX, IDC_COMBO_MAPCHG, m_ComboMapChg);
	DDX_Control(pDX, IDC_STATIC_GRID_INFO, m_StaticGridInfo);
	DDX_Control(pDX, IDC_SCROLLBAR_GRALIB, m_ScrollBarGraLib);
	DDX_Control(pDX, IDC_STATIC_LIB_INFO, m_StaticLibInfo);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_ButtonNew);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_ButtonOpen);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ButtonSave);
	DDX_Control(pDX, IDC_BUTTON_SAVEAS, m_ButtonSaveAs);
	DDX_Control(pDX, IDC_CHECK_NEST, m_CheckNest);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_BUTTON_RESIZE, m_ButtonResize);
	DDX_Control(pDX, IDC_BUTTON_SAVE_FINAL, m_ButtonSaveFinal);
	DDX_Control(pDX, IDC_CHECK_NEST_AUTO, m_CheckNestAuto);
	DDX_Control(pDX, IDC_CHECK_EDG, m_CheckEdg);
	DDX_Control(pDX, IDC_EDIT_EDG, m_EditEdg);
	DDX_Control(pDX, IDC_BUTTON_ROM_IMAGE, m_ButtonRomImage);
	DDX_Control(pDX, IDC_BUTTON_GAME_TEST, m_ButtonGameTest);
}

BEGIN_MESSAGE_MAP(CMapDeluxeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMapDeluxeDlg::OnBnClickedButtonOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_FILE, &CMapDeluxeDlg::OnNMDblclkTreeFile)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CMapDeluxeDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMapDeluxeDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_CHECK_LZ77, &CMapDeluxeDlg::OnBnClickedCheckLz77)
	ON_CBN_SELCHANGE(IDC_COMBO_BGM, &CMapDeluxeDlg::OnCbnSelchangeComboBgm)
	ON_CBN_SELCHANGE(IDC_COMBO_SPCMAP, &CMapDeluxeDlg::OnCbnSelchangeComboSpcmap)
	ON_EN_CHANGE(IDC_EDIT_CLIP_X, &CMapDeluxeDlg::OnEnChangeEditClipX)
	ON_EN_CHANGE(IDC_EDIT_CLIP_Y, &CMapDeluxeDlg::OnEnChangeEditClipY)
	ON_EN_CHANGE(IDC_EDIT_BEGIN_X, &CMapDeluxeDlg::OnEnChangeEditBeginX)
	ON_EN_CHANGE(IDC_EDIT_BEGIN_Y, &CMapDeluxeDlg::OnEnChangeEditBeginY)
	ON_EN_CHANGE(IDC_EDIT_BCK_MOV, &CMapDeluxeDlg::OnEnChangeEditBckMov)
	ON_EN_CHANGE(IDC_EDIT_UNK36, &CMapDeluxeDlg::OnEnChangeEditUnk36)
	ON_EN_CHANGE(IDC_EDIT_UNK54, &CMapDeluxeDlg::OnEnChangeEditUnk54)
	ON_BN_CLICKED(IDC_CHECK_UNK3E, &CMapDeluxeDlg::OnBnClickedCheckUnk3e)
	ON_BN_CLICKED(IDC_CHECK_UNK55, &CMapDeluxeDlg::OnBnClickedCheckUnk55)
	ON_BN_CLICKED(IDC_CHECK_BCKROLL, &CMapDeluxeDlg::OnBnClickedCheckBckroll)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_COMBO_MAPCHG, &CMapDeluxeDlg::OnCbnSelchangeComboMapchg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TEST, &CMapDeluxeDlg::OnBnClickedButtonSaveTest)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CMapDeluxeDlg::OnBnClickedButtonSaveas)
	ON_BN_CLICKED(IDC_CHECK_NEST, &CMapDeluxeDlg::OnBnClickedCheckNest)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_FINAL, &CMapDeluxeDlg::OnBnClickedButtonSaveFinal)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE, &CMapDeluxeDlg::OnBnClickedButtonResize)
	ON_BN_CLICKED(IDC_BUTTON_DOOR, &CMapDeluxeDlg::OnBnClickedButtonDoor)
	ON_EN_CHANGE(IDC_EDIT_PLT_FRT, &CMapDeluxeDlg::OnEnChangeEditPltFrt)
	ON_EN_CHANGE(IDC_EDIT_PLT_BCK, &CMapDeluxeDlg::OnEnChangeEditPltBck)
	ON_EN_CHANGE(IDC_EDIT_TILL_FRT, &CMapDeluxeDlg::OnEnChangeEditTillFrt)
	ON_EN_CHANGE(IDC_EDIT_TILL_BCK, &CMapDeluxeDlg::OnEnChangeEditTillBck)
	ON_EN_CHANGE(IDC_EDIT_GRAMAP_FRT, &CMapDeluxeDlg::OnEnChangeEditGramapFrt)
	ON_EN_CHANGE(IDC_EDIT_GRAMAP_BCK, &CMapDeluxeDlg::OnEnChangeEditGramapBck)
	ON_EN_CHANGE(IDC_EDIT_GRA_ANI, &CMapDeluxeDlg::OnEnChangeEditGraAni)
	ON_BN_CLICKED(IDC_CLEAR_MAP, &CMapDeluxeDlg::OnBnClickedClearMap)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK_EDG, &CMapDeluxeDlg::OnBnClickedCheckEdg)
	ON_BN_CLICKED(IDC_BUTTON_ROM_IMAGE, &CMapDeluxeDlg::OnBnClickedButtonRomImage)
	ON_BN_CLICKED(IDC_BUTTON_GAME_TEST, &CMapDeluxeDlg::OnBnClickedButtonGameTest)
END_MESSAGE_MAP()


// CMapDeluxeDlg 消息处理程序

BOOL CMapDeluxeDlg::OnInitDialog()
{
	InitConsole();
	CString strVer=GetProductVersion();
	CString str;
	printf("MapDeluxe-dev Initializing.\n");
	AlphaValue=128;
	LockRefreshing=false;
	MouseLState=false;
	MouseRState=false;
	
	BFunction.BlendOp=AC_SRC_OVER;
	BFunction.BlendFlags=0;
	BFunction.SourceConstantAlpha=AlphaValue;
	BFunction.AlphaFormat=AC_SRC_ALPHA;
	
	CDialog::OnInitDialog();
	
	TCHAR auth[]=_T("vuvh`f^");
	for(int i=0;i<7;i++)auth[i]+=i+1;//=w=被我发现了
	str.Format(_T("MapDeluxe 版本:%s 作者:%s"),strVer.GetBuffer(),auth);
	SetWindowText(str.GetBuffer());


	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_ToolTip.Create(this,TTS_BALLOON);
	m_ToolTip.Activate(TRUE); 
	m_ToolTip.SetMaxTipWidth(400);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,30000);
	//m_ToolTip.SetTitle(0,_T("aaa"));

	InitToolTipText();

	ZeroMemory(map,sizeof(GRID_DATA*)*2*STEP_INDEXING_COUNT);
	ZeroMemory(door,sizeof(DOOR_DATA*)*STEP_INDEXING_COUNT);
	ZeroMemory(obj,sizeof(OBJ_DATA*)*STEP_INDEXING_COUNT);

	cur_step=0xFFFF;
	cur_x=cur_y=0xFFFF;
	cur_sel=0xFFFF;
	cur_chg=0;

	
	for(u32 i=0;i<256;i++)if(CodeTran_Bgm((u8)i))
	{
		str.Format(CODETRAN_FORMAT,i,CodeTran_Bgm((u8)i));
		m_ComboBgm.AddString(str.GetBuffer());
	}
	for(u8 i=0;i<10;i++)if(CodeTran_SpcMap(i))
	{
		str.Format(CODETRAN_FORMAT,i,CodeTran_SpcMap(i));
		m_ComboSpcMap.AddString(str.GetBuffer());
	}

	SCROLLINFO scrinfo={0};
	RECT scrrc;
	scrinfo.cbSize=sizeof(SCROLLINFO);
	scrinfo.fMask=SIF_PAGE;
	m_HScrollMap.GetWindowRect(&scrrc);
	scrinfo.nPage=scrrc.right-scrrc.left;
	m_HScrollMap.SetScrollInfo(&scrinfo);
	m_VScrollMap.GetWindowRect(&scrrc);
	scrinfo.nPage=scrrc.bottom-scrrc.top;
	m_VScrollMap.SetScrollInfo(&scrinfo);

	CDC* pDC=GetDC();
	m_TmpDC.CreateCompatibleDC(pDC);
	m_TmpDC2.CreateCompatibleDC(pDC);
	m_TmpDC3.CreateCompatibleDC(pDC);
	m_BmpMap.CreateCompatibleBitmap(pDC,1,1);//No use
	m_BmpGra.CreateCompatibleBitmap(pDC,1,1);//No use
	m_BmpGraOut.CreateCompatibleBitmap(pDC,1,1);//No use
	m_BmpDet.LoadBitmap(IDB_DET);
	m_BmpDetOut.CreateCompatibleBitmap(pDC,256,256);
	m_BmpDetMask.CreateCompatibleBitmap(pDC,256,256);
	m_BmpCache.CreateCompatibleBitmap(pDC,256,256);
	//m_BmpDetMask.CreateBitmap(
	m_TmpDC.SelectObject(&m_BmpDet);
	m_TmpDC2.SelectObject(&m_BmpDetMask);
	m_TmpDC3.SelectObject(&m_BmpCache);
	COLORREF cbck=m_TmpDC.GetPixel(0,0);
	for(int y=0;y<256;y++)for(int x=0;x<256;x++)
	{
		m_TmpDC2.SetPixel(x,y,m_TmpDC.GetPixel(x,y)==cbck?
			RGB(255,255,255):0);
	}
	m_BmpNest.LoadBitmap(IDB_NEST);

	m_ComboMapChg.AddString(_T("图像"));
	m_ComboMapChg.AddString(_T("判定"));
	m_ComboMapChg.SelectString(0,_T("图像"));

	m_CheckNestAuto.SetCheck(TRUE);
	m_AllowKeyControl.SetCheck(TRUE);

	m_EditEdg.SetWindowText(_T("0"));
	m_EditEdg.EnableWindow(FALSE);
	m_CheckEdg.EnableWindow(FALSE);
	
	
	
	CString strFileName=AfxGetApp()->m_lpCmdLine;
	strFileName.Replace(_T("\""),_T(""));
	CFile file;
	char *MagicStr="ROM_REBUILD_V1";
	char MagicTmp[16];
	if(!file.Open(strFileName.GetBuffer(),CFile::modeReadWrite))
	{
		//MessageBox(_T("未能打开文件！此文件可能正在被其它程序使用！"),_T("错误"),MB_ICONERROR);
		return TRUE;
	}


#ifdef CHECK_ROM_MAGIC
	file.Seek(0x7FFFF0,CFile::begin);
	file.Read(MagicTmp,16);
	if(strcmp(MagicTmp,MagicStr)!=0)
	{
		//MessageBox(_T("所选文件非可解析的梦之泉ROM。\n请选择本程序创建的ROM！"),_T("错误"),MB_ICONERROR);
		file.Close();
		return TRUE;
	}
#endif

	ReadRom(file);

	m_EditFileName.SetWindowText(strFileName.GetBuffer());


	printf("MapDeluxe-dev Initialized.\n");
	return TRUE;  
}
void CMapDeluxeDlg::ClearData()
{
	for(int index=0;index<STEP_INDEXING_COUNT;index++)
	{
		if(map[0][index])
		{
			delete[] (u8*)(map[0][index]);
			map[0][index]=0;
		}
		if(map[1][index])
		{
			delete[] map[1][index];
			map[1][index]=0;
		}
		if(door[index])
		{
			delete[] door[index];
			door[index]=0;
		}
		if(obj[index])
		{
			delete[] obj[index];
			obj[index]=0;
		}
	}
}
void CMapDeluxeDlg::LockRomResource()
{
	HRSRC hResInfo=FindResource(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_ROM),_T("ROM")); 
	hRomResData=LoadResource(AfxGetApp()->m_hInstance,hResInfo);
	pRomRebuild=LockResource(hRomResData);
}
void CMapDeluxeDlg::UnlockRomResource()
{
	UnlockResource(hRomResData);
	FreeResource(hRomResData);
}

void CMapDeluxeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC DC(this);
		PresentMap(&DC);
		PresentGraLib(&DC);
		CDialog::OnPaint();
	}
}


HCURSOR CMapDeluxeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMapDeluxeDlg::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);
	if (m_AllowKeyControl.GetCheck()){
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST && pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam=='W')
		{
			//printf("W Pressed.\n");
			CString str;
			m_ComboMapChg.GetWindowText(str);
			switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
			{
			case 0:
				//gralib_len
				cur_chg-=16;
				if (cur_chg>65000) {
					cur_chg+=16;
					if ((gralib_len%16)>=cur_chg) cur_chg=gralib_len-(gralib_len%16-cur_chg);
					else cur_chg=gralib_len-(16-cur_chg+gralib_len%16);
					}
				break;
			case 1:
				//256
				cur_chg-=16;
				if (cur_chg>65000) cur_chg+=256;
				break;
			}
			CDC* pDC=GetDC();
			PaintGraLib();
			//PresentMap(pDC);
			PresentGraLib(pDC);
			ReleaseDC(pDC);
			//UpdateGridInfo();
			//UpdateLibInfo();
			m_ScrollBarGraLib.SetScrollPos(max((cur_chg/16 - 5)*16,0),TRUE);
		}
		if(pMsg->wParam=='A')
		{
			//printf("W Pressed.\n");
			CString str;
			m_ComboMapChg.GetWindowText(str);
			switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
			{
			case 0:
				//gralib_len
				cur_chg-=1;
				if (cur_chg>65000) cur_chg=gralib_len-65536+cur_chg;
				break;
			case 1:
				//256
				cur_chg-=1;
				if (cur_chg>65000) cur_chg+=256;
				break;
			}
			CDC* pDC=GetDC();
			PaintGraLib();
			//PresentMap(pDC);
			PresentGraLib(pDC);
			ReleaseDC(pDC);
			//UpdateGridInfo();
			//UpdateLibInfo();
			m_ScrollBarGraLib.SetScrollPos(max((cur_chg/16 - 5)*16,0),TRUE);
		}
		if(pMsg->wParam=='D')
		{
			//printf("W Pressed.\n");
			CString str;
			m_ComboMapChg.GetWindowText(str);
			switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
			{
			case 0:
				//gralib_len
				cur_chg+=1;
				if (cur_chg>=gralib_len) cur_chg-=gralib_len;
				break;
			case 1:
				//256
				cur_chg+=1;
				if (cur_chg>=256) cur_chg-=256;
				break;
			}
			CDC* pDC=GetDC();
			PaintGraLib();
			//PresentMap(pDC);
			PresentGraLib(pDC);
			ReleaseDC(pDC);
			//UpdateGridInfo();
			//UpdateLibInfo();
			m_ScrollBarGraLib.SetScrollPos(max((cur_chg/16 - 5)*16,0),TRUE);
		}
		if(pMsg->wParam=='S')
		{
			//printf("W Pressed.\n");
			CString str;
			m_ComboMapChg.GetWindowText(str);
			switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
			{
			case 0:
				//gralib_len
				cur_chg+=16;
				if (cur_chg>=gralib_len) cur_chg=cur_chg%16;
				break;
			case 1:
				//256
				cur_chg+=16;
				if (cur_chg>=256) cur_chg-=256;
				break;
			}
			CDC* pDC=GetDC();
			PaintGraLib();
			//PresentMap(pDC);
			PresentGraLib(pDC);
			ReleaseDC(pDC);
			//UpdateGridInfo();
			//UpdateLibInfo();
			m_ScrollBarGraLib.SetScrollPos(max((cur_chg/16 - 5)*16,0),TRUE);
		}
		if (pMsg->wParam=='Z'){
			CString str;
			m_ComboMapChg.GetWindowText(str);
			switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
			{
			case 0:
				m_ComboMapChg.SelectString(0,_T("判定"));
				break;
			case 1:
				//256
				m_ComboMapChg.SelectString(0,_T("图像"));				
				break;
			}
			CMapDeluxeDlg::OnCbnSelchangeComboMapchg();
		}
		if (pMsg->wParam=='X'){
			if (m_CheckNest.GetCheck()){
				m_CheckNest.SetCheck(FALSE);
			}
			else {
				m_CheckNest.SetCheck(TRUE);
			}
			CMapDeluxeDlg::OnBnClickedCheckNest();
		}
	}}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMapDeluxeDlg::OnDestroy()
{
	FreeConsole();
	CDialog::OnDestroy();
	ClearData();
	cur_step=0xFFFF;
}

void CMapDeluxeDlg::OnBnClickedButtonOpen()
{
	char *MagicStr="ROM_REBUILD_V1";
	char MagicTmp[16];

	//打开文件
	CFileDialog filedlg(TRUE,0,0,OFN_HIDEREADONLY,
		_T("梦之泉ROM|*.gba||"),this);
	if(filedlg.DoModal()==IDCANCEL)return;
	CFile file;
	CString strFileName=filedlg.GetPathName();
	if(!file.Open(strFileName.GetBuffer(),CFile::modeReadWrite))
	{
		MessageBox(_T("未能打开文件！此文件可能正在被其它程序使用！"),_T("错误"),MB_ICONERROR);
		return;
	}
#define CHECK_ROM_MAGIC

#ifdef CHECK_ROM_MAGIC
	file.Seek(0x7FFFF0,CFile::begin);
	file.Read(MagicTmp,16);
	if(strcmp(MagicTmp,MagicStr)!=0)//雪米才不知道这是什么呢
	{
		MessageBox(_T("所选文件非可解析的梦之泉ROM。\n请选择本程序创建的ROM！"),_T("错误"),MB_ICONERROR);
		file.Close();
		return;
	}
#endif

	ReadRom(file);

	m_EditFileName.SetWindowText(strFileName.GetBuffer());
	
	file.Close();
}
void CMapDeluxeDlg::StepHeaderOut()
{
	CString str;
	str.Format(_T("Level%d.Stage%d.Step%02d"),
		step_header[cur_step].level+1,
		step_header[cur_step].stage+1,
		step_header[cur_step].step);
	m_EditStepNumber.SetWindowText(str.GetBuffer());

	m_CheckLz77.SetCheck(step_header[cur_step].lz77?TRUE:FALSE);

	str.Format(_T("%d"),step_header[cur_step].clip_x);
	m_EditClipX.SetWindowTextW(str.GetBuffer());
	str.Format(_T("%d"),step_header[cur_step].clip_y);
	m_EditClipY.SetWindowTextW(str.GetBuffer());

	str.Format(_T("%d"),step_header[cur_step].begin_x);
	m_EditBeginX.SetWindowTextW(str.GetBuffer());
	str.Format(_T("%d"),step_header[cur_step].begin_y);
	m_EditBeginY.SetWindowTextW(str.GetBuffer());

	str.Format(_T("%d"),step_header[cur_step].bck_mov);
	m_EditBckMov.SetWindowTextW(str.GetBuffer());

	str.Format(_T("%d"),step_header[cur_step].x36);
	m_EditX36.SetWindowTextW(str.GetBuffer());

	str.Format(_T("%d"),step_header[cur_step].x54);
	m_EditX54.SetWindowTextW(str.GetBuffer());

	m_CheckX3E.SetCheck(step_header[cur_step].x3E?TRUE:FALSE);
	m_CheckX55.SetCheck(step_header[cur_step].x55?TRUE:FALSE);
	m_CheckBckRoll.SetCheck(step_header[cur_step].bck_roll?TRUE:FALSE);

	str.Format(_T("0x%08X"),step_header[cur_step].ptr_plt_frt);
	m_EditPltFrt.SetWindowText(str.GetBuffer());
	str.Format(_T("0x%08X"),step_header[cur_step].ptr_till_frt);
	m_EditTillFrt.SetWindowText(str.GetBuffer());
	str.Format(_T("0x%08X"),step_header[cur_step].ptr_gra_map_frt);
	m_EditGraMapFrt.SetWindowText(str.GetBuffer());

	str.Format(_T("0x%08X"),step_header[cur_step].ptr_plt_bck);
	m_EditPltBck.SetWindowText(str.GetBuffer());
	str.Format(_T("0x%08X"),step_header[cur_step].ptr_till_bck);
	m_EditTillBck.SetWindowText(str.GetBuffer());
	str.Format(_T("0x%08X"),step_header[cur_step].ptr_gra_map_bck);
	m_EditGraMapBck.SetWindowText(str.GetBuffer());

	str.Format(_T("%d"),step_header[cur_step].gra_ani_index);
	m_EditGraAni.SetWindowText(str.GetBuffer());

	str.Format(CODETRAN_FORMAT,step_header[cur_step].bgm,CodeTran_Bgm(step_header[cur_step].bgm));
	m_ComboBgm.SelectString(0,str.GetBuffer());

	str.Format(CODETRAN_FORMAT,step_header[cur_step].spc_map,CodeTran_SpcMap(step_header[cur_step].spc_map));
	m_ComboSpcMap.SelectString(0,str.GetBuffer());
}

u32 CMapDeluxeDlg::StrToI(CString &str)
{
	u32 s;
	try{
		int x=str.Find(_T(":"));
		if(x!=-1)
		{
			str=str.Left(x);
		}
		x=str.Find(_T("0x"));
		if(x==-1)
		{
			//return _ttoi(str.GetBuffer());
			_stscanf_s(str.GetBuffer(),_T("%u"),&s);
		}
		else
		{
			_stscanf_s(str.GetBuffer(),_T("0x%x"),&s);
		}
	}catch(...){
		s=0;
	}
	return s;
	
}


void CMapDeluxeDlg::LoadGraLib()
{
	if(cur_step==0xFFFF)return;
	LockRomResource();

	//读取调色板
	u16* plt=(u16*)((u8*)pRomRebuild+(step_header[cur_step].ptr_plt_frt-0x08000000+2));

	//读取瓦图
	u32 lz77len;
	lz77len=*(u32*)((u8*)pRomRebuild+(step_header[cur_step].ptr_till_frt-0x08000000));
	lz77len>>=8;
	u8* till_lz=new u8[lz77len+0x100];
	TILE *till=(TILE *)till_lz;
	UncompressLZ((u8*)pRomRebuild+(step_header[cur_step].ptr_till_frt-0x08000000),till_lz);

	//读取映射
	lz77len=*(u32*)((u8*)pRomRebuild+(step_header[cur_step].ptr_gra_map_frt-0x08000000));
	lz77len>>=8;
	u32 liblen=lz77len/8;
	u8* gmap_lz=new u8[lz77len+0x100];
	BLOCK_MAPPING* pMapping=(BLOCK_MAPPING*)gmap_lz;
	UncompressLZ((u8*)pRomRebuild+(step_header[cur_step].ptr_gra_map_frt-0x08000000),gmap_lz);

	m_BmpGra.DeleteObject();
	m_BmpGraOut.DeleteObject();
	CDC *pDC=GetDC(); 
	//m_BmpGra.CreateCompatibleBitmap(pDC,liblen*16,16);
	m_BmpGra.CreateCompatibleBitmap(pDC,256,(liblen/16+1)*16);
	m_BmpGraOut.CreateCompatibleBitmap(pDC,256,(liblen/16+1)*16);
	m_TmpDC2.SelectObject(&m_BmpGra);
	u16 submapping;
	bool flipx,flipy;
	u8 pltcls;
	u8 pltidx;
	u32 drx,dry;
	for(u32 i=0;i<liblen;i++)
	{
		for(int subb=0;subb<4;subb++)
		{
			submapping=pMapping[i].mapping[subb]&0x3FF;
			flipx=pMapping[i].mapping[subb]&1024 ? true:false;
			flipy=pMapping[i].mapping[subb]&2048 ? true:false;
			pltcls=pMapping[i].mapping[subb]>>12;
			for(int by=0;by<8;by++)for(int bx=0;bx<8;bx++)
			{
				pltidx=till[submapping].Get(flipx?7-bx:bx,flipy?7-by:by);
				drx=(i%16)*16+(subb%2)*8+bx;
				dry=(i/16)*16+(subb/2)*8+by;
				if(pltidx!=0)m_TmpDC2.SetPixel(drx,dry,
						R5G5B5X1toR8G8B8X8(plt[(pltcls-2)*16+(pltidx)])
						);
				else m_TmpDC2.SetPixel(drx,dry,
						(drx&4) ^ (dry&4) ? RGB(250,250,250):RGB(255,255,255)
						);
			}
		}
	}

	ReleaseDC(pDC);

	delete[] till_lz;
	delete[] gmap_lz;
	UnlockRomResource();

	gralib_len=liblen;
}
void CMapDeluxeDlg::OnNMDblclkTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LockRefreshing=true;
	HTREEITEM tic=m_Tree.GetSelectedItem();
	CString str;
	for(int i=0;i<STEP_INDEXING_COUNT;i++)
	{
		if(ti_step[i]==tic)
		{


			if(pNMHDR)cur_step=i;
			StepHeaderOut();

			LoadGraLib();

			//设置滚动条
			RECT hsrc,vsrc;
			int hsrg,vsrg;
			m_HScrollMap.GetWindowRect(&hsrc);
			m_VScrollMap.GetWindowRect(&vsrc);
			hsrg=step_header[i].width*16-(hsrc.right-hsrc.left);
			if(hsrg<1)hsrg=1;
			vsrg=step_header[i].height*16-(vsrc.bottom-vsrc.top);
			if(vsrg<1)vsrg=1;
			SCROLLINFO scrinfo={0};
			scrinfo.cbSize=sizeof(SCROLLINFO);
			scrinfo.fMask=SIF_PAGE|SIF_RANGE|SIF_POS;
			scrinfo.nPage=hsrc.right-hsrc.left;
			scrinfo.nMin=0;scrinfo.nMax=hsrg-1+scrinfo.nPage-1;
			scrinfo.nPos=0;
			m_HScrollMap.SetScrollInfo(&scrinfo);
			scrinfo.nPage=vsrc.bottom-vsrc.top;
			scrinfo.nMin=0;scrinfo.nMax=vsrg-1+scrinfo.nPage-1;
			scrinfo.nPos=0;
			m_VScrollMap.SetScrollInfo(&scrinfo);

			ResetGraLibScrollBar();
			cur_chg=0;
			cur_chgR=0;

			CDC* pDC=GetDC();
			m_BmpMap.DeleteObject();
			m_BmpMap.CreateCompatibleBitmap(pDC,step_header[i].width*16,step_header[i].height*16);
			m_TmpDC.SelectObject(&m_BmpMap);
			ScreenToClient(&hsrc);
			ScreenToClient(&vsrc);
			pDC->FillRect((RECT*)&CRect(hsrc.left,vsrc.top,hsrc.right,vsrc.bottom),&CBrush(RGB(0,0,0)));
			PaintMap();
			PaintGraLib();
			PresentMap(pDC);
			PresentGraLib(pDC);
			ReleaseDC(pDC);

			LockRefreshing=false;
			if(pResult)*pResult=0;
			return;
		}
	}
	LockRefreshing=false;
	if(pResult)*pResult = 0;
	
}
void CMapDeluxeDlg::PaintMap()
{
	/**
	*>_<雪米已经尽全力试图读懂这段代码了……还是问乃吧……
	*/
	printf("System:Map Repainted\n");
	if(cur_step==0xFFFF)return;
	for(u16 y=0;y<step_header[cur_step].height;y++)for(u16 x=0;x<step_header[cur_step].width;x++)
	PaintMapTile(x,y);	
}
void CMapDeluxeDlg::PresentMap(CDC* pDC)
{
	if(cur_step==0xFFFF)return;
	m_TmpDC.SelectObject(&m_BmpMap);
	RECT hsrc,vsrc;
	int hsp=m_HScrollMap.GetScrollPos();
	int vsp=m_VScrollMap.GetScrollPos();
	m_HScrollMap.GetWindowRect(&hsrc);
	m_VScrollMap.GetWindowRect(&vsrc);
	ScreenToClient(&hsrc);
	ScreenToClient(&vsrc);
	pDC->BitBlt(hsrc.left,vsrc.top,
		hsrc.right-hsrc.left,vsrc.bottom-vsrc.top,&m_TmpDC,
		hsp,vsp,SRCCOPY);
}
void CMapDeluxeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

	//PaintMap();
	PaintMapTile(cur_x,cur_y);
	CDC* pDC=GetDC();
	PresentMap(pDC);
	ReleaseDC(pDC);
}

void CMapDeluxeDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
	//PaintMap();
	PaintMapTile(cur_x,cur_y);
	PaintGraLib();
	CDC* pDC=GetDC();
	PresentMap(pDC);
	PresentGraLib(pDC);
	ReleaseDC(pDC);
}
void CMapDeluxeDlg::ReadRom(CFile& file)
{
	ClearData();
	cur_step=0xFFFF;
	m_Tree.DeleteAllItems();
	CString str;
	m_Tree.DeleteAllItems();
	ti_rom=m_Tree.InsertItem(_T("ROM"));
	for(u32 level=0;level<8;level++)
	{
		str.Format(_T("Level%d"),level+1);
		ti_level[level]=m_Tree.InsertItem(str.GetBuffer(),ti_rom);
	}
	ZeroMemory(ti_stage,sizeof(HTREEITEM)*8*8);

	u32 lz77len;
	u8* lz77psrc;
	u8* lz77pdst;
	for(int index=0;index<STEP_INDEXING_COUNT;index++)
	{
		file.Seek(0x00800000+index*0x4000,CFile::begin);
		file.Read(&step_header[index],sizeof(STEP_HEADER));
		if(ti_stage[step_header[index].level][step_header[index].stage]==0)
		{
			str.Format(_T("Stage%d"),step_header[index].stage+1);
			ti_stage[step_header[index].level][step_header[index].stage]=
				m_Tree.InsertItem(str.GetBuffer(),ti_level[step_header[index].level]);
		}
		str.Format(_T("Step%02d"),step_header[index].step);
		ti_step[index]=m_Tree.InsertItem(str.GetBuffer(),
			ti_stage[step_header[index].level][step_header[index].stage]);

		//主地图
		file.Seek(0x00800000+index*0x4000+0x00001000,CFile::begin);
		file.Read(&lz77len,4);
		lz77len>>=8;
		file.Seek(-4,CFile::current);
		lz77psrc=new u8[lz77len];
		lz77pdst=new u8[lz77len+0x100];
		file.Read(lz77psrc,lz77len);
		UncompressLZ(lz77psrc,lz77pdst);
		delete[]lz77psrc;
		map[0][index]=(GRID_DATA*)lz77pdst;

		//嵌套地图
		file.Seek(0x00800000+index*0x4000+0x00002000,CFile::begin);
		file.Read(&lz77len,4);
		lz77len>>=8;
		file.Seek(-4,CFile::current);
		lz77psrc=new u8[lz77len];
		lz77pdst=new u8[lz77len+0x100];
		file.Read(lz77psrc,lz77len);
		UncompressLZ(lz77psrc,lz77pdst);
		delete[]lz77psrc;
		map[1][index]=new GRID_DATA[step_header[index].width*step_header[index].height];
		u16* pnest=(u16*)lz77pdst;
		for(s32 k=0;k<step_header[index].width*step_header[index].height;k++)
		{
			if(pnest[k]==0)
			{
				*(u32*)&map[1][index][k]=0xFFFFFFFF;
			}
			else
			{
				file.Seek(0x00800000+index*0x4000+0x00003000+pnest[k]*4,CFile::begin);
				file.Read(&map[1][index][k],4);
			}
		}
		delete[]lz77pdst;

		//门
		if(step_header[index].door_count)
		{
			file.Seek(0x00800000+index*0x4000+0x00000100,CFile::begin);
			door[index]=new DOOR_DATA[step_header[index].door_count];
			file.Read(door[index],step_header[index].door_count*sizeof(DOOR_DATA));
		}

	}

	//起始页
	file.Seek(ROM_IMAGE_OFFSET,CFile::begin);
	file.Read(rom_image,sizeof(u16)*160*240);
	
	
}

void CMapDeluxeDlg::OnBnClickedButtonNew()
{
	//打开文件
	CFileDialog filedlg(FALSE,_T("gba"),_T("我的梦之泉.gba"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("梦之泉ROM|*.gba||"),this);
	filedlg.m_ofn.lpstrTitle=_T("选择新游戏的保存位置...");
	if(filedlg.DoModal()==IDCANCEL)return;
	CFile file;
	CString strFileName=filedlg.GetPathName();
	if(!file.Open(strFileName.GetBuffer(),CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox(_T("未能创建文件，请重试"),_T("错误"),MB_ICONERROR);
		return;
	}
	LockRomResource();
	file.Write(pRomRebuild,0x01000000);
	file.Close();
	UnlockRomResource();
	
	file.Open(strFileName.GetBuffer(),CFile::modeReadWrite);
	ReadRom(file);

	m_EditFileName.SetWindowText(strFileName.GetBuffer());
	
	file.Close();
}

void CMapDeluxeDlg::OnBnClickedButtonSave()
{
	if(map[0][0]==0)return;
	CString strFileName;
	m_EditFileName.GetWindowText(strFileName);
	CFile file;
	if(!file.Open(strFileName.GetBuffer(),CFile::modeReadWrite))
	{
		MessageBox(_T("未能写入文件！此文件可能正在被其它程序使用！"),_T("错误"),MB_ICONERROR);
		return;
	}
	LockRomResource();
	file.Write(pRomRebuild,0x01000000);//
	UnlockRomResource();
	WriteRom(file);
	file.Close();
}

void CMapDeluxeDlg::WriteRom(CFile& file)
{
	m_Progress.SetRange(1,STEP_INDEXING_COUNT);
	u8 *buff;
	u16 *nestbuff;
	u32 len;
	CString str;
	const u32 zero32=0,F32=0xFFFFFFFF;
	CList<u32> nestlist;
	for(int index=0;index<STEP_INDEXING_COUNT;index++)
	{
		file.Seek(0x00800000+index*0x4000,CFile::begin);
		file.Write(&step_header[index],sizeof(STEP_HEADER));

		file.Seek(0x00800000+index*0x4000+0x00001000,CFile::begin);
		buff=new u8[step_header[index].width*step_header[index].height*4];
		len=CompressLZ((u8*)(map[0][index]),
			step_header[index].width*step_header[index].height*4,
			buff);
		if(len>0x1000 || len==0)
		{
			delete[] buff;
			str.Format(_T("地图[Level%d.Stage%d.Step%02d]的主地图空间溢出！\n写入文件已中断"),
				step_header[index].level+1,step_header[index].stage+1,step_header[index].step);
			MessageBox(str.GetBuffer());
			m_Progress.SetPos(0);
			return;
		}
		file.Write(buff,len);
		delete[] buff;

		nestlist.RemoveAll();
		nestbuff=new u16[step_header[index].width*step_header[index].height];
		for(int k=0;k<step_header[index].width*step_header[index].height;k++)
		{
			if(*(u32*)&map[1][index][k]==0xFFFFFFFF)
			{
				nestbuff[k]=0;
			}
			else
			{
				POSITION pos = nestlist.GetHeadPosition();
				int ki=0;
				while(1)   
				{
					if(pos==0)
					{
						ki=-1;
						break;
					}
					if(nestlist.GetNext(pos)==*(u32*)&map[1][index][k])
					{
						break;
					}
					ki++;
				}
				if(ki!=-1)
				{
					nestbuff[k]=(u16)(ki+1);
				}
				else
				{
					nestlist.AddTail(*(u32*)&map[1][index][k]);
					nestbuff[k]=nestlist.GetCount()-1+1;
				}
			}
		}
		file.Seek(0x00800000+index*0x4000+0x00003000,CFile::begin);
		file.Write(&zero32,4);
		POSITION pos;
		pos=nestlist.GetHeadPosition();  
		if(nestlist.GetCount()+2>0x1000/4)
		{
			delete[] nestbuff;
			str.Format(_T("地图[Level%d.Stage%d.Step%02d]的掀开地图表溢出！\n写入文件已中断"),
				step_header[index].level+1,step_header[index].stage+1,step_header[index].step);
			MessageBox(str.GetBuffer());
			m_Progress.SetPos(0);
			return;
		}
		while(pos)   
		{   
			file.Write(&nestlist.GetNext(pos),4);
		}
		file.Write(&F32,4);
		/*
		nestbuff=new u16[step_header[index].width*step_header[index].height];
		file.Seek(0x00800000+index*0x4000+0x00003000,CFile::begin);
		file.Write(&zero32,4);
		nestcount=1;
		for(int k=0;k<step_header[index].width*step_header[index].height;k++)
		{
			if(*(u32*)&map[1][index][k]==0xFFFFFFFF)
			{
				nestbuff[k]=0;
			}
			else
			{
				nestbuff[k]=nestcount;
				nestcount++;
				if(nestcount+1>0x1000/4)
				{
					delete[] nestbuff;
					str.Format(_T("地图[Level%d.Stage%d.Step%02d]的掀开地图表溢出！\n写入文件已中断"),
						step_header[index].level+1,step_header[index].stage+1,step_header[index].step);
					MessageBox(str.GetBuffer());
					m_Progress.SetPos(0);
					return;
				}
				file.Write(&map[1][index][k],4);
			}
		}
		file.Write(&F32,4);
		*/
		buff=new u8[step_header[index].width*step_header[index].height*2];
		len=CompressLZ((u8*)(nestbuff),
			step_header[index].width*step_header[index].height*2,
			buff);
		if(len>0x1000 || len==0)
		{
			delete[] nestbuff;
			str.Format(_T("地图[Level%d.Stage%d.Step%02d]的掀开地图映射溢出！\n写入文件已中断"),
				step_header[index].level+1,step_header[index].stage+1,step_header[index].step);
			MessageBox(str.GetBuffer());
			m_Progress.SetPos(0);
			return;
		}
		file.Seek(0x00800000+index*0x4000+0x00002000,CFile::begin);
		file.Write(buff,len);
		delete[]nestbuff;
		delete[]buff;

		//写入传送门
		file.Seek(0x00800000+index*0x4000+0x00000100,CFile::begin);
		file.Write(door[index],step_header[index].door_count*sizeof(DOOR_DATA));


		m_Progress.SetPos(index+1);
		UpdateData(FALSE);
	}
	
	//起始页
	file.Seek(ROM_IMAGE_OFFSET,CFile::begin);
	file.Write(rom_image,sizeof(u16)*160*240);

	MessageBox(_T("写入文件成功！"),_T("成功"));
	m_Progress.SetPos(0);
}
void CMapDeluxeDlg::OnBnClickedCheckLz77()
{
	if(cur_step==0xFFFF)return;
	step_header[cur_step].lz77=m_CheckLz77.GetCheck()?1:0;
}

void CMapDeluxeDlg::OnCbnSelchangeComboBgm()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_ComboBgm.GetWindowText(str);
	step_header[cur_step].bgm=(u8)StrToI(str);
}

void CMapDeluxeDlg::OnCbnSelchangeComboSpcmap()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_ComboSpcMap.GetWindowText(str);
	step_header[cur_step].spc_map=(u8)StrToI(str);
}

void CMapDeluxeDlg::OnEnChangeEditClipX()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_EditClipX.GetWindowText(str);
	step_header[cur_step].clip_x=(u16)StrToI(str);
}
void CMapDeluxeDlg::OnEnChangeEditClipY()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_EditClipY.GetWindowText(str);
	step_header[cur_step].clip_y=(u16)StrToI(str);
}


void CMapDeluxeDlg::OnEnChangeEditBeginX()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_EditBeginX.GetWindowText(str);
	step_header[cur_step].begin_x=(u16)StrToI(str);

}

void CMapDeluxeDlg::OnEnChangeEditBeginY()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_EditBeginY.GetWindowText(str);
	step_header[cur_step].begin_y=(u16)StrToI(str);
}

void CMapDeluxeDlg::OnEnChangeEditBckMov()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_EditBckMov.GetWindowText(str);
	step_header[cur_step].bck_mov=(u16)StrToI(str);
}

void CMapDeluxeDlg::OnEnChangeEditUnk36()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_EditX36.GetWindowText(str);
	step_header[cur_step].x36=(u16)StrToI(str);

	// TODO:  在此添加控件通知处理程序代码
}

void CMapDeluxeDlg::OnEnChangeEditUnk54()
{
	if(cur_step==0xFFFF)return;
	CString str;
	m_EditX54.GetWindowText(str);
	step_header[cur_step].x54=(u8)StrToI(str);
}

void CMapDeluxeDlg::OnBnClickedCheckUnk3e()
{
	if(cur_step==0xFFFF)return;
	step_header[cur_step].x3E=m_CheckX3E.GetCheck()?1:0;
}

void CMapDeluxeDlg::OnBnClickedCheckUnk55()
{
	if(cur_step==0xFFFF)return;
	step_header[cur_step].x55=m_CheckX55.GetCheck()?1:0;
}

void CMapDeluxeDlg::OnBnClickedCheckBckroll()
{
	if(cur_step==0xFFFF)return;
	step_header[cur_step].bck_roll=m_CheckBckRoll.GetCheck()?1:0;
}


void CMapDeluxeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(cur_step==0xFFFF)return;
	//PaintMapTile(cur_x,cur_y);
	RECT hsrc,vsrc,glsrc;
	int hsp=m_HScrollMap.GetScrollPos();
	int vsp=m_VScrollMap.GetScrollPos();
	int lsp=m_ScrollBarGraLib.GetScrollPos();
	m_HScrollMap.GetWindowRect(&hsrc);
	m_VScrollMap.GetWindowRect(&vsrc);
	m_ScrollBarGraLib.GetWindowRect(&glsrc);
	ScreenToClient(&hsrc);
	ScreenToClient(&vsrc);
	ScreenToClient(&glsrc);
	CRect rectmap;

	u16 tcur_x,tcur_y,tcur_sel;
	if(point.x<hsrc.right && point.x>hsrc.left &&
		point.y<vsrc.bottom && point.y>vsrc.top)
	{
		tcur_x=(u16)((point.x-hsrc.left+hsp)/16);
		tcur_y=(u16)((point.y-vsrc.top+vsp)/16);
		if(tcur_x>=step_header[cur_step].width ||
			tcur_y>=step_header[cur_step].height)
		{
			tcur_x=0xFFFF;
			tcur_y=0xFFFF;
		}
		tcur_sel=0xFFFF;
	}
	else if(point.y<glsrc.bottom && point.y>glsrc.top &&
		point.x>glsrc.right && point.x<glsrc.right+256)
	{
		tcur_x=0xFFFF;
		tcur_y=0xFFFF;
		tcur_sel=(u16)((point.x-glsrc.right)/16 + (point.y-glsrc.top+lsp)/16*16);
		int len;
		CString str;
		m_ComboMapChg.GetWindowText(str);
		switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
		{
		case 0:
			len=(gralib_len/16+1)*16;
			break;
		case 1:
			len=256;
			break;
		}
		if(tcur_sel>=len)tcur_sel=0xFFFF;
	}
	else
	{
		tcur_x=0xFFFF;
		tcur_y=0xFFFF;
		tcur_sel=0xFFFF;
	}
	
	if(tcur_x!=cur_x||tcur_y!=cur_y||tcur_sel!=cur_sel)//@wwylele 改了一个低级错误=w=
	{
		int t1=cur_x,t2=cur_y;
		cur_x=tcur_x;
		cur_y=tcur_y;
		PaintMapTile(t1,t2);
		cur_sel=tcur_sel;
		if ((cur_step!=0xFFFF)&&(MouseLState))
		DrawGrid();
		if ((cur_step!=0xFFFF)&&(MouseRState))
		DrawGridR();
		//PaintMap();
		PaintMapTile(cur_x,cur_y);
		PaintGraLib();
		CDC* pDC=GetDC();
		PresentMap(pDC);
		PresentGraLib(pDC);
		ReleaseDC(pDC);
		UpdateGridInfo();
		UpdateLibInfo();
	}
	
	CDialog::OnMouseMove(nFlags, point);
}
void CMapDeluxeDlg::UpdateGridInfo()
{
	if(cur_step==0xFFFF)return;
	CString str,str2;
	if(cur_x==0xFFFF)
	{
		m_StaticGridInfo.SetWindowText(_T(""));
		return;
	}
	str.Format(_T("(%u,%u)\n[主数据]\n判定=%u(%s)\n图像=%u\n未知值=%u"),
		cur_x,cur_y,
		map[0][cur_step][cur_x+cur_y*step_header[cur_step].width].det,
		CodeTran_Det(map[0][cur_step][cur_x+cur_y*step_header[cur_step].width].det),
		map[0][cur_step][cur_x+cur_y*step_header[cur_step].width].gra,
		map[0][cur_step][cur_x+cur_y*step_header[cur_step].width].edg
		);
	if(*(u32*)&map[1][cur_step][cur_x+cur_y*step_header[cur_step].width]!=0xFFFFFFFF)
	{
		str2.Format(_T("\n[掀开数据]\n判定=%u(%s)\n图像=%u\n未知值=%u"),
			map[1][cur_step][cur_x+cur_y*step_header[cur_step].width].det,
			CodeTran_Det(map[1][cur_step][cur_x+cur_y*step_header[cur_step].width].det),
			map[1][cur_step][cur_x+cur_y*step_header[cur_step].width].gra,
			map[1][cur_step][cur_x+cur_y*step_header[cur_step].width].edg
			);
	}
	else
	{
		str2=_T("\n[掀开数据]\n无");
	}
	str+=str2;
	m_StaticGridInfo.SetWindowText(str.GetBuffer());
}
void CMapDeluxeDlg::UpdateLibInfo()
{
	if(cur_step==0xFFFF)return;
	CString str;
	if(cur_sel==0xFFFF)
	{

		CString str2;
		m_ComboMapChg.GetWindowText(str2);
		if(m_ComboMapChg.FindString(-1,str2.GetBuffer())==1)
		{
			str.Format(_T("已选中编号:%d(%s)/%d(%s)"),
				cur_chg,CodeTran_Det((u8)cur_chg),
				cur_chgR,CodeTran_Det((u8)cur_chgR)
			);
		}
		else
		{
			str.Format(_T("已选中编号:%d/%d"),
				cur_chg,
				cur_chgR
				);
		}
		m_StaticLibInfo.SetWindowText(str.GetBuffer());
		return;
	}
	

	CString str2;
	m_ComboMapChg.GetWindowText(str2);
	if(m_ComboMapChg.FindString(-1,str2.GetBuffer())==1)
	{
		str.Format(_T("已选中编号:%d(%s)/%d(%s)\n当前编号:%d(%s)"),
			cur_chg,CodeTran_Det((u8)cur_chg),
			cur_chgR,CodeTran_Det((u8)cur_chgR),
			cur_sel,CodeTran_Det((u8)cur_sel)
		);
	}
	else
	{
		str.Format(_T("已选中编号:%d/%d\n当前编号:%d"),
			cur_chg,cur_chgR,cur_sel
			);
	}

	m_StaticLibInfo.SetWindowText(str.GetBuffer());
}
void CMapDeluxeDlg::OnCbnSelchangeComboMapchg()
{
	if(cur_step==0xFFFF)return;
	
	CString str;
	m_ComboMapChg.GetWindowText(str);
	bool EdgOpen=(m_ComboMapChg.FindString(-1,str.GetBuffer())==1);
	m_CheckEdg.EnableWindow(EdgOpen);
	m_EditEdg.EnableWindow(EdgOpen && m_CheckEdg.GetCheck());
	
	ResetGraLibScrollBar();
	cur_chg=0;
	cur_chgR=0;
	CDC* pDC=GetDC();
	PaintMap();
	PaintGraLib();
	PresentMap(pDC);
	PresentGraLib(pDC);
	ReleaseDC(pDC);
}


void CMapDeluxeDlg::ResetGraLibScrollBar()
{
	int len;
	CString str;
	m_ComboMapChg.GetWindowText(str);
	switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
	{
	case 0:
		len=(gralib_len/16+1)*16;
		break;
	case 1:
		len=256;
		break;
	}

	RECT vsrc;
	int vsrg;
	m_ScrollBarGraLib.GetWindowRect(&vsrc);
	vsrg=len-(vsrc.bottom-vsrc.top);
	if(vsrg<1)vsrg=1;
	SCROLLINFO scrinfo={0};
	scrinfo.cbSize=sizeof(SCROLLINFO);
	scrinfo.fMask=SIF_PAGE|SIF_RANGE|SIF_POS;
	scrinfo.nPage=vsrc.bottom-vsrc.top;
	scrinfo.nMin=0;scrinfo.nMax=vsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_ScrollBarGraLib.SetScrollInfo(&scrinfo);
}

void CMapDeluxeDlg::PaintGraLib()
{
	if(cur_step==0xFFFF)return;
	static CPen PenCur(PS_SOLID,1,RGB(255,0,0)),PenChg(PS_SOLID,2,RGB(255,0,0)),PenChgR(PS_SOLID,2,RGB(0,0,255));
	CBrush BrushNull;
	BrushNull.CreateStockObject(NULL_BRUSH);

	int len;
	CString str;
	m_ComboMapChg.GetWindowText(str);
	switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
	{
	case 0:
		len=(gralib_len/16+1)*16;
		m_TmpDC2.SelectObject(&m_BmpGra);
		m_TmpDC.SelectObject(&m_BmpGraOut);
		break;
	case 1:
		len=256;
		m_TmpDC2.SelectObject(&m_BmpDet);
		m_TmpDC.SelectObject(&m_BmpDetOut);
		break;
	}
	
	m_TmpDC.BitBlt(0,0,256,len,&m_TmpDC2,0,0,SRCCOPY);
	if(cur_sel!=0xFFFF)
	{
		m_TmpDC.SelectObject(&BrushNull);
		m_TmpDC.SelectObject(&PenCur);
		m_TmpDC.Rectangle((cur_sel%16)*16,cur_sel/16*16,(cur_sel%16)*16+16,cur_sel/16*16+16);
		
	}
	m_TmpDC.SelectObject(&PenChgR);
	m_TmpDC.Rectangle((cur_chgR%16)*16,cur_chgR/16*16,(cur_chgR%16)*16+16,cur_chgR/16*16+16);
	m_TmpDC.SelectObject(&PenChg);
	m_TmpDC.Rectangle((cur_chg%16)*16,cur_chg/16*16,(cur_chg%16)*16+16,cur_chg/16*16+16);

}
void CMapDeluxeDlg::PresentGraLib(CDC* pDC)
{
	if(cur_step==0xFFFF)return;
	int len;
	CString str;
	m_ComboMapChg.GetWindowText(str);
	switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
	{
	case 0:
		len=(gralib_len/16+1)*16;
		m_TmpDC.SelectObject(&m_BmpGraOut);
		break;
	case 1:
		len=256;
		m_TmpDC.SelectObject(&m_BmpDetOut);
		break;
	}
	
	RECT vsrc;
	int vsp=m_ScrollBarGraLib.GetScrollPos();
	m_ScrollBarGraLib.GetWindowRect(&vsrc);
	ScreenToClient(&vsrc);
	pDC->BitBlt(vsrc.right,vsrc.top,
		256,vsrc.bottom-vsrc.top,&m_TmpDC,
		0,vsp,SRCCOPY);
}
void CMapDeluxeDlg::DrawGrid()
{
	int nest=m_CheckNest.GetCheck()?1:0;
	if(cur_step==0xFFFF)return;
	if(cur_x!=0xFFFF)
	{
		CString str;
		m_ComboMapChg.GetWindowText(str);
		if(*(u32*)&map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width]==0xFFFFFFFF)
		{
			*(u32*)&map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width]=0;
		}
		switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
		{
		case 0:
			map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width].gra=cur_chg;
			break;
		case 1:
			map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width].det=(u8)cur_chg;
			if(m_CheckEdg.GetCheck())
			{
				m_EditEdg.GetWindowText(str);
				map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width].edg=(u8)StrToI(str);
			}
			if(nest==0 && m_CheckNestAuto.GetCheck())
			{
				*(u32*)&map[1][cur_step][cur_x+cur_y*step_header[cur_step].width]=
					CodeTran_DetNset((u8)cur_chg)?0:0xFFFFFFFF;
			}
			break;
		}

	}
	else if(cur_sel!=0xFFFF)
	{
		cur_chg=cur_sel;
	}
}

void CMapDeluxeDlg::DrawGridR()
{
	int nest=m_CheckNest.GetCheck()?1:0;
	if (m_CheckNest.GetCheck()){
		*(u32*)&map[1][cur_step][cur_x+cur_y*step_header[cur_step].width]=0xFFFFFFFF;
	}
	else 
	if(cur_x!=0xFFFF)
	{
		CString str;
		m_ComboMapChg.GetWindowText(str);
		if(*(u32*)&map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width]==0xFFFFFFFF)
		{
			*(u32*)&map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width]=0;
		}
		switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
		{
		case 0:
			map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width].gra=cur_chgR;
			break;
		case 1:
			map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width].det=(u8)cur_chgR;
			if(nest==0 && m_CheckNestAuto.GetCheck())
			{
				*(u32*)&map[1][cur_step][cur_x+cur_y*step_header[cur_step].width]=
					CodeTran_DetNset((u8)cur_chgR)?0:0xFFFFFFFF;
			}
			break;
		}

	}
	else if(cur_sel!=0xFFFF)
	{
		cur_chgR=cur_sel;
	}
}

void CMapDeluxeDlg::OnLButtonUp(UINT nFlags, CPoint point){
	MouseLState=false;
}
void CMapDeluxeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	MouseLState=true;
	if(cur_step==0xFFFF)return;
	DrawGrid();
	CDC* pDC=GetDC();
	//PaintMap();
	PaintMapTile(cur_x,cur_y);
	PaintGraLib();
	PresentMap(pDC);
	PresentGraLib(pDC);
	ReleaseDC(pDC);
	UpdateGridInfo();
	UpdateLibInfo();
	CDialog::OnLButtonDown(nFlags, point);
}
void CMapDeluxeDlg::OnRButtonUp(UINT nFlags, CPoint point){
	MouseRState=false;
}
void CMapDeluxeDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	MouseRState=true;
	if(cur_step==0xFFFF)return;
	DrawGridR();
	CDC* pDC=GetDC();
	//PaintMap();
	PaintMapTile(cur_x,cur_y);
	PaintGraLib();
	PresentMap(pDC);
	PresentGraLib(pDC);
	ReleaseDC(pDC);
	UpdateGridInfo();
	UpdateLibInfo();
	CDialog::OnRButtonDown(nFlags, point);
}
void CMapDeluxeDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	int nest=m_CheckNest.GetCheck()?1:0;
	if(cur_x!=0xFFFF)
	{
		CString str;
		m_ComboMapChg.GetWindowText(str);
		if(*(u32*)&map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width]==0xFFFFFFFF)
		{
			*(u32*)&map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width]=0;
		}
		switch(m_ComboMapChg.FindString(-1,str.GetBuffer()))
		{
		case 0:
			cur_chg=map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width].gra;
			break;
		case 1:
			cur_chg=map[nest][cur_step][cur_x+cur_y*step_header[cur_step].width].det;
			break;
		}

	}
	CDC* pDC=GetDC();
	PaintGraLib();
	//PresentMap(pDC);
	PresentGraLib(pDC);
	ReleaseDC(pDC);
	//UpdateGridInfo();
	//UpdateLibInfo();
	m_ScrollBarGraLib.SetScrollPos((cur_chg/16)*16,TRUE);
	
}

void CMapDeluxeDlg::OnBnClickedButtonSaveTest()
{
	if(cur_step==0xFFFF)return;

	bool lhe=true,lok=true;

	u32 mmlen,nmlen;
	u8* buff;
	buff=new u8[step_header[cur_step].width*step_header[cur_step].height*4];
	mmlen=CompressLZ((u8*)(map[0][cur_step]),
		step_header[cur_step].width*step_header[cur_step].height*4,
		buff);
	delete[] buff;
	if(mmlen>0x1000 || mmlen==0)lok=false;
	if(mmlen>0xC00)lhe=false;

	u16 nestcount;
	u16* nestbuff;
	nestbuff=new u16[step_header[cur_step].width*step_header[cur_step].height];
	CList<u32> nestlist;
	nestlist.RemoveAll();
	for(int k=0;k<step_header[cur_step].width*step_header[cur_step].height;k++)
	{
		if(*(u32*)&map[1][cur_step][k]==0xFFFFFFFF)
		{
			nestbuff[k]=0;
		}
		else
		{
			POSITION pos = nestlist.GetHeadPosition();
			int ki=0;
			while(1)   
			{
				if(pos==0)
				{
					ki=-1;
					break;
				}
				if(nestlist.GetNext(pos)==*(u32*)&map[1][cur_step][k])
				{
					break;
				}
				ki++;
			}
			if(ki!=-1)
			{
				nestbuff[k]=(u16)(ki+1);
			}
			else
			{
				nestlist.AddTail(*(u32*)&map[1][cur_step][k]);
				nestbuff[k]=nestlist.GetCount()-1+1;
			}
		}
	}
	/*for(int k=0;k<step_header[cur_step].width*step_header[cur_step].height;k++)
	{
		if(*(u32*)&map[1][cur_step][k]==0xFFFFFFFF)
		{
			nestbuff[k]=0;
		}
		else
		{
			nestbuff[k]=nestcount;
			nestcount++;
		}
	}*/

	nestcount=nestlist.GetCount()+2;
	if(nestcount*4>0x1000)lok=false;
	if(nestcount*4>0xC00)lhe=false;

	buff=new u8[step_header[cur_step].width*step_header[cur_step].height*2];
	nmlen=CompressLZ((u8*)(nestbuff),
		step_header[cur_step].width*step_header[cur_step].height*2,
		buff);
	if(nmlen>0x1000 || nmlen==0)lok=false;
	if(nmlen>0xC00)lhe=false;
	delete[]nestbuff;
	delete[]buff;

	CString str;
	str.Format(_T("主地图占用空间:%.3fKB / 4.000KB\n")
		_T("掀开地图映射占用空间:%.3fKB / 4.000KB\n")
		_T("掀开地图表占用空间:%.3fKB / 4.000KB\n")
		_T("空间占用情况:%s"),
		mmlen/1024.0f,nmlen/1024.0f,nestcount*4/1024.0f,
		lok?(lhe?_T("良好 ^_^"):_T("危险 -_-")):_T("溢出 ×_×"));
	MessageBox(str.GetBuffer(),_T("测试结果"));
}

void CMapDeluxeDlg::OnBnClickedButtonSaveas()
{
	if(map[0][0]==0)return;
	//打开文件
	CFileDialog filedlg(FALSE,_T("gba"),_T("我的梦之泉.gba"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("梦之泉ROM|*.gba||"),this);
	filedlg.m_ofn.lpstrTitle=_T("另存为...");
	if(filedlg.DoModal()==IDCANCEL)return;
	CFile file;
	CString strFileName=filedlg.GetPathName();
	if(!file.Open(strFileName.GetBuffer(),CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox(_T("未能创建文件，请重试"),_T("错误"),MB_ICONERROR);
		return;
	}
	LockRomResource();
	file.Write(pRomRebuild,0x01000000);
	UnlockRomResource();
	WriteRom(file);
	m_EditFileName.SetWindowText(strFileName.GetBuffer());
	file.Close();
}

void CMapDeluxeDlg::OnBnClickedCheckNest()
{
	CDC* pDC=GetDC();
	PaintMap();
	PresentMap(pDC);
	ReleaseDC(pDC);
}

/*void CMapDeluxeDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(cur_step==0xFFFF)return;

	if(!m_CheckNest.GetCheck())return;
	*(u32*)&map[1][cur_step][cur_x+cur_y*step_header[cur_step].width]=0xFFFFFFFF;
	CDC* pDC=GetDC();
	PaintMap();
	PresentMap(pDC);
	ReleaseDC(pDC);

	CDialog::OnRButtonDown(nFlags, point);
}*/

void CMapDeluxeDlg::OnBnClickedButtonSaveFinal()
{
	if(map[0][0]==0)return;
	char MagicErase[]="FINAL_PLAY_ONLY";
	//打开文件
	CFileDialog filedlg(FALSE,_T("gba"),_T("我的梦之泉.gba"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("梦之泉ROM|*.gba||"),this);
	filedlg.m_ofn.lpstrTitle=_T("另存为最终版游戏...");
	if(filedlg.DoModal()==IDCANCEL)return;
	CFile file;
	CString strFileName=filedlg.GetPathName();
	if(!file.Open(strFileName.GetBuffer(),CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox(_T("未能创建文件，请重试"),_T("错误"),MB_ICONERROR);
		return;
	}
	LockRomResource();
	file.Write(pRomRebuild,0x01000000);
	UnlockRomResource();
	file.Seek(0x7FFFF0,CFile::begin);//
	file.Write(MagicErase,16);//Rewrite the magic text which is the flag of editing
	WriteRom(file);
	file.Close();
}

void CMapDeluxeDlg::OnBnClickedButtonResize()
{
	if(cur_step==0xFFFF)return;
	CDlgResize dlg;
	dlg.width=step_header[cur_step].width;
	dlg.height=step_header[cur_step].height;
	if(dlg.DoModal()==IDCANCEL)return;


	GRID_DATA* newmap[2];
	for(int n=0;n<2;n++)
	{
		newmap[n]=new GRID_DATA[dlg.width*dlg.height];
		for(u16 y=0;y<dlg.height;y++)for(u16 x=0;x<dlg.width;x++)
		{
			if(x+dlg.dx>-1 && x+dlg.dx<step_header[cur_step].width &&
				y+dlg.dy>-1 && y+dlg.dy<step_header[cur_step].height)
			{
				newmap[n][x+y*dlg.width]=
					map[n][cur_step][x+dlg.dx+(y+dlg.dy)*step_header[cur_step].width];
			}
			else
			{
				*(u32*)&newmap[n][x+y*dlg.width]=n?0xFFFFFFFF:0;
			}
		}
		delete[] map[n][cur_step];
		map[n][cur_step]=newmap[n];
	}
	step_header[cur_step].width=dlg.width;
	step_header[cur_step].height=dlg.height;
	OnNMDblclkTreeFile(0,0);//重新加载地图
	
}

void CMapDeluxeDlg::OnBnClickedButtonDoor()
{
	if(cur_step==0xFFFF)return;
	u16 tcount=step_header[cur_step].door_count;
	DOOR_DATA *tdata;
	if(tcount)
	{
		tdata=new DOOR_DATA[tcount];
		memcpy(tdata,door[cur_step],tcount*sizeof(DOOR_DATA));
	}
	else
	{
		tdata=0;
	}
	CDlgDoor dlg;
	dlg.pcount=&tcount;
	dlg.ppdata=&tdata;
	//添加坐标预设菜单
	CString str;
	u8 det0,det1;
	int menuidx=0;
	for(u16 y=0;y<step_header[cur_step].height;y++)for(u16 x=0;x<step_header[cur_step].width;x++)
	{
		det0=map[0][cur_step][x+y*step_header[cur_step].width].det;
		det1=map[1][cur_step][x+y*step_header[cur_step].width].det;
		if(det0==0x10||det0==0x36||det0==0x37||det0==0x90||det0==0xB6||det0==0xB7)
		{
			str.Format(_T("(%d,%d)%s"),x,y,CodeTran_Det(det0));
			dlg.m_MenuXY.AppendMenu(MF_STRING,DOOR_XY_MENU_ID_BASE+menuidx,str.GetBuffer());
			menuidx++;
		}
		else if(det1==0x10||det1==0x36||det1==0x37||det1==0x90||det1==0xB6||det1==0xB7)
		{
			str.Format(_T("(%d,%d)(掀开)%s"),x,y,CodeTran_Det(det1));
			dlg.m_MenuXY.AppendMenu(MF_STRING,DOOR_XY_MENU_ID_BASE+menuidx,str.GetBuffer());
			menuidx++;
		}
	}
	if(dlg.DoModal()==IDOK)
	{
		if(step_header[cur_step].door_count)
		{
			delete[] door[cur_step];
		}
		step_header[cur_step].door_count=tcount;
		door[cur_step]=tdata;
	}
	else
	{
		if(tcount)delete[] tdata;
	}
}


void CMapDeluxeDlg::Refresh()
{
	if (LockRefreshing==true) return;
	HTREEITEM tic=m_Tree.GetSelectedItem();
	CString str;
	for(int i=0;i<STEP_INDEXING_COUNT;i++)
	{
		if(ti_step[i]==tic)
		{
			m_Progress.SetRange(1,7);
			LoadGraLib();
			m_Progress.SetPos(1);
			CDC* pDC=GetDC();
			m_BmpMap.DeleteObject();
			m_BmpMap.CreateCompatibleBitmap(pDC,step_header[i].width*16,step_header[i].height*16);
			m_TmpDC.SelectObject(&m_BmpMap);
			PaintMap();
			m_Progress.SetPos(2);
			PaintGraLib();
			m_Progress.SetPos(3);
			PresentMap(pDC);
			m_Progress.SetPos(4);
			PresentGraLib(pDC);
			m_Progress.SetPos(5);
			ReleaseDC(pDC);
			m_Progress.SetPos(6);
		}
	}
	
	m_Progress.SetPos(0);
}

void CMapDeluxeDlg::OnEnChangeEditPltFrt()
{
	CString f;
	m_EditPltFrt.GetWindowText(f);
	if ((wcstol(f,NULL,16)>16777215)&&(wcstol(f,NULL,16)<268435455)){
	step_header[cur_step].ptr_plt_frt=wcstol(f,NULL,16);
	CMapDeluxeDlg::Refresh();}
}
void CMapDeluxeDlg::OnEnChangeEditPltBck()
{
	CString f;
	m_EditPltBck.GetWindowText(f);
	if ((wcstol(f,NULL,16)>16777215)&&(wcstol(f,NULL,16)<268435455)){
	step_header[cur_step].ptr_plt_bck=wcstol(f,NULL,16);
	CMapDeluxeDlg::Refresh();}
}


void CMapDeluxeDlg::OnEnChangeEditTillFrt()
{
	CString f;
	m_EditTillFrt.GetWindowText(f);
	if ((wcstol(f,NULL,16)>16777215)&&(wcstol(f,NULL,16)<268435455)){
	step_header[cur_step].ptr_till_frt=wcstol(f,NULL,16);
	CMapDeluxeDlg::Refresh();}
}


void CMapDeluxeDlg::OnEnChangeEditTillBck()
{
	CString f;
	m_EditTillBck.GetWindowText(f);
	if ((wcstol(f,NULL,16)>16777215)&&(wcstol(f,NULL,16)<268435455)){
	step_header[cur_step].ptr_till_bck=wcstol(f,NULL,16);
	CMapDeluxeDlg::Refresh();}
}


void CMapDeluxeDlg::OnEnChangeEditGramapFrt()
{
	CString f;
	m_EditGraMapFrt.GetWindowText(f);
	if ((wcstol(f,NULL,16)>16777215)&&(wcstol(f,NULL,16)<268435455)){
	step_header[cur_step].ptr_gra_map_frt=wcstol(f,NULL,16);
	CMapDeluxeDlg::Refresh();}

}


void CMapDeluxeDlg::OnEnChangeEditGramapBck()
{
	CString f;
	m_EditGraMapBck.GetWindowText(f);
	if ((wcstol(f,NULL,16)>16777215)&&(wcstol(f,NULL,16)<268435455)){
	step_header[cur_step].ptr_gra_map_bck=wcstol(f,NULL,16);
	CMapDeluxeDlg::Refresh();}
}


void CMapDeluxeDlg::OnEnChangeEditGraAni()
{
	CString f;
	m_EditGraMapBck.GetWindowText(f);
	step_header[cur_step].gra_ani_index=wcstol(f,NULL,10);
	CMapDeluxeDlg::Refresh();
}

void CMapDeluxeDlg::OnBnClickedClearMap()
{
	if(cur_step==0xFFFF)return;
	if(MessageBox(_T("会将本地图完全清空，就和新的一样，数据不可恢复（当然SL大法除外）。\n是否继续?"),
		_T("警告"),MB_YESNO)==IDYES)
	{
		for(s32 z=0;z<step_header[cur_step].width*step_header[cur_step].height;z++)
		{
			*(u32*)&map[0][cur_step][z]=0;
			*(u32*)&map[1][cur_step][z]=0xFFFFFFFF;

		}
		CDC* pDC=GetDC();
		PaintMap();
		PresentMap(pDC);
		ReleaseDC(pDC);
	}

}
void CMapDeluxeDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR fname[256]={0};
	DragQueryFile(hDropInfo,0,fname,255);
	DragFinish(hDropInfo);

	CString strFileName=fname;
	CFile file;
	char *MagicStr="ROM_REBUILD_V1";
	char MagicTmp[16];
	if(!file.Open(strFileName.GetBuffer(),CFile::modeReadWrite))
	{
		MessageBox(_T("未能打开文件！此文件可能正在被其它程序使用！"),_T("错误"),MB_ICONERROR);
		return;
	}

#ifdef CHECK_ROM_MAGIC
	file.Seek(0x7FFFF0,CFile::begin);
	file.Read(MagicTmp,16);
	if(strcmp(MagicTmp,MagicStr)!=0)
	{
		MessageBox(_T("所选文件非可解析的梦之泉ROM。\n请选择本程序创建的ROM！"),_T("错误"),MB_ICONERROR);
		file.Close();
		return;
	}
#endif

	ReadRom(file);

	m_EditFileName.SetWindowText(strFileName.GetBuffer());

	CDialog::OnDropFiles(hDropInfo);
}

void CMapDeluxeDlg::OnBnClickedCheckEdg()
{
	m_EditEdg.EnableWindow(m_CheckEdg.GetCheck());
}

void CMapDeluxeDlg::OnBnClickedButtonRomImage()
{
	if(map[0][0]==0)return;
	CDlgRomImage dlg;
	memcpy(dlg.rom_image,rom_image,sizeof(u16)*240*160);
	if(dlg.DoModal()==IDOK)memcpy(rom_image,dlg.rom_image,sizeof(u16)*240*160);
}

void CMapDeluxeDlg::OnBnClickedButtonGameTest()
{
	if(map[0][0]==0)return;
	OnBnClickedButtonSave();
	
	CString str;
	m_EditFileName.GetWindowText(str);
	str=_T("VisualBoyAdvance.exe \"")+str+_T("\"");
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi,sizeof(pi));
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	CreateProcess(0,str.GetBuffer(),0,0,0,0,0,0,&si,&pi);
}

void CMapDeluxeDlg::PaintMapTile(int x, int y)
{
	int nestm=m_CheckNest.GetCheck()?2:1;

	static CPen PenCur(PS_SOLID,1,RGB(255,0,0));
	CBrush BrushNull;
	BrushNull.CreateStockObject(NULL_BRUSH);

	m_TmpDC.SelectObject(&m_BmpMap);
	u16 v;
	CString str;
	for(int nest=0;nest<nestm;nest++)
	{
		if(*(u32*)&map[nest][cur_step][x+y*step_header[cur_step].width]!=0xFFFFFFFF)
		{
			m_TmpDC2.SelectObject(&m_BmpGra);
			v=map[nest][cur_step][x+y*step_header[cur_step].width].gra;
			m_TmpDC.BitBlt(x*16,y*16,16,16,&m_TmpDC2,(v&0xF)*16,(v>>4)*16,SRCCOPY);
			
		}
		else
		{
			m_TmpDC2.SelectObject(&m_BmpNest);
			m_TmpDC.BitBlt(x*16,y*16,16,16,&m_TmpDC2,0,0,SRCAND);
		}
	}
	m_ComboMapChg.GetWindowText(str);
	for(int nest=0;nest<nestm;nest++)
	{
		if(*(u32*)&map[nest][cur_step][x+y*step_header[cur_step].width]!=0xFFFFFFFF)
		{
			if(m_ComboMapChg.FindString(-1,str.GetBuffer())==1)
			{
				v=map[nest][cur_step][x+y*step_header[cur_step].width].det;
				m_TmpDC2.SelectObject(&m_BmpDet);
				m_TmpDC.BitBlt(x*16,y*16,16,16,&m_TmpDC2,(v&0xF)*16,(v>>4)*16,SRCINVERT);
				m_TmpDC2.SelectObject(&m_BmpDetMask);
				m_TmpDC.BitBlt(x*16,y*16,16,16,&m_TmpDC2,(v&0xF)*16,(v>>4)*16,SRCAND);
				m_TmpDC2.SelectObject(&m_BmpDet);
				m_TmpDC.BitBlt(x*16,y*16,16,16,&m_TmpDC2,(v&0xF)*16,(v>>4)*16,SRCINVERT);
				v=map[nest][cur_step][x+y*step_header[cur_step].width].edg;
				for(int biti=0;biti<8;biti++)
				{
					if((v>>biti)&1)
						for(int sx=1;sx<3;sx++)for(int sy=0;sy<2;sy++)
							m_TmpDC.SetPixel(x*16+biti*2+sx,y*16+8+sy,BitColor[biti]);
				}
			}
		}
	}
	if(cur_x!=0xFFFF)
	{
		m_TmpDC.SelectObject(&BrushNull);
		m_TmpDC.SelectObject(&PenCur);
		m_TmpDC.Rectangle(cur_x*16,cur_y*16,cur_x*16+16,cur_y*16+16);
		
	}
}

