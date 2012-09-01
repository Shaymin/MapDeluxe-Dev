//主入口
//编写:wwylele

#include "stdafx.h"
#include "MapDeluxe.h"
#include "MapDeluxeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib,"Version.lib")

// CMapDeluxeApp

BEGIN_MESSAGE_MAP(CMapDeluxeApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMapDeluxeApp 构造

CMapDeluxeApp::CMapDeluxeApp()
{
}


// 唯一的一个 CMapDeluxeApp 对象

CMapDeluxeApp theApp_MapDeluxe_by_wwylele_Why_do_I_write_such_a_fucking_long_name;


// CMapDeluxeApp 初始化

BOOL CMapDeluxeApp::InitInstance()
{

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	CMapDeluxeDlg* dlg=new CMapDeluxeDlg;
	m_pMainWnd = dlg;
	dlg->DoModal();
	delete dlg;


	return FALSE;
}

CString GetProductVersion()  
 {  
    int nMaxPathName = 4096; // Max length of file name/path  
     TCHAR *pBuffer;  
     UINT nItemLength;  
    void* pData, *lpBuffer;  
    CString sVersion;  
     DWORD dwInfoSize, dwHandle;  
    VS_FIXEDFILEINFO *pFileInfo;  
     // Get the file path and name  
    pBuffer = new TCHAR[nMaxPathName];  
    GetModuleFileName(NULL, pBuffer, nMaxPathName-1);  
  
     // Get File Version Info size  
     dwInfoSize = GetFileVersionInfoSize(pBuffer,  &dwHandle);  
     if(dwInfoSize > 0)  
    {  
       pData = new TCHAR[dwInfoSize];  
         if(GetFileVersionInfo(pBuffer, dwHandle,  dwInfoSize, pData))  
             if(VerQueryValue(pData,  _T("\\"), &lpBuffer,  &nItemLength))  
             {  
                pFileInfo = (VS_FIXEDFILEINFO*)lpBuffer;  
                sVersion.Format(_T("%d.%d.%d.%d"),   
                  pFileInfo->dwProductVersionMS >> 16,   
                 pFileInfo->dwProductVersionMS & 0xFFFF,   
                 pFileInfo->dwProductVersionLS >> 16,  
                  pFileInfo->dwProductVersionLS & 0xFFFF);      
                 // Calculate the product version as a number, you can delete the next statement if you don't need it.  
                 DWORD dwProductVersion =    (pFileInfo->dwProductVersionMS >> 16)   * 1000 +  
                                            (pFileInfo->dwProductVersionMS & 0xFFFF) * 100 +  
                                             (pFileInfo->dwProductVersionLS >> 16)   * 10 +  
                                             (pFileInfo->dwProductVersionLS & 0xFFFF) * 1;  
    
            }  
         // Delete the data buffer  
        delete [] pData;  
     }  
     // Get rid of the allocated string buffer  
     delete [] pBuffer;  
     return sVersion;  
}  
