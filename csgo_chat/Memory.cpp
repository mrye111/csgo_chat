#include "Memory.h"
Memory mem;


MODULEENTRY32 Memory::GetModule(DWORD pID, const wchar_t* moduleName)
{
	//��ʼ����С
	mEntry.dwSize = sizeof(MODULEENTRY32);
	// ���� Windows API ���� CreateToolhelp32Snapshot������ָ�����͵�ϵͳ��Դ����
	hSS = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,   // DWORD ���ͣ�ָ��Ҫ�����Ŀ������ͣ��˴���ʾģ�����
		pID                     // DWORD ���ͣ������ʾ���հ������н���
	);

	// �жϿ��վ���Ƿ���Ч
	if (hSS != INVALID_HANDLE_VALUE) {
		if (Module32First(hSS,&mEntry)) {
			do
			{
				//�ж�ģ�����Ƿ�ƥ��
				if (!wcscmp((const wchar_t*)mEntry.szModule, moduleName)) {
					break;
				}
			} while (Module32Next(hSS,&mEntry));
		}
		CloseHandle(hSS); 
	}

	return mEntry;
}

DWORD Memory::GetProcessId(const wchar_t* windowsName)
{
	DWORD pId = 0;
	do
	{
		//�����ȴ򿪸����������Ϸ
		offsets.hWnd = FindWindow(NULL, windowsName);
		Sleep(50); 
	} while (!offsets.hWnd);

	// ���� Windows API ���� GetWindowThreadProcessId����ȡ��ָ�����ڹ������߳� ID �ͽ��� ID
	GetWindowThreadProcessId(
		offsets.hWnd,         // HWND ���ͣ�Ŀ�괰�ڵľ�������ڱ�ʶҪ��ѯ�Ĵ���
		&pId                  // LPDWORD ���ͣ�ָ�� DWORD ������ָ�룬���ڽ��մ����������̵Ľ��� ID
	);
	cout << "[ *] CS2 ProcessID found: " <<  pId  << endl;
	return pId;
}



void Memory::GetModules()
{
	do
	{
		//��ȡģ���ַ
		offsets.clientBase = (DWORD64)GetModule(offsets.processId, L"client.dll").modBaseAddr;
		Sleep(50);
	} while (!offsets.clientBase);
	
}


void Memory::Setup()
{
	//��ȡ����ID
	offsets.processId = GetProcessId(L"Counter-Strike 2");
	//��ȡ���̾��
	offsets.hProcess = (HWND) OpenProcess(PROCESS_ALL_ACCESS,FALSE,offsets.processId);//��ȡ�����ù���ԱȨ�����г��� X86�ĳ�X64
	//��ȡCSGO���ھ�� VS���ߡ�Spy++���������ɻ��
	offsets.hWnd = FindWindow(NULL,L"Counter-Strike 2");

	if (offsets.hProcess) {
		cout << "[ *] CS2 ProcessID found: " << offsets.hProcess << endl;
	}
	else
	{
		cout << "[ *] CS2 ProcessID found: " << offsets.hProcess << endl;

	}
}
