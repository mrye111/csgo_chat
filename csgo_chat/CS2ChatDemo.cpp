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
	//���ÿ���̨����
	SetConsoleTitle(L"��Ϸ��������");
	HWND console = GetConsoleWindow();
	RECT r;
	/*
	# ��ȡ����λ�úͳߴ�
	1.���ھ�� GetConsoleWindow()��ȡ���ھ��
	2.����ֵ���� RECT
	3.����ֵ������
	*/

	GetWindowRect(console, &r);
	/*
	# ���ÿ���̨��ʼλ��
	1.���ھ�� GetConsoleWindow()��ȡ���ھ��
	2.x����
	3.y����
	4.���
	5.�߶�
	6.�Ƿ��ػ�
	*/
	MoveWindow(console, r.left, r.right, 500, 700, TRUE);
	cout << "\n CS2����Demo" << endl;
	cout << "\n ------------------------------------------" << endl;
	cout << "\n CS2����Demo" << endl;
}