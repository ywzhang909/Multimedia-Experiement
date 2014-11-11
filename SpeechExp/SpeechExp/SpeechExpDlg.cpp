
// SpeechExpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SpeechExp.h"
#include "SpeechExpDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSpeechExpDlg �Ի���



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


// CSpeechExpDlg ��Ϣ�������

BOOL CSpeechExpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSpeechExpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSpeechExpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSpeechExpDlg::OnBnClickedOk()
{
	ISpVoice * pVoice = NULL;

	//COM ��ʼ���� 
	if (FAILED(::CoInitialize(NULL)))
	
		return;

	//��ȡ ISpVoice �ӿڣ� 
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

	//ǧ��Ҫ���ǣ� 
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
	
	//ǧ��Ҫ���ǣ� 
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

