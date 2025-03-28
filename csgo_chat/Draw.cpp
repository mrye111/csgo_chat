#include "Draw.h"
#include "Structures.h"
#include "Offsets.h"
Draw draw;

void Draw::GetWindowInfo()
{
    //ȡ�ͻ���������Ϣ����������������
    GetClientRect(offsets.hWnd,&rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    // ȡ�ⴰ����Ϣ
    GetWindowRect(offsets.hWnd, &rectEx);
    widthEx = rectEx.right - rectEx.left;
    heightEx = rectEx.bottom - rectEx.top;


}


BOOL Draw::WorldToScreen(Vec3& wordPos, Vec2& screenPos)
{
    //��Ϸ��ά����
    float matrix[4][4];
    //��������ת��������
    Vec4 clipPos;
    clipPos.x = matrix[0][0] * clipPos.x + matrix[0][1] * clipPos.y + matrix[0][2] * clipPos.z + matrix[0][3] * clipPos.w;
    clipPos.y = matrix[1][0] * clipPos.x + matrix[1][1] * clipPos.y + matrix[1][2] * clipPos.z + matrix[1][3] * clipPos.w;
    clipPos.z = matrix[2][0] * clipPos.x + matrix[2][1] * clipPos.y + matrix[2][2] * clipPos.z + matrix[2][3] * clipPos.w;
    clipPos.w = matrix[3][0] * clipPos.x + matrix[3][1] * clipPos.y + matrix[3][2] * clipPos.z + matrix[3][3] * clipPos.w;
    //�����Ļ����С��0.01f��������Ļ�� 
    if (clipPos.w < 0.01f) return false;

    //��������תNDC����
    Vec3 NDC;
    NDC.x = clipPos.x / clipPos.w;
    NDC.y = clipPos.y / clipPos.w;
    NDC.z = clipPos.z / clipPos.w;

    //NDC����ת��Ļ����
    screenPos.x = (width / 2) + (width / 2) * NDC.x;
    screenPos.y = (height / 2) - (height / 2) * NDC.y;

    return true;
}

