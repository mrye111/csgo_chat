#pragma once
#include <windows.h>
#include <iostream>
#include <Tlhelp32.h>
#include "Offsets.h"
using namespace std;
//游戏内存数据
class Memory
{
	public:
		HWND hWnd;
		HANDLE hSS;//快照句柄
		MODULEENTRY32 mEntry;//保存模块句柄信息
		void Setup();

		//获取进程ID
		DWORD GetProcessId(const wchar_t* windowsName);

		//获取模块
		MODULEENTRY32 GetModule(DWORD pID,const wchar_t* moduleName);

		void GetModules();

		//封装自己的内存读写函数
		template<typename T>
		T ReadMeory(DWORD32 adress) {
			T buffer;
			// 调用 Windows API 函数 ReadProcessMemory，从指定进程的内存中读取数据
			ReadProcessMemory(
				offsets.hProcess,      // HANDLE 类型，目标进程的句柄，用于标识要读取的进程
				(LPCVOID)adress,       // LPCVOID 类型，指向目标进程内存中的起始地址（需要强制转换为const void*）
				&buffer,               // LPVOID 类型，指向接收读取数据的缓冲区地址
				sizeof(buffer),        // SIZE_T 类型，指定要读取的字节数，这里使用缓冲区的大小
				NULL                   // SIZE_T* 类型，指向接收实际读取字节数的变量，可选，此处为NULL表示不关心实际读取的字节数
			);
			return buffer;
		};
		
		template<typename T>
		void WriteMemory(DWORD32 adress, T data) {
			WriteProcessMemory(
				offsets.hProcess,      // HANDLE 类型，目标进程的句柄，用于标识要读取的进程
				(LPVOID)adress,       // LPVOID 类型，指向目标进程内存中的起始地址（需要强制转换为const void*）
				&data,               // LPVOID 类型，指向接收写入数据的缓冲区地址
				sizeof(data),        // SIZE_T 类型，指定要写入的字节数，这里使用缓冲区的大小
				NULL                   // SIZE_T* 类型，指向接收实际写入字节数的变量，可选，此处为NULL表示不关心实际写入的字节数
			);
		};

		//读取人物骨骼
		void ReadBone(DWORD64 dwEntity, int nBoneIndex, Vec3& bonePos) {
			//通过偏移找到骨骼基址
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

