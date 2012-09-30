#pragma once
#include "afxwin.h"
#include "MapDataType.h"

// CDlgObj 对话框

class CDlgObj : public CDialog
{
	DECLARE_DYNAMIC(CDlgObj)

public:
	CDlgObj(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgObj();

// 对话框数据
	enum { IDD = IDD_OBJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	CScrollBar m_HScroll;
	CScrollBar m_VScroll;
	CBitmap* m_pBmpBck;
	CBitmap m_BmpBck;
	CBitmap m_BmpOut;
	CDC *m_pTmpDC,*m_pTmpDC2;

	u32 width,height;

	OBJ_DATA_EX** pobjlist;
	u16* pcount;

	u16* pobj_vertical;

	void PresentObj(CDC* pDC);
	void DrawObj(u16 x,u16 y);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();

	u16 cur_obj;
	u16 cur_subobj;
	bool moving;
	s16 mdx,mdy;
	bool copymov;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CButton m_CheckGridAlign;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnDestroy();
	void ObjSort(bool l2r,bool u2d,bool vert);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	CButton m_CheckObjVertical;
};
