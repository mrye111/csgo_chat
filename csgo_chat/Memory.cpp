#include "Memory.h"
Memory mem;


MODULEENTRY32 Memory::GetModule(DWORD pID, const wchar_t* moduleName)
{
	//初始化大小
	mEntry.dwSize = sizeof(MODULEENTRY32);
	// 调用 Windows API 函数 CreateToolhelp32Snapshot，创建指定类型的系统资源快照
	hSS = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,   // DWORD 类型，指定要创建的快照类型，此处表示模块快照
		pID                     // DWORD 类型，，则表示快照包含所有进程
	);

	// 判断快照句柄是否有效
	if (hSS != INVALID_HANDLE_VALUE) {
		if (Module32First(hSS,&mEntry)) {
			do
			{
				//判断模块名是否匹配
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
		//避免先打开辅助，后打开游戏
		offsets.hWnd = FindWindow(NULL, windowsName);
		Sleep(50); 
	} while (!offsets.hWnd);

	// 调用 Windows API 函数 GetWindowThreadProcessId，获取与指定窗口关联的线程 ID 和进程 ID
	GetWindowThreadProcessId(
		offsets.hWnd,         // HWND 类型，目标窗口的句柄，用于标识要查询的窗口
		&pId                  // LPDWORD 类型，指向 DWORD 变量的指针，用于接收窗口所属进程的进程 ID
	);
	cout << "[ *] CS2 ProcessID found: " <<  pId  << endl;
	return pId;
}



void Memory::GetModules()
{
	do
	{
		//获取模块地址
		offsets.clientBase = (DWORD64)GetModule(offsets.processId, L"client.dll").modBaseAddr;
		Sleep(50);
	} while (!offsets.clientBase);
	
}


void Memory::Setup()
{
	//获取进程ID
	offsets.processId = GetProcessId(L"Counter-Strike 2");
	//获取进程句柄
	offsets.hProcess = (HWND) OpenProcess(PROCESS_ALL_ACCESS,FALSE,offsets.processId);//获取不到用管理员权限运行程序 X86改成X64
	//获取CSGO窗口句柄 VS工具→Spy++→搜索即可获得
	offsets.hWnd = FindWindow(NULL,L"Counter-Strike 2");

	if (offsets.hProcess) {
		cout << "[ *] CS2 ProcessID found: " << offsets.hProcess << endl;
	}
	else
	{
		cout << "[ *] CS2 ProcessID found: " << offsets.hProcess << endl;

	}
}
