// _PumpMessage() -- DXUT ImeUi.cpp: drain the thread's message queue (PeekMessage/GetMessage/TranslateMessage/DispatchMessage), posting WM_QUIT again when GetMessage returns 0
// Leaf: ?_PumpMessage@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2F00, 127 bytes. No arguments (file-static in ImeUi.cpp,
// FableWin.pdb "_PumpMessage"; external here so the TU emits it). The manifest label "ProcessAllMessages" is
// descriptive.
// Callees: user32 PeekMessageA / GetMessageA / TranslateMessage / DispatchMessageA / PostQuitMessage (IAT 0x014403D4 /
// 0x014403CC / 0x0144036C / 0x014403D0 / 0x014403C8; the four loop imports are cached in esi/ebp/edi/ebx).
// Shape: the DXUT body verbatim (MSG is 0x1c bytes, PM_NOREMOVE = 0).
struct HWND__ { int unused; };   typedef HWND__* HWND;
struct tagPOINT { long x, y; };
struct tagMSG {
    HWND          hwnd;                             // +0x00
    unsigned int  message;                          // +0x04
    unsigned int  wParam;                           // +0x08
    long          lParam;                           // +0x0c
    unsigned long time;                             // +0x10
    tagPOINT      pt;                               // +0x14
};
typedef tagMSG MSG;

extern "C" __declspec(dllimport) int __stdcall PeekMessageA(MSG* lpMsg, HWND hWnd, unsigned int wMsgFilterMin,
                                                            unsigned int wMsgFilterMax, unsigned int wRemoveMsg);   // IAT 0x014403D4
extern "C" __declspec(dllimport) int __stdcall GetMessageA(MSG* lpMsg, HWND hWnd, unsigned int wMsgFilterMin,
                                                           unsigned int wMsgFilterMax);                         // IAT 0x014403CC
extern "C" __declspec(dllimport) int __stdcall TranslateMessage(const MSG* lpMsg);                              // IAT 0x0144036C
extern "C" __declspec(dllimport) long __stdcall DispatchMessageA(const MSG* lpMsg);                             // IAT 0x014403D0
extern "C" __declspec(dllimport) void __stdcall PostQuitMessage(int nExitCode);                                 // IAT 0x014403C8

// this function is used only in brief time in CHT IME handling, so accelerator isn't processed.
void __fastcall _PumpMessage()
{
    MSG msg;
    while (PeekMessageA(&msg, 0, 0, 0, 0 /* PM_NOREMOVE */))
    {
        if (!GetMessageA(&msg, 0, 0, 0))
        {
            PostQuitMessage(msg.wParam);
            return;
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}