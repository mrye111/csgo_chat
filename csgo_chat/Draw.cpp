#include "Draw.h"
#include "Structures.h"
#include "Offsets.h"
Draw draw;

void Draw::GetWindowInfo()
{
    //取客户区窗口信息（不包含标题栏）
    GetClientRect(offsets.hWnd,&rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    // 取外窗口信息
    GetWindowRect(offsets.hWnd, &rectEx);
    widthEx = rectEx.right - rectEx.left;
    heightEx = rectEx.bottom - rectEx.top;


}


BOOL Draw::WorldToScreen(Vec3& wordPos, Vec2& screenPos)
{

    GetWindowInfo();
    //游戏四维矩阵
    float matrix[4][4];

    //二进制64位读取游戏四维矩阵 4*4*4  
    ReadProcessMemory(offsets.hProcess, (LPCVOID)(offsets.clientBase + offsets.dw_ViewMatrix), matrix, 64, NULL);
    //世界坐标转剪辑坐标
    Vec4 clipPos;
    clipPos.x = matrix[0][0] * wordPos.x + matrix[0][1] * wordPos.y + matrix[0][2] * wordPos.z + matrix[0][3];
    clipPos.y = matrix[1][0] * wordPos.x + matrix[1][1] * wordPos.y + matrix[1][2] * wordPos.z + matrix[1][3];
    clipPos.z = matrix[2][0] * wordPos.x + matrix[2][1] * wordPos.y + matrix[2][2] * wordPos.z + matrix[2][3];
    clipPos.w = matrix[3][0] * wordPos.x + matrix[3][1] * wordPos.y + matrix[3][2] * wordPos.z + matrix[3][3];
    //如果屏幕坐标小于0.01f，则不在屏幕内 
    if (clipPos.w < 0.01f) return false;

    //剪辑坐标转NDC坐标
    Vec3 NDC;
    NDC.x = clipPos.x / clipPos.w;
    NDC.y = clipPos.y / clipPos.w;
    NDC.z = clipPos.z / clipPos.w;

    //NDC坐标转屏幕坐标
    screenPos.x = (width / 2) + (width / 2) * NDC.x;
    screenPos.y = (height / 2) - (height / 2) * NDC.y;

    return true;
}

