// CSystemManager::InitWindow(long width, long height) -- register the application window class and create, show and update a centred fixed-size captioned window
// Leaf: InitWindow
// Retail 0x009A64B0, 352 bytes, `ret 8`.
// Callees (USER32/GDI32 imports): LoadCursorW 0x01440398, GetStockObject 0x0143FC78, LoadIconW 0x01440394 (resource 101,
// twice), RegisterClassExW 0x01440390, GetSystemMetrics 0x0144038C (SM_CXSCREEN, SM_CYSCREEN, SM_CXBORDER, SM_CYBORDER,
// SM_CYCAPTION), CreateWindowExW 0x01440388, ShowWindow 0x01440354, UpdateWindow 0x01440384; CWideString's character
// pointer conversion 0x0099AF00 (labelled CCharString::operator const char*); the window procedure 0x009A5B60.
// Layout: Ego_r.pdb CSystemManager + 8 in this region (as retail stores show): WinInstanceHandle +0x90, ApplicationWinHandle
// +0x94, WinShow +0x98, WinAppName +0xA0 (CWideString), WinClass +0xA4 (WNDCLASSEXW, 0x30 bytes), WindowValid +0xE8.
// Shape: the class record is filled field by field (hIconSm loaded before hIcon); screen size and doubled border width are
// locals, the doubled border height is inline in the y expression (a local for it spills an extra frame word);
// and the window is centred with signed `/ 2`; the app name is both class and window title; `this` is the create parameter.
typedef void* HINSTANCE;
typedef void* HWND;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HBRUSH;
typedef void* HGDIOBJ;
typedef void* HMENU;
typedef long (__stdcall* WNDPROC)(HWND, unsigned int, unsigned int, long);
struct WNDCLASSEXW {
    unsigned int          cbSize;
    unsigned int          style;
    WNDPROC               lpfnWndProc;
    int                   cbClsExtra;
    int                   cbWndExtra;
    HINSTANCE             hInstance;
    HICON                 hIcon;
    HCURSOR               hCursor;
    HBRUSH                hbrBackground;
    const unsigned short* lpszMenuName;
    const unsigned short* lpszClassName;
    HICON                 hIconSm;
};
extern "C" __declspec(dllimport) HCURSOR __stdcall LoadCursorW(HINSTANCE inst, const unsigned short* name);
extern "C" __declspec(dllimport) HGDIOBJ __stdcall GetStockObject(int object);
extern "C" __declspec(dllimport) HICON __stdcall LoadIconW(HINSTANCE inst, const unsigned short* name);
extern "C" __declspec(dllimport) unsigned short __stdcall RegisterClassExW(const WNDCLASSEXW* wc);
extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int index);
extern "C" __declspec(dllimport) HWND __stdcall CreateWindowExW(unsigned long exStyle, const unsigned short* className,
    const unsigned short* windowName, unsigned long style, int x, int y, int width, int height, HWND parent, HMENU menu,
    HINSTANCE inst, void* param);
extern "C" __declspec(dllimport) int __stdcall ShowWindow(HWND wnd, int show);
extern "C" __declspec(dllimport) int __stdcall UpdateWindow(HWND wnd);
long __stdcall SystemManagerWndProc(HWND wnd, unsigned int msg, unsigned int wparam, long lparam);   // 0x009A5B60

namespace N_0099af00 { struct CWideString {
    void* PStringData;
    operator const unsigned short*() const;                // 0x0099AF00
}; } using N_0099af00::CWideString;
struct CSystemManager {
    unsigned char _pad00[0x90];
    HINSTANCE     WinInstanceHandle;                       // +0x90
    HWND          ApplicationWinHandle;                    // +0x94
    int           WinShow;                                 // +0x98
    char*         WinCommandLine;                          // +0x9c
    CWideString   WinAppName;                              // +0xa0
    WNDCLASSEXW   WinClass;                                // +0xa4
    unsigned char _padd4[0xe8 - 0xd4];
    bool          WindowValid;                             // +0xe8
    bool InitWindow(long width, long height);
};

bool CSystemManager::InitWindow(long width, long height)
{
    WinClass.cbSize = sizeof(WNDCLASSEXW);
    WinClass.style = 0;
    WinClass.lpfnWndProc = SystemManagerWndProc;
    WinClass.cbClsExtra = 0;
    WinClass.cbWndExtra = 0;
    WinClass.hInstance = WinInstanceHandle;
    WinClass.hCursor = LoadCursorW(0, (const unsigned short*)0x7f00);
    WinClass.hbrBackground = (HBRUSH)GetStockObject(4);
    WinClass.lpszMenuName = 0;
    WinClass.lpszClassName = WinAppName;
    WinClass.hIconSm = LoadIconW(WinInstanceHandle, (const unsigned short*)101);
    WinClass.hIcon = LoadIconW(WinInstanceHandle, (const unsigned short*)101);
    RegisterClassExW(&WinClass);
    int screenWidth = GetSystemMetrics(0);
    int screenHeight = GetSystemMetrics(1);
    int borderWidth = GetSystemMetrics(5) * 2;
    int y = (screenHeight - GetSystemMetrics(6) * 2 - height - GetSystemMetrics(4)) / 2;
    int x = (screenWidth - borderWidth - width) / 2;
    ApplicationWinHandle = CreateWindowExW(0, WinAppName, WinAppName, 0xca0000, x, y, width, height, 0, 0,
                                           WinInstanceHandle, this);
    ShowWindow(ApplicationWinHandle, WinShow);
    UpdateWindow(ApplicationWinHandle);
    WindowValid = true;
    return true;
}