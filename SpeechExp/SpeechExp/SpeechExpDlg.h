
// SpeechExpDlg.h : ͷ�ļ�
//

#pragma once

#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>

#define WM_RECOEVENT WM_USER+102
#define GID_CMD_GR 333333

#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance��Ҫ����ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib

// CSpeechExpDlg �Ի���
class CSpeechExpDlg : public CDialogEx
{
// ����
public:
	CSpeechExpDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SPEECHEXP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
