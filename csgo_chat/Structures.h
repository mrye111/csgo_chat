#pragma once
#include "Windows.h"
using namespace std;

//二维屏幕坐标结构体
struct Vec2
{
    float x = 0;
    float y = 0;
};



//三维世界坐标结构体
struct Vec3
{
    float x = 0;
    float y = 0;
    float z = 0;
};

//四维结构体
struct Vec4
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;

};

//头部骨骼坐标
#define BONE_HEAD 6