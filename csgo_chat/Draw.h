#pragma once
#include <windows.h>
#include "Structures.h"
using namespace std;
//�����ࣺ��Ϸ���������Ϣ
class Draw
{	
public:
	RECT rectGame;//cs2��Ϸ���ڴ�С��Ϣ
	HWND hExWnd;//�ɰ洰�ھ��

	RECT rect;// �ڴ��ڷֱ���
	int width, height; //�ڴ��ڿ��

	RECT rectEx;// �ⴰ�ڷֱ���
	int widthEx, heightEx; //�ⴰ�ڿ��

	BOOL WorldToScreen(Vec3& wordPos, Vec2& screenPos);
	void GetWindowInfo();//��ȡ������Ϣ
};
extern Draw draw;
