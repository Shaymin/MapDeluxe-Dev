#pragma once


// CDlgRomImage 对话框

class CDlgRomImage : public CDialog
{
	DECLARE_DYNAMIC(CDlgRomImage)

public:
	CDlgRomImage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRomImage();

// 对话框数据
	enum { IDD = IDD_ROM_IMAGE };

	u16 rom_image[240*160];

	void PaintImage(CDC* pDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonImpRomImage();
};
