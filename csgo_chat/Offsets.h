#pragma once
#include <windows.h>
using namespace std;

//��Ϸ��Ϣ
class Offsets{
public:
	HWND hWnd = 0;//��Ϸ���ھ��
	HWND hProcess = 0;//���̾��
	DWORD processId = 0;//����ID

	DWORD64 clientBase = 0;//ģ���ַ

};
extern Offsets offsets;