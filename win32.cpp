#include<windows.h>
#include<stdio.h>
using namespace std;


HANDLE g_hOutput = 0;

void OnSize(HWND hWnd, UINT flag, LPARAM lParam){
    int width = LOWORD(lParam);  // Macro to get the low-order word.
    int height = HIWORD(lParam); // Macro to get the high-order word.

    char szText[255] = {0};
    sprintf(szText, "WM_SIZE:宽:%d, 高:%d\n", width, height);
    WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
    // Handle resizing
}

void OnCreate(HWND hWnd, LPARAM lParam){
    int width = LOWORD(lParam);  // Macro to get the low-order word.
    int height = HIWORD(lParam); // Macro to get the high-order word.

    // CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
    // wchar_t* pszText = (wchar_t*)pcs->lpCreateParams;
    // MessageBox(NULL, pszText, L"infor", MB_OK);
    char szText[255] = {0};
    sprintf(szText, "WM_SIZE:宽:%d, 高:%d\n", width, height);
    WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);

    CreateWindowExW(0, L"EDIT", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 10, 30, 460, 210, hWnd, NULL, 0, NULL);
}

void OnPaint(HWND hWnd){
    char* pszText = (char*)"WM_PAINT\n";
    WriteConsole(g_hOutput, pszText, strlen(pszText), NULL, NULL);

    PAINTSTRUCT ps = {0};
    HDC hdc = BeginPaint(hWnd, &ps);
    
    // All painting occurs here, between BeginPaint and EndPaint.
    EndPaint(hWnd, &ps); 
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID,WPARAM wParam,LPARAM lParam){
    switch (msgID){           
        case WM_CREATE:
            OnCreate(hWnd, lParam);
            break;

        case WM_SIZE: // Handle window resizing
            OnSize(hWnd, (UINT)wParam, lParam);
            break;

        case WM_PAINT:
            OnPaint(hWnd);
            break;
        
        case WM_LBUTTONDOWN:
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_CLOSE:
            if (MessageBox(NULL, TEXT("确认退出?"), TEXT("是否退出"), MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hWnd);
            }
            // Else: User canceled. Do nothing.
            return 0;
            break;

        case WM_DESTROY:
            PostQuitMessage(0);    //WM_QUIT
            break;  

        default:
            break;
    }
    return DefWindowProc(hWnd, msgID, wParam, lParam);
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PSTR lpCmdLine,
    _In_ INT nCmdShow
){
    AllocConsole();
    g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    const wchar_t CLASS_NAME[]  = L"Win32";

    WNDCLASSEXW wce;
    wce.cbClsExtra = 0;
    wce.cbSize = sizeof(WNDCLASSEXW);
    wce.cbWndExtra = 0;
    wce.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wce.hCursor = NULL;
    wce.hIcon = NULL;
    wce.hIconSm = NULL;
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WndProc;
    wce.lpszClassName = CLASS_NAME;
    wce.lpszMenuName = NULL;
    wce.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    
    RegisterClassExW(&wce);

    wchar_t* pszText = (wchar_t*)TEXT("hello");

    HWND hWnd = CreateWindowExW(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        (wchar_t*)TEXT("测试"),         // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        // CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        300, 300, 500, 300,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        pszText     // Additional application data
    );

    ShowWindow(hWnd,nCmdShow);

    UpdateWindow(hWnd);
    
    MSG nMsg;
    while (1){
        if (PeekMessage(&nMsg, NULL, 0, 0, PM_NOREMOVE)){
            if (GetMessage(&nMsg,NULL,0,0)){
                TranslateMessage(&nMsg);
                DispatchMessage(&nMsg);

            }else 
                return 0;
            
        }
    }    
}


