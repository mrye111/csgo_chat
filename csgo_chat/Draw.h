#pragma once
#include <windows.h>
#include "Structures.h"
using namespace std;
//绘制类：游戏窗口相关信息
class Draw
{	
public:
	RECT rectGame;//cs2游戏窗口大小信息
	HWND hExWnd;//蒙版窗口句柄

	RECT rect;// 内窗口分辨率
	int width, height; //内窗口宽高

	RECT rectEx;// 外窗口分辨率
	int widthEx, heightEx; //外窗口宽高

	BOOL WorldToScreen(Vec3& wordPos, Vec2& screenPos);
	void GetWindowInfo();//获取窗口信息
};
extern Draw draw;
