#include "ExWindow.h"
#include"Memory.h"



void Loop()
{
    //��ȡ���ﱾ������
    DWORD64 LocalPlayer = mem.ReadMeory<DWORD64>(offsets.clientBase + offsets.dw_LocalPlayer);
    //��ȡ��Ӫ��־λ
    DWORD64 LocalTeam = mem.ReadMeory<DWORD64>(LocalPlayer + offsets.m_iTeamNum);

    // ���������豸������(DC)��ͨ����ȡָ�����ڵ�DC��ʵ�ֻ�ͼ
    HDC hDc = GetDC(draw.hExWnd);

    // ������ˢ
    // GetStockObject ��ȡһ��Ԥ�����ϵͳ��ˢ(����ʹ�� BLACK_BRUSH)
    // ����ת��Ϊ HBRUSH ������ƥ��Ԥ������
    HBRUSH hHrush = (HBRUSH)GetStockObject(BLACK_BRUSH); // ʹ��Ĭ�ϵĺ�ɫ��ˢ

    // ʹ�ú�ɫ��ˢ����豸��������ָ���ľ�������(draw.rectEx)
    FillRect(hDc, &draw.rectEx, hHrush);

    // ������Դ��ͨ��ɾ����ˢ�������ͷ��ڴ�
    DeleteObject(hHrush);



    if (LocalPlayer)
    {
        //�������н�ɫ
        for (int i = 0; i < 32; i++)
        {
            //��ȡÿ����ҵĵ�ַ��ÿ����ַ8�ֽ�,����Ҫ����8
            DWORD64 Entity = mem.ReadMeory<DWORD64>(offsets.clientBase + offsets.dw_EntityList + i * 0x8);
            //�ж��Ƿ����Լ���������Լ��Ļ�������
            if (LocalPlayer == Entity) { continue; }
            //�ж��Ƿ�Ϊ�գ�Ϊ������
            if (Entity == 0) { continue; }

            //ʹ�����������Ľṹ��Vec3��Vec2�ṹ�壬���ڴ洢���������������Ļ����
            Vec3 entityPos3;//������������
            Vec2 entityPos2;//��Ļ����

            //ÿ��������ռ4�ֽڣ�����Ҫ+4������ȡxyz��ֵ
            entityPos3.x =  mem.ReadMeory<float>(Entity + offsets.m_fPos + 0x0);//x
            entityPos3.y =  mem.ReadMeory<float>(Entity + offsets.m_fPos + 0x4);//y
            entityPos3.z =  mem.ReadMeory<float>(Entity + offsets.m_fPos + 0x8);//z

            //��ȡ���˵���Ӫ������������BYTE��Ҫô��02CT,Ҫô��03C
            DWORD teamId = mem.ReadMeory<BYTE>(Entity + offsets.m_iTeamNum);
            //�ж������Ƿ�Ϊ��
            if (teamId != 2 && teamId != 3){ continue; }
            //�ж��Ƿ�Ϊ����
            if (teamId != LocalTeam) {
                //��ȡ����Ѫ��
                DWORD entityHealth = mem.ReadMeory<int>(Entity + offsets.m_iHealth);
                //�жϵ����Ƿ�������Ƿ�����ȷ��Ѫ����ֵ,������Ļ����ת���ɹ�
                if (0 < entityHealth && entityHealth <= 100 && draw.WorldToScreen(entityPos3, entityPos2))
                {
                    //��������Ĺ�����
                    Vec3 tmpBone3;
                    Vec2 tmpBone2;
                    for (int i = 0; i < 50; i++)
                    {   
                        DWORD64 other_Adress_Entity  = mem.ReadMeory<DWORD64>(offsets.clientBase + offsets.dw_EntityList2 + i * 0x8);
                        mem.ReadBone(other_Adress_Entity, i, tmpBone3);
                        wchar_t buffer[MAXBYTE];
                        if (draw.WorldToScreen(tmpBone3, tmpBone2)) {
                            TextOut(hDc, tmpBone2.x, tmpBone2.y, buffer, 2);
                        }
                    }
                }
            }


        }
    }
}

/*
1�����ע�ᴰ����
2����������
3����ʾ����
4�����´���
5��������Ϣѭ��
6��ʵ�ִ��ڹ��̺���
*/

/*
����һ�����ڹ��̺���(Window Procedure)�����ڴ�������Ϣ
LRESULT: ����ֵ���ͣ�ͨ���� long�����ڱ�ʾ������
CALLBACK: Windows API �ĵ���Լ������ʾ������ѭstdcall����Լ��
HWND hwnd: ���ھ������ʶ������Ϣ�Ĵ���
UINT uMsg: ��Ϣ���ͣ���ʾ��ǰ���յ�����Ϣ����WM_CREATE��WM_DESTROY�ȣ�
WPARAM wParam: ���Ӳ���1���������Ϣ��أ��ṩ������Ϣ
LPARAM lParam: ���Ӳ���2���������Ϣ��أ��ṩ������Ϣ
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //uMsg = WM_PRINT;
    switch (uMsg) // ������Ϣ���� uMsg ���з�֧����
    {
    case WM_PRINT:
        // ��������Ҫ����ӡ����Ƶ��豸�����ģ�DC��ʱ����
        // wParam: ָ��Ŀ���豸�����ĵľ�� (HDC)
        // lParam: ָ����ӡѡ���PRF_CHECKVISIBLE�ȱ�־��
        // ͨ�������Զ��崰�ڵĴ�ӡ��Ϊ��ĿǰΪ�գ�δʵ�־����߼�
        Loop();
        break;

    case WM_CREATE:
        // �ڴ��ڴ�����ɺ���ʾ֮ǰ����
        // wParam: δʹ��
        // lParam: ָ�� CREATESTRUCT �ṹ���ָ�룬�������ڴ����Ĳ�������λ�á���С�ȣ�
        // �����ڳ�ʼ��������Դ����ؼ����ڴ����ȣ���ĿǰΪ��
        break;

    case WM_DESTROY:
        // �����ڱ�����ʱ������ͨ���ڴ��ڹرպ����
        // wParam: δʹ��
        // lParam: δʹ��
        // ������������Դ�����ͷ��ڴ桢����״̬�ȣ���ĿǰΪ��
        DestroyWindow(hwnd);
        break;

    case WM_CLOSE:
        // ���û����Թرմ��ڣ������رհ�ť��ʱ����
        // wParam: δʹ��
        // lParam: δʹ��
        // Ĭ������»���� DestroyWindow �����ٴ��ڣ����ڴ����ȷ���߼�
        // ĿǰΪ�գ�δ��ֹĬ�Ϲر���Ϊ
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
        // ��������δ��ʽ�г�����Ϣ
        // ͨ����δ�������Ϣ���� DefWindowProc ������ȷ��ϵͳĬ����Ϊ
        // ĿǰΪ�գ�δʵ�־����߼�
        break;
    }
    // ע�⣺��ǰ����û�з���ֵ��ͨ��Ӧ���� DefWindowProc(hwnd, uMsg, wParam, lParam)
    // �Դ���δ��ʽ�������Ϣ�����������Ҫ����
}

void CreateExternalWindow()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    //��ȡ��Ϸ�������꣬������draw�ṹ����
    GetWindowRect(offsets.hWnd,&draw.rectGame);

	//��ƴ�����
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));//����ڴ�����ṹ�壬��ʼ��0
	wc.cbSize = sizeof(wc);	//���÷��
    wc.style = CS_HREDRAW | CS_VREDRAW; // ���ô��������ʽ
    // CS_HREDRAW: ������ˮƽ��С�ı�ʱ�������ػ���������
    // CS_VREDRAW: �����ڴ�ֱ��С�ı�ʱ�������ػ���������
    // ��������־���ȷ�������ڴ�С����ʱ�Զ�ˢ�£������Ӱ

    wc.lpfnWndProc = (WNDPROC)WindowProc; // ָ�����ڹ��̺�����ָ��
    // WindowProc �Ǵ�������Ϣ�Ļص���������WM_CREATE��WM_DESTROY�ȣ�
    // ����ת��Ϊ WNDPROC��ȷ�������ԣ�������ô������������Ϣ

    wc.hInstance = GetModuleHandle(NULL); // ��ȡ��ǰӦ�ó����ʵ�����
    // GetModuleHandle(NULL) ���ص�ǰ��ִ���ļ���ģ����
    // ���ڹ�����������Ӧ�ó���ʵ����ͨ���� WinMain �е���

    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ���ô��ڵ�Ĭ�Ϲ��
    // LoadCursor ��ϵͳ��Դ���ع�꣬NULL ��ʾʹ��ϵͳԤ������
    // IDC_ARROW �Ǳ�׼�ļ�ͷ��꣬�û������ͣ�ڴ�����ʱ��ʾ

    wc.hbrBackground = (HBRUSH)RGB(0, 0, 0); // ���ô��ڵı�����ˢ
    // RGB(0, 0, 0) �����ɫ������ɫ����=0, ��=0, ��=0��
    // ����ת��Ϊ HBRUSH��ָ�����ڿͻ����ı���ɫΪ��ɫ

    wc.lpszClassName = L"ExternalWindow"; // ���崰���������
    // L"ExternalWindow" ��һ�����ַ��ַ�����Unicode������Ϊ�������Ψһ��ʶ��
    // ���ں�����������ʱ���øô����࣬����ȫ��Ψһ

    //ע�ᴰ����
    RegisterClassEx(&wc);

    //��������
    draw.hExWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, // ��չ������ʽ
        // WS_EX_TOPMOST: ����ʼ���ö�����ʾ���������ö�����֮��
        // WS_EX_TRANSPARENT: ����͸��������ɴ�͸���·����� 
        // WS_EX_LAYERED: ���÷ֲ㴰�ڣ�֧��͸���Ȼ� alpha ���Ч��

        wc.lpszClassName, // �����������
        // ʹ��֮ǰע��Ĵ������������� L"ExternalWindow"�������崰�ڵ���Ϊ������

        L"ExWindow", // ���ڱ���
        // ��ʾ�ڴ��ڱ��������ı����˴�Ϊ "ExWindow"�����ڵ������ڿ��ܲ��ɼ�

        WS_POPUP, // ������ʽ
        // WS_POPUP: ����һ���������ڣ��ޱ߿�ͱ��������ʺϸ������ڻ򸲸ǲ�

        draw.rectGame.left, // ���ڵ� x ���꣨���Ե��
        // ָ����������Ļ�ϵ�ˮƽ��ʼλ�ã�ȡ�� draw.rectGame ����߽�

        draw.rectGame.top, // ���ڵ� y ���꣨�ϱ�Ե��
        // ָ����������Ļ�ϵĴ�ֱ��ʼλ�ã�ȡ�� draw.rectGame ���ϱ߽�

        draw.rectGame.right - draw.rectGame.left, // ���ڿ��
        // ���㴰�ڿ�ȣ�ͨ�����ε��ұ߽��ȥ��߽�ó�

        draw.rectGame.bottom - draw.rectGame.top, // ���ڸ߶�
        // ���㴰�ڸ߶ȣ�ͨ�����ε��±߽��ȥ�ϱ߽�ó�

        NULL, // �����ھ��
        // NULL ��ʾ�ô���û�и����ڣ��Ƕ�������

        NULL, // �˵����
        // NULL ��ʾ���ڲ�ʹ�ò˵������ڵ�������ͨ������Ҫ

        wc.hInstance, // Ӧ�ó���ʵ�����
        // ָ���������ڵ�Ӧ�ó���ʵ����ͨ���봰����ע��ʱһ��

        NULL // ��������
        // NULL ��ʾ�����ݶ������ݸ����ڹ��̺����� WM_CREATE ��Ϣ
    );
    // û�д����ɹ�������
    if (draw.hExWnd == NULL)
    {
        return;
    }

    //����͸������
    SetLayeredWindowAttributes(
        draw.hExWnd, // Ŀ�괰�ڵľ��
        // draw.hExWnd ��Ҫ�������ԵĴ��ھ����ͨ���� CreateWindowEx ����
        // �����Ǵ��� WS_EX_LAYERED ��չ��ʽ�Ĵ���

        RGB(0, 0, 0), // ��ɫ����͸��ɫ��
        // RGB(0, 0, 0) ָ����ɫ��Ϊ͸��ɫ
        // ��ʹ�� LWA_COLORKEY ��־ʱ�������ɫƥ������ؽ���Ϊ͸��
        // �˴��� LWA_ALPHA һ��ʹ�ã�����Ч��ȡ���ڱ�־

        0, // Alpha ֵ��͸���ȣ�
        // ��Χ 0-255��0 ��ʾ��ȫ͸����255 ��ʾ��ȫ��͸��
        // �˴���Ϊ 0����� LWA_ALPHA ��־��ʹ����������ȫ͸��

        LWA_COLORKEY // ������־
        // LWA_ALPHA: ʹ�õ�����������Alpha ֵ�����ƴ��ڵ�����͸����
        // ע�⣺δʹ�� LWA_COLORKEY����˵ڶ�����������ɫ�����ڴ��������в�������
    );

    // ��ʾ����
    ShowWindow(draw.hExWnd, SW_SHOW);
    // ShowWindow: ���ô��ڵ���ʾ״̬
    // draw.hExWnd: Ŀ�괰�ڵľ������ʶҪ�����Ĵ���
    // SW_SHOW: ��ʾ���ڲ������������֮ǰ���أ����������ڵ�ǰ Z ˳��λ��
    // ���ã�ʹ���ڿɼ����������֮ǰδ��ʾ���˺����ᴥ�� WM_SHOWWINDOW ��Ϣ

    // ���´���
    UpdateWindow(draw.hExWnd);
    // UpdateWindow: ǿ�Ƹ��´��ڵĿͻ���
    // draw.hExWnd: Ŀ�괰�ڵľ������ʶҪ���µĴ���
    // ���ã������ػ洰�ڵĿͻ����������δ����� WM_PAINT ��Ϣ���ᴥ������
    // ͨ���ڸ��Ĵ������ݺ���ã�ȷ����ʾ����״̬

   //������Ϣѭ����ֻҪ���ڲ��رգ�ѭ��ִ��
   while (msg.message != WM_QUIT)
   {
      //ȷ������ʼ������ǰ��
       SetWindowPos(
           draw.hExWnd, // Ŀ�괰�ڵľ��
           // draw.hExWnd ��Ҫ�����Ĵ��ڵľ����ͨ���� CreateWindowEx ����

           HWND_TOPMOST, // ���ڵ� Z ˳�򣨲㼶��
           // HWND_TOPMOST ��ʾ�������������з��ö�����֮�ϣ�ʼ�ձ������ϲ�

           draw.rectGame.left, // ���ڵ��� x ���꣨���Ե��
           // ָ����������Ļ�ϵ�ˮƽλ�ã�ȡ�� draw.rectGame ����߽�

           draw.rectGame.top, // ���ڵ��� y ���꣨�ϱ�Ե��
           // ָ����������Ļ�ϵĴ�ֱλ�ã�ȡ�� draw.rectGame ���ϱ߽�

           draw.rectGame.right - draw.rectGame.left, // ���ڵ��¿��
           // ���㴰�ڿ�ȣ�ͨ�����ε��ұ߽��ȥ��߽�ó�

           draw.rectGame.bottom - draw.rectGame.top, // ���ڵ��¸߶�
           // ���㴰�ڸ߶ȣ�ͨ�����ε��±߽��ȥ�ϱ߽�ó�

           SWP_SHOWWINDOW // ���ƴ�����Ϊ�ı�־
           // SWP_SHOWWINDOW: ��ʾ���ڣ����֮ǰ���أ���ȷ�����ڿɼ�
           // δʹ��������־���� SWP_NOZORDER �� SWP_NOMOVE������ʾͬʱ����λ�úʹ�С
       );
       // ÿ��ѭ�����»�ȡ��Ϸ���ڵ�λ�úͳߴ磬����ȫ���������
       GetWindowRect(offsets.hWnd, &draw.rectGame);
       // ��鲢����ָ�����ڵ���Ϣ���У����������Լ�����ģ�洰�ڵ���Ϣ
       if (PeekMessage(&msg, draw.hExWnd, 0, 0, PM_REMOVE))
       {
           // PeekMessage: �����Ϣ�������Ƿ�����Ϣ�����ȴ���Ϣ����
           // &msg: ָ�� MSG �ṹ���ָ�룬���ڽ�����Ϣ����
           // draw.hExWnd: Ŀ�괰�ڵľ����������˴��ڵ���Ϣ
           // 0, 0: ��Ϣ��Χ���˴�Ϊ 0 ��ʾ��ɸѡ�ض���Ϣ���ͣ�����������Ϣ
           // PM_REMOVE: �������Ϣ���Ӷ������Ƴ�����䵽 msg �ṹ��
           // ����ֵ: ���㣨TRUE����ʾ����Ϣ���㣨FALSE����ʾ����Ϣ��

           //�ַ�������Ϣ
           TranslateMessage(&msg);

           //�ַ���Ϣ������Ϣ���ݸ����ڹ��̺������д���
           DispatchMessage(&msg);

       }

   }
}

;
