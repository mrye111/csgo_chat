#pragma once
#include <windows.h>
using namespace std;

//游戏信息
class Offsets{
public:
	HWND hWnd = 0;//游戏窗口句柄
	HWND hProcess = 0;//进程句柄
	DWORD processId = 0;//进程ID

	DWORD64 clientBase = 0;//模块地址

};
extern Offsets offsets;