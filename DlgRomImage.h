#pragma once


// CDlgRomImage �Ի���

class CDlgRomImage : public CDialog
{
	DECLARE_DYNAMIC(CDlgRomImage)

public:
	CDlgRomImage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRomImage();

// �Ի�������
	enum { IDD = IDD_ROM_IMAGE };

	u16 rom_image[240*160];

	void PaintImage(CDC* pDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonImpRomImage();
};
