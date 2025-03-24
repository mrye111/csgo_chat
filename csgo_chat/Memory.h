#pragma once
#include <windows.h>
#include "Offsets.h"
using namespace std;
//游戏内存数据
class Memory
{
	public:
		HWND hWnd;
		void Setup();
};
extern Memory mem;

