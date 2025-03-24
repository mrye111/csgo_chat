#include <iostream>
#include <Windows.h>
#include "Memory.h"
using namespace std;

void Init() {
	mem.Setup();
}

int main()
{
	Init();
	//设置控制台标题
	SetConsoleTitle(L"游戏辅助工具");
	HWND console = GetConsoleWindow();
	RECT r;
	/*
	# 获取窗口位置和尺寸
	1.窗口句柄 GetConsoleWindow()获取窗口句柄
	2.返回值类型 RECT
	3.返回值变量名
	*/

	GetWindowRect(console, &r);
	/*
	# 设置控制台初始位置
	1.窗口句柄 GetConsoleWindow()获取窗口句柄
	2.x坐标
	3.y坐标
	4.宽度
	5.高度
	6.是否重绘
	*/
	MoveWindow(console, r.left, r.right, 500, 700, TRUE);
	cout << "\n CS2辅助Demo" << endl;
	cout << "\n ------------------------------------------" << endl;
	cout << "\n CS2辅助Demo" << endl;
}