#pragma once
#include <windows.h>
using namespace std;
//绘制类：游戏窗口相关信息
class Draw
{	
public:
	RECT rectGame;//cs2游戏窗口大小信息
	HWND hExWnd;//蒙版窗口句柄
};
extern Draw draw;
