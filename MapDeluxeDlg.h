//主窗口类
//编写:wwylele

#pragma once
#include "afxwin.h"
#include "MapDataType.h"
#include "afxcmn.h"

#define STEP_INDEXING_COUNT 292

struct TILE
{
	u8 dt[32];
	inline u8 Get(u8 x,u8 y)
	{
		u8& k=dt[y*4+x/2];
		return x%2 ? k>>4:k&0xF;
	}
	inline void Set(u8 x,u8 y,u8 value)
	{
		u8& k=dt[y*4+x/2];
		if(x%2)
		{
			k=(k&0xF)|(value<<4);
			
		}
		else
		{
			k=(k&0xF0)|(value&0xF);
		}
	}
	inline bool operator ==(TILE& value)
	{
		//bool k=true;
		for(int i=0;i<32;i++)
		{
			if(dt[i]!=value.dt[i])return false;
		}
		//return k;
		return true;
	}
};

struct BLOCK_MAPPING
{
	u16 mapping[4];
};

// CMapDeluxeDlg 对话框
class CMapDeluxeDlg : public CDialog
{
// 构造
public:
	CMapDeluxeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DREAMATRIX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	
	STEP_HEADER step_header[STEP_INDEXING_COUNT];

	GRID_DATA* map[2][STEP_INDEXING_COUNT];//map[0][]=mainmap[],map[1][]=nestmap[]
	DOOR_DATA* door[STEP_INDEXING_COUNT];
	OBJ_DATA* obj[STEP_INDEXING_COUNT];//还没用呢


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CToolTipCtrl m_ToolTip;
	CEdit m_EditStepNumber;
	CButton m_CheckLz77;
	CComboBox m_ComboBgm;
	CComboBox m_ComboSpcMap;
	CEdit m_EditClipX;
	CEdit m_EditClipY;
	CEdit m_EditBeginX;
	CEdit m_EditBeginY;
	CEdit m_EditBckMov;
	CEdit m_EditX36;
	CEdit m_EditX54;
	//CComboBox m_ComboWind;
	//CComboBox m_ComboGraAni;
	CButton m_CheckX3E;
	CButton m_CheckX55;
	CButton m_CheckBckRoll;
	CButton m_AllowKeyControl;
	CEdit m_EditPltFrt;
	CEdit m_EditTillFrt;
	CEdit m_EditGraMapFrt;
	CComboBox m_ComboGraFrt;
	CEdit m_EditPltBck;
	CEdit m_EditTillBck;
	CEdit m_EditGraMapBck;
	CComboBox m_ComboGraBck;

	CDC m_TmpDC,m_TmpDC2,m_TmpDC3;
	CBitmap m_BmpMap;
	CBitmap m_BmpMapCache;
	CBitmap m_BmpDet;
	CBitmap m_BmpDetMask;
	CBitmap m_BmpDetOut;
	CBitmap m_BmpGra;
	CBitmap m_BmpGraOut;
	CBitmap m_BmpNest;
	CBitmap m_BmpCache;

	
	void InitToolTipText();
	afx_msg void OnDestroy();

	HGLOBAL hRomResData;
	void *pRomRebuild;
	void LockRomResource();
	void UnlockRomResource();

	void ClearData();
	void Refresh();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonSave();
	void ReadRom(CFile& file);
	void WriteRom(CFile& file);
	CEdit m_EditFileName;
	CTreeCtrl m_Tree;
	HTREEITEM ti_rom;
	HTREEITEM ti_level[8];
	HTREEITEM ti_stage[8][8];
	HTREEITEM ti_step[STEP_INDEXING_COUNT];
	afx_msg void OnNMDblclkTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	u32 cur_step;
	void StepHeaderOut();
	void LoadGraLib();
	u32 gralib_len;
	void ResetGraLibScrollBar();
	CScrollBar m_ScrollBarGraLib;
	bool LockRefreshing;
	u32 StrToI(CString &str);
	
	afx_msg void OnBnClickedCheckLz77();
	CEdit m_EditGraAni;
	afx_msg void OnCbnSelchangeComboBgm();
	afx_msg void OnCbnSelchangeComboSpcmap();
	afx_msg void OnEnChangeEditClipX();
	afx_msg void OnEnChangeEditClipY();
	afx_msg void OnEnChangeEditBeginX();
	afx_msg void OnEnChangeEditBeginY();
	afx_msg void OnEnChangeEditBckMov();
	afx_msg void OnEnChangeEditUnk36();
	afx_msg void OnEnChangeEditUnk54();
	afx_msg void OnBnClickedCheckUnk3e();
	afx_msg void OnBnClickedCheckUnk55();
	afx_msg void OnBnClickedCheckBckroll();
	CScrollBar m_HScrollMap;
	CScrollBar m_VScrollMap;

	void PaintMap();
	void PresentMap(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CComboBox m_ComboMapChg;

	u16 cur_x,cur_y;
	u16 cur_sel;
	u16 cur_chg;
	
	BLENDFUNCTION BFunction;
	u8 AlphaValue;

	afx_msg void OnCbnSelchangeComboMapchg();
	CStatic m_StaticGridInfo;
	void UpdateGridInfo();
	void UpdateLibInfo();
	void DrawGrid();

	void PaintGraLib();
	void PresentGraLib(CDC* pDC);
	bool MouseLState;
	CStatic m_StaticLibInfo;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CButton m_ButtonNew;
	CButton m_ButtonOpen;
	CButton m_ButtonSave;
	CButton m_ButtonSaveAs;
	afx_msg void OnBnClickedButtonSaveTest();
	afx_msg void OnBnClickedButtonSaveas();
	CButton m_CheckNest;
	afx_msg void OnBnClickedCheckNest();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CProgressCtrl m_Progress;
	afx_msg void OnBnClickedButtonSaveFinal();
	afx_msg void OnBnClickedButtonResize();
	CButton m_ButtonResize;
	CButton m_ButtonSaveFinal;
	CButton m_CheckNestAuto;
	afx_msg void OnBnClickedButtonDoor();
	
	afx_msg void OnEnChangeEditPltFrt();
	afx_msg void OnEnChangeEditPltBck();
	afx_msg void OnEnChangeEditTillFrt();
	afx_msg void OnEnChangeEditTillBck();
	afx_msg void OnEnChangeEditGramapFrt();
	afx_msg void OnEnChangeEditGramapBck();
	afx_msg void OnEnChangeEditGraAni();
	afx_msg void OnBnClickedClearMap();
	void PaintMapTile(int x, int y);
	BOOL PreTranslateMessage(MSG* pMsg);
};
