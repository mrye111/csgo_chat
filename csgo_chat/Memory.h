#pragma once
#include <windows.h>
#include "Offsets.h"
using namespace std;
//��Ϸ�ڴ�����
class Memory
{
	public:
		HWND hWnd;
		void Setup();
};
extern Memory mem;

