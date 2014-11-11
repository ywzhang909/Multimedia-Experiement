
// SpeechExpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SpeechExp.h"
#include "SpeechExpDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSpeechExpDlg 对话框



CSpeechExpDlg::CSpeechExpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpeechExpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpeechExpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSpeechExpDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSpeechExpDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSpeechExpDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_RECOEVENT, &CSpeechExpDlg::OnSREvent)
END_MESSAGE_MAP()


// CSpeechExpDlg 消息处理程序

BOOL CSpeechExpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSpeechExpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSpeechExpDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSpeechExpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSpeechExpDlg::OnBnClickedOk()
{
	ISpVoice * pVoice = NULL;

	//COM 初始化： 
	if (FAILED(::CoInitialize(NULL)))
	
		return;

	//获取 ISpVoice 接口： 
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice,
		(void **) &pVoice);
	if (SUCCEEDED(hr))
	{
		CString str;
		GetDlgItem(IDC_EDIT1)->GetWindowText(str);
		hr = pVoice->Speak(str, 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}

	//千万不要忘记： 
	::CoUninitialize();
}


void CSpeechExpDlg::GetText(WCHAR **ppszCoMemText, ULONG ulStart, ULONG nlCount)
{
	USES_CONVERSION;
	CSpEvent event;

	// Process all of the recognition events 
	while (event.GetFrom(m_cpRecoCtxt) == S_OK)
	{
		switch (event.eEventId)
		{
		case SPEI_RECOGNITION:
			// There may be multiple recognition results, so get all of them 
		{
			HRESULT hr = S_OK;
			if (nlCount == -1)
				event.RecoResult()->GetText(SP_GETWHOLEPHRASE,
				SP_GETWHOLEPHRASE, TRUE, ppszCoMemText, NULL);
			else
			{
				ASSERT(nlCount > 0);
				event.RecoResult()->GetText(ulStart, nlCount, FALSE,
					ppszCoMemText, NULL);
			}
		}
			break;
		}
	}
}

LRESULT CSpeechExpDlg::OnSREvent(WPARAM, LPARAM)
{
	WCHAR *pwzText;
	GetText(&pwzText);
	GetDlgItem(IDC_EDIT1)->SetWindowTextW(CString(pwzText));
	UpdateData(FALSE);

	return 0;
}

void CSpeechExpDlg::OnBnClickedCancel()
{

	if (FAILED(::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		return;
	}
	HRESULT hr = S_OK;
	hr = m_cpRecognizer.CoCreateInstance(CLSID_SpSharedRecoContext);//Share

	if (SUCCEEDED(hr))
	{
		hr = m_cpRecognizer->CreateRecoContext(&m_cpRecoCtxt);
	}
	
	if (SUCCEEDED(hr))
	{
		hr = m_cpRecoCtxt->SetNotifyWindowMessage(m_hWnd, WM_RECOEVENT, 0, 0);
	}

	if (SUCCEEDED(hr))
	{
		ULONGLONG ullMyEvent = SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_HYPOTHESIS);
		hr = m_cpRecoCtxt->SetInterest(ullMyEvent, ullMyEvent);
	}

	CComPtr<ISpAudio> m_cpAudio;
	hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &m_cpAudio);
	hr = m_cpRecognizer->SetInput(m_cpAudio, TRUE);
	hr = m_cpRecognizer->SetRecoState(SPRST_ACTIVE);

	hr = m_cpRecoCtxt->CreateGrammar(NULL, &m_cpRecoGrammar);

	if (SUCCEEDED(hr))
	{
		hr = m_cpRecoGrammar->LoadDictation(NULL, SPLO_STATIC);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_cpRecoGrammar->SetDictationState(SPRS_ACTIVE);
	}

	if (SUCCEEDED(hr))
	{
		m_cpRecoGrammar.Release();
	}
	
	//千万不要忘记： 
	/*
	if (m_cpRecoGrammar)
	{
	m_cpRecoGrammar.Release();
	}
	if (m_cpRecoCtxt)
	{
	m_cpRecoCtxt.Release();
	}
	if (m_cpRecognizer)
	{
	m_cpRecognizer.Release();
	}
	 */
	::CoUninitialize();
}

