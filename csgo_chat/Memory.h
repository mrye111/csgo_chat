#pragma once
#include <windows.h>
#include <iostream>
#include <Tlhelp32.h>
#include "Offsets.h"
using namespace std;
//��Ϸ�ڴ�����
class Memory
{
	public:
		HWND hWnd;
		HANDLE hSS;//���վ��
		MODULEENTRY32 mEntry;//����ģ������Ϣ
		void Setup();

		//��ȡ����ID
		DWORD GetProcessId(const wchar_t* windowsName);

		//��ȡģ��
		MODULEENTRY32 GetModule(DWORD pID,const wchar_t* moduleName);

		void GetModules();

		//��װ�Լ����ڴ��д����
		template<typename T>
		T ReadMeory(DWORD32 adress) {
			T buffer;
			// ���� Windows API ���� ReadProcessMemory����ָ�����̵��ڴ��ж�ȡ����
			ReadProcessMemory(
				offsets.hProcess,      // HANDLE ���ͣ�Ŀ����̵ľ�������ڱ�ʶҪ��ȡ�Ľ���
				(LPCVOID)adress,       // LPCVOID ���ͣ�ָ��Ŀ������ڴ��е���ʼ��ַ����Ҫǿ��ת��Ϊconst void*��
				&buffer,               // LPVOID ���ͣ�ָ����ն�ȡ���ݵĻ�������ַ
				sizeof(buffer),        // SIZE_T ���ͣ�ָ��Ҫ��ȡ���ֽ���������ʹ�û������Ĵ�С
				NULL                   // SIZE_T* ���ͣ�ָ�����ʵ�ʶ�ȡ�ֽ����ı�������ѡ���˴�ΪNULL��ʾ������ʵ�ʶ�ȡ���ֽ���
			);
			return buffer;
		};
		
		template<typename T>
		void WriteMemory(DWORD32 adress, T data) {
			WriteProcessMemory(
				offsets.hProcess,      // HANDLE ���ͣ�Ŀ����̵ľ�������ڱ�ʶҪ��ȡ�Ľ���
				(LPVOID)adress,       // LPVOID ���ͣ�ָ��Ŀ������ڴ��е���ʼ��ַ����Ҫǿ��ת��Ϊconst void*��
				&data,               // LPVOID ���ͣ�ָ�����д�����ݵĻ�������ַ
				sizeof(data),        // SIZE_T ���ͣ�ָ��Ҫд����ֽ���������ʹ�û������Ĵ�С
				NULL                   // SIZE_T* ���ͣ�ָ�����ʵ��д���ֽ����ı�������ѡ���˴�ΪNULL��ʾ������ʵ��д����ֽ���
			);
		};

		
};
extern Memory mem;

