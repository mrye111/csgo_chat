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

		//��ȡ�������
		void ReadBone(DWORD64 dwEntity, int nBoneIndex, Vec3& bonePos) {
			//ͨ��ƫ���ҵ�������ַ
			DWORD64 tmp, tmp1, tmp2, tmp3 ,tmp4, tmp5,dwBoneMatrix;
			tmp = 0x00;
			tmp1 = 0X18;
			tmp2 = 0xD8;
			tmp3 = 0x10;
			tmp4 = 0xD8;
			tmp5 = 0x00;
			ReadMeory<DWORD64>(dwEntity + tmp);
			ReadMeory<DWORD64>(tmp + tmp1);
			ReadMeory<DWORD64>(tmp + tmp2);
			ReadMeory<DWORD64>(tmp + tmp3);
			ReadMeory<DWORD64>(tmp + tmp4);
			dwBoneMatrix = ReadMeory<DWORD64>(tmp + tmp5);

			bonePos.x = ReadMeory<float>(dwBoneMatrix + nBoneIndex * 0x30 + 0xC);
			bonePos.y = ReadMeory<float>(dwBoneMatrix + nBoneIndex * 0x30 + 0xC);
			bonePos.z = ReadMeory<float>(dwBoneMatrix + nBoneIndex * 0x30 + 0xC);

		

		}
		
};
extern Memory mem;

