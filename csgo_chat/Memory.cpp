#include "Memory.h"
Memory mem;

void Memory::Setup()
{
	//获取CSGO窗口句柄 VS工具→Spy++→搜索即可获得
   offsets.hWnd = FindWindow(NULL,L"Counter - Strike 2");
}
