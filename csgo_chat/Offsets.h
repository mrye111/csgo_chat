#pragma once
#include <windows.h>
using namespace std;

//��Ϸ��Ϣ
class Offsets{
public:

	//��������ƫ��
	DWORD64 dw_EntityList = 0x189A280;
	//��������ƫ��2
	DWORD64 dw_EntityList2 = 0x01987A98;
	//��������ƫ��
	DWORD64 dw_LocalPlayer = 0x188CF70;
	//�������ƫ��
	DWORD64 dw_ViewMatrix = 0x1AA45F0;

	//����Ѫ��ƫ��
	DWORD64 m_iHealth = 0xAB4;
	//������Ӫƫ��
	DWORD64 m_iTeamNum = 0xE68;
	//����λ��ƫ��
	DWORD64 m_fPos = 0xDB8;


	HWND hWnd = 0;//��Ϸ���ھ��
	HWND hProcess = 0;//���̾��
	DWORD processId = 0;//����ID

	DWORD64 clientBase = 0;//ģ���ַ

};
extern Offsets offsets;