
// SpeechExpDlg.h : 头文件
//

#pragma once

#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>

#define WM_RECOEVENT WM_USER+102
#define GID_CMD_GR 333333

#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance需要调用ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib

// CSpeechExpDlg 对话框
class CSpeechExpDlg : public CDialogEx
{
// 构造
public:
	CSpeechExpDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SPEECHEXP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnSREvent(WPARAM, LPARAM);
public:
	CComPtr<ISpRecognizer> m_cpRecognizer;
	CComPtr<ISpRecoContext> m_cpRecoCtxt;
	CComPtr<ISpRecoGrammar> m_cpRecoGrammar;


	void GetText(WCHAR **ppszCoMemText, ULONG ulStart = 0, ULONG nlCount = -1);
};
