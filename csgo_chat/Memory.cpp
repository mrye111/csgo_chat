#include "Memory.h"
Memory mem;

void Memory::Setup()
{
	//��ȡCSGO���ھ�� VS���ߡ�Spy++���������ɻ��
   offsets.hWnd = FindWindow(NULL,L"Counter - Strike 2");
}
