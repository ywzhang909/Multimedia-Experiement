
// SpeechExp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSpeechExpApp: 
// �йش����ʵ�֣������ SpeechExp.cpp
//

class CSpeechExpApp : public CWinApp
{
public:
	CSpeechExpApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSpeechExpApp theApp;