#pragma once
#include <windows.h>
using namespace std;

//游戏信息
class Offsets{
public:

	//人物数组偏移
	DWORD64 dw_EntityList = 0x189A280;
	//人物数组偏移2
	DWORD64 dw_EntityList2 = 0x01987A98;
	//人物自身偏移
	DWORD64 dw_LocalPlayer = 0x188CF70;
	//人物矩阵偏移
	DWORD64 dw_ViewMatrix = 0x1AA45F0;

	//人物血量偏移
	DWORD64 m_iHealth = 0xAB4;
	//人物阵营偏移
	DWORD64 m_iTeamNum = 0xE68;
	//人物位置偏移
	DWORD64 m_fPos = 0xDB8;


	HWND hWnd = 0;//游戏窗口句柄
	HWND hProcess = 0;//进程句柄
	DWORD processId = 0;//进程ID

	DWORD64 clientBase = 0;//模块地址

};
extern Offsets offsets;