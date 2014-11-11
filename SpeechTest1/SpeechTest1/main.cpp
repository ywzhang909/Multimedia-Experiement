#pragma warning(disable:4996)

#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>

#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance��Ҫ����ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib

int main(int argc, char* argv [])
{
	ISpVoice * pVoice = NULL;

	//COM ��ʼ���� 
	if (FAILED(::CoInitialize(NULL)))
		return FALSE;

	//��ȡ ISpVoice �ӿڣ� 
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice,
		(void **) &pVoice);
	if (SUCCEEDED(hr))
	{
		hr = pVoice->Speak(L"Hello world", 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}

	//ǧ��Ҫ���ǣ� 
	::CoUninitialize();
	return TRUE;
}