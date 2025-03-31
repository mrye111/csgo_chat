#include "ExWindow.h"
#include"Memory.h"



void Loop()
{
    //读取人物本身数据
    DWORD64 LocalPlayer = mem.ReadMeory<DWORD64>(offsets.clientBase + offsets.dw_LocalPlayer);
    //读取阵营标志位
    DWORD64 LocalTeam = mem.ReadMeory<DWORD64>(LocalPlayer + offsets.m_iTeamNum);

    // 创建画布设备上下文(DC)，通过获取指定窗口的DC来实现绘图
    HDC hDc = GetDC(draw.hExWnd);

    // 创建画刷
    // GetStockObject 获取一个预定义的系统画刷(这里使用 BLACK_BRUSH)
    // 将其转换为 HBRUSH 类型以匹配预期类型
    HBRUSH hHrush = (HBRUSH)GetStockObject(BLACK_BRUSH); // 使用默认的黑色画刷

    // 使用黑色画刷填充设备上下文中指定的矩形区域(draw.rectEx)
    FillRect(hDc, &draw.rectEx, hHrush);

    // 清理资源，通过删除画刷对象来释放内存
    DeleteObject(hHrush);



    if (LocalPlayer)
    {
        //遍历所有角色
        for (int i = 0; i < 32; i++)
        {
            //获取每个玩家的地址，每个地址8字节,所以要乘以8
            DWORD64 Entity = mem.ReadMeory<DWORD64>(offsets.clientBase + offsets.dw_EntityList + i * 0x8);
            //判断是否是自己，如果是自己的话，跳过
            if (LocalPlayer == Entity) { continue; }
            //判断是否为空，为空跳过
            if (Entity == 0) { continue; }

            //使用我们声明的结构体Vec3和Vec2结构体，用于存储人物世界坐标和屏幕坐标
            Vec3 entityPos3;//人物世界坐标
            Vec2 entityPos2;//屏幕坐标

            //每个浮点型占4字节，所以要+4，来获取xyz的值
            entityPos3.x =  mem.ReadMeory<float>(Entity + offsets.m_fPos + 0x0);//x
            entityPos3.y =  mem.ReadMeory<float>(Entity + offsets.m_fPos + 0x4);//y
            entityPos3.z =  mem.ReadMeory<float>(Entity + offsets.m_fPos + 0x8);//z

            //获取敌人的阵营，数据类型是BYTE。要么是02CT,要么是03C
            DWORD teamId = mem.ReadMeory<BYTE>(Entity + offsets.m_iTeamNum);
            //判断数据是否为空
            if (teamId != 2 && teamId != 3){ continue; }
            //判断是否为敌人
            if (teamId != LocalTeam) {
                //获取敌人血量
                DWORD entityHealth = mem.ReadMeory<int>(Entity + offsets.m_iHealth);
                //判断敌人是否存活，并且是否是正确的血量数值,并且屏幕坐标转换成功
                if (0 < entityHealth && entityHealth <= 100 && draw.WorldToScreen(entityPos3, entityPos2))
                {
                    //遍历人物的骨骼点
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
1、设计注册窗口类
2、创建窗口
3、显示窗口
4、更新窗口
5、建立消息循环
6、实现窗口过程函数
*/

/*
定义一个窗口过程函数(Window Procedure)，用于处理窗口消息
LRESULT: 返回值类型，通常是 long，用于表示处理结果
CALLBACK: Windows API 的调用约定，表示函数遵循stdcall调用约定
HWND hwnd: 窗口句柄，标识接收消息的窗口
UINT uMsg: 消息类型，表示当前接收到的消息（如WM_CREATE、WM_DESTROY等）
WPARAM wParam: 附加参数1，与具体消息相关，提供额外信息
LPARAM lParam: 附加参数2，与具体消息相关，提供额外信息
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //uMsg = WM_PRINT;
    switch (uMsg) // 根据消息类型 uMsg 进行分支处理
    {
    case WM_PRINT:
        // 当窗口需要被打印或绘制到设备上下文（DC）时触发
        // wParam: 指向目标设备上下文的句柄 (HDC)
        // lParam: 指定打印选项（如PRF_CHECKVISIBLE等标志）
        // 通常用于自定义窗口的打印行为，目前为空，未实现具体逻辑
        Loop();
        break;

    case WM_CREATE:
        // 在窗口创建完成后、显示之前触发
        // wParam: 未使用
        // lParam: 指向 CREATESTRUCT 结构体的指针，包含窗口创建的参数（如位置、大小等）
        // 常用于初始化窗口资源（如控件、内存分配等），目前为空
        break;

    case WM_DESTROY:
        // 当窗口被销毁时触发，通常在窗口关闭后调用
        // wParam: 未使用
        // lParam: 未使用
        // 常用于清理资源（如释放内存、保存状态等），目前为空
        DestroyWindow(hwnd);
        break;

    case WM_CLOSE:
        // 当用户尝试关闭窗口（如点击关闭按钮）时触发
        // wParam: 未使用
        // lParam: 未使用
        // 默认情况下会调用 DestroyWindow 来销毁窗口，可在此添加确认逻辑
        // 目前为空，未阻止默认关闭行为
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
        // 处理所有未显式列出的消息
        // 通常将未处理的消息交给 DefWindowProc 处理，以确保系统默认行为
        // 目前为空，未实现具体逻辑
        break;
    }
    // 注意：当前代码没有返回值，通常应返回 DefWindowProc(hwnd, uMsg, wParam, lParam)
    // 以处理未显式处理的消息，这里可能需要补充
}

void CreateExternalWindow()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    //获取游戏窗口坐标，保存在draw结构体中
    GetWindowRect(offsets.hWnd,&draw.rectGame);

	//设计窗口类
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));//清空内存区域结构体，初始化0
	wc.cbSize = sizeof(wc);	//设置风格
    wc.style = CS_HREDRAW | CS_VREDRAW; // 设置窗口类的样式
    // CS_HREDRAW: 当窗口水平大小改变时，触发重绘整个窗口
    // CS_VREDRAW: 当窗口垂直大小改变时，触发重绘整个窗口
    // 这两个标志组合确保窗口在大小调整时自动刷新，避免残影

    wc.lpfnWndProc = (WNDPROC)WindowProc; // 指定窗口过程函数的指针
    // WindowProc 是处理窗口消息的回调函数（如WM_CREATE、WM_DESTROY等）
    // 类型转换为 WNDPROC，确保兼容性，负责处理该窗口类的所有消息

    wc.hInstance = GetModuleHandle(NULL); // 获取当前应用程序的实例句柄
    // GetModuleHandle(NULL) 返回当前可执行文件的模块句柄
    // 用于关联窗口类与应用程序实例，通常在 WinMain 中调用

    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 设置窗口的默认光标
    // LoadCursor 从系统资源加载光标，NULL 表示使用系统预定义光标
    // IDC_ARROW 是标准的箭头光标，用户鼠标悬停在窗口上时显示

    wc.hbrBackground = (HBRUSH)RGB(0, 0, 0); // 设置窗口的背景画刷
    // RGB(0, 0, 0) 定义黑色背景颜色（红=0, 绿=0, 蓝=0）
    // 类型转换为 HBRUSH，指定窗口客户区的背景色为黑色

    wc.lpszClassName = L"ExternalWindow"; // 定义窗口类的名称
    // L"ExternalWindow" 是一个宽字符字符串（Unicode），作为窗口类的唯一标识符
    // 用于后续创建窗口时引用该窗口类，必须全局唯一

    //注册窗口类
    RegisterClassEx(&wc);

    //创建窗口
    draw.hExWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, // 扩展窗口样式
        // WS_EX_TOPMOST: 窗口始终置顶，显示在其他非置顶窗口之上
        // WS_EX_TRANSPARENT: 窗口透明，点击可穿透到下方窗口 
        // WS_EX_LAYERED: 启用分层窗口，支持透明度或 alpha 混合效果

        wc.lpszClassName, // 窗口类的名称
        // 使用之前注册的窗口类名（例如 L"ExternalWindow"），定义窗口的行为和属性

        L"ExWindow", // 窗口标题
        // 显示在窗口标题栏的文本，此处为 "ExWindow"，对于弹出窗口可能不可见

        WS_POPUP, // 窗口样式
        // WS_POPUP: 创建一个弹出窗口，无边框和标题栏，适合浮动窗口或覆盖层

        draw.rectGame.left, // 窗口的 x 坐标（左边缘）
        // 指定窗口在屏幕上的水平起始位置，取自 draw.rectGame 的左边界

        draw.rectGame.top, // 窗口的 y 坐标（上边缘）
        // 指定窗口在屏幕上的垂直起始位置，取自 draw.rectGame 的上边界

        draw.rectGame.right - draw.rectGame.left, // 窗口宽度
        // 计算窗口宽度，通过矩形的右边界减去左边界得出

        draw.rectGame.bottom - draw.rectGame.top, // 窗口高度
        // 计算窗口高度，通过矩形的下边界减去上边界得出

        NULL, // 父窗口句柄
        // NULL 表示该窗口没有父窗口，是独立窗口

        NULL, // 菜单句柄
        // NULL 表示窗口不使用菜单，对于弹出窗口通常不需要

        wc.hInstance, // 应用程序实例句柄
        // 指定创建窗口的应用程序实例，通常与窗口类注册时一致

        NULL // 附加数据
        // NULL 表示不传递额外数据给窗口过程函数的 WM_CREATE 消息
    );
    // 没有创建成功，返回
    if (draw.hExWnd == NULL)
    {
        return;
    }

    //设置透明窗口
    SetLayeredWindowAttributes(
        draw.hExWnd, // 目标窗口的句柄
        // draw.hExWnd 是要设置属性的窗口句柄，通常由 CreateWindowEx 创建
        // 必须是带有 WS_EX_LAYERED 扩展样式的窗口

        RGB(0, 0, 0), // 颜色键（透明色）
        // RGB(0, 0, 0) 指定黑色作为透明色
        // 当使用 LWA_COLORKEY 标志时，与此颜色匹配的像素将变为透明
        // 此处与 LWA_ALPHA 一起使用，具体效果取决于标志

        0, // Alpha 值（透明度）
        // 范围 0-255，0 表示完全透明，255 表示完全不透明
        // 此处设为 0，结合 LWA_ALPHA 标志，使整个窗口完全透明

        LWA_COLORKEY // 操作标志
        // LWA_ALPHA: 使用第三个参数（Alpha 值）控制窗口的整体透明度
        // 注意：未使用 LWA_COLORKEY，因此第二个参数（颜色键）在此上下文中不起作用
    );

    // 显示窗口
    ShowWindow(draw.hExWnd, SW_SHOW);
    // ShowWindow: 设置窗口的显示状态
    // draw.hExWnd: 目标窗口的句柄，标识要操作的窗口
    // SW_SHOW: 显示窗口并激活它（如果之前隐藏），将其置于当前 Z 顺序位置
    // 作用：使窗口可见，如果窗口之前未显示，此函数会触发 WM_SHOWWINDOW 消息

    // 更新窗口
    UpdateWindow(draw.hExWnd);
    // UpdateWindow: 强制更新窗口的客户区
    // draw.hExWnd: 目标窗口的句柄，标识要更新的窗口
    // 作用：立即重绘窗口的客户区，如果有未处理的 WM_PAINT 消息，会触发绘制
    // 通常在更改窗口内容后调用，确保显示最新状态

   //建立消息循环，只要窗口不关闭，循环执行
   while (msg.message != WM_QUIT)
   {
      //确保窗口始终在最前面
       SetWindowPos(
           draw.hExWnd, // 目标窗口的句柄
           // draw.hExWnd 是要调整的窗口的句柄，通常由 CreateWindowEx 返回

           HWND_TOPMOST, // 窗口的 Z 顺序（层级）
           // HWND_TOPMOST 表示将窗口置于所有非置顶窗口之上，始终保持最上层

           draw.rectGame.left, // 窗口的新 x 坐标（左边缘）
           // 指定窗口在屏幕上的水平位置，取自 draw.rectGame 的左边界

           draw.rectGame.top, // 窗口的新 y 坐标（上边缘）
           // 指定窗口在屏幕上的垂直位置，取自 draw.rectGame 的上边界

           draw.rectGame.right - draw.rectGame.left, // 窗口的新宽度
           // 计算窗口宽度，通过矩形的右边界减去左边界得出

           draw.rectGame.bottom - draw.rectGame.top, // 窗口的新高度
           // 计算窗口高度，通过矩形的下边界减去上边界得出

           SWP_SHOWWINDOW // 控制窗口行为的标志
           // SWP_SHOWWINDOW: 显示窗口（如果之前隐藏），确保窗口可见
           // 未使用其他标志（如 SWP_NOZORDER 或 SWP_NOMOVE），表示同时调整位置和大小
       );
       // 每次循环重新获取游戏窗口的位置和尺寸，避免全屏等情况。
       GetWindowRect(offsets.hWnd, &draw.rectGame);
       // 检查并处理指定窗口的消息队列，处理我们自己创建模版窗口的消息
       if (PeekMessage(&msg, draw.hExWnd, 0, 0, PM_REMOVE))
       {
           // PeekMessage: 检查消息队列中是否有消息，不等待消息到达
           // &msg: 指向 MSG 结构体的指针，用于接收消息内容
           // draw.hExWnd: 目标窗口的句柄，仅处理此窗口的消息
           // 0, 0: 消息范围，此处为 0 表示不筛选特定消息类型，处理所有消息
           // PM_REMOVE: 如果有消息，从队列中移除并填充到 msg 结构体
           // 返回值: 非零（TRUE）表示有消息，零（FALSE）表示无消息阿

           //分发键盘消息
           TranslateMessage(&msg);

           //分发消息，将消息传递给窗口过程函数进行处理
           DispatchMessage(&msg);

       }

   }
}

;
