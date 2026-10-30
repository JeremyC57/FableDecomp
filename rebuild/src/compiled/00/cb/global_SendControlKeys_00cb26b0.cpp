// SendControlKeys(unsigned int vk, unsigned int num) -- DXUT ImeUi.cpp: send `num` WM_KEYDOWNs of vk to the current window, then one WM_KEYUP
// Shape (retry1): parked as an "LTCG custom convention" gap; ImeUi.cpp's file statics (FableWin.pdb local procs) are written as `static __declspec(noinline)` with a same-TU caller, and VC7.1 /O2 picks retail's private register convention itself. SendControlKeys: num in EAX, vk in EDI; CancelCompString (also in the TU) is its caller.
// Leaf: ?SendControlKeys@@YAXII@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB26B0, 83 bytes. File-static in ImeUi.cpp (FableWin.pdb
// "SendControlKeys"); callers 0x00CB273E / 0x00CB27C4 / 0x00CB27DC (CancelCompString) and 0x00CB4CA9. The manifest
// label "ImeUi_SendCompositionEvents" is descriptive.
// Shape: the DXUT body with the inline SendKeyMsg of this SDK drop (lParam 0); the `= false` stores inside the loop
// are dead and only the last one survives.
extern "C" __declspec(dllimport) int __stdcall lstrlenW(const unsigned short* string);    // IAT 0x0143FE48
struct HWND__ { int unused; };   typedef HWND__* HWND;

extern long (__stdcall* _SendMessage)(HWND hwnd, unsigned int msg, unsigned int wp, long lp);   // 0x0143E8C4
extern bool bIsSendingKeyMessage;                   // 0x0143E8AD
extern HWND g_hwndCurr;                             // 0x0143CF0C

inline long SendKeyMsg(HWND hwnd, unsigned int msg, unsigned int wp)
{
    bIsSendingKeyMessage = true;
    long lRc = _SendMessage(hwnd, msg, wp, 0);
    bIsSendingKeyMessage = false;
    return lRc;
}
#define SendKeyMsg_DOWN(hwnd,vk)    SendKeyMsg(hwnd, 0x0100 /* WM_KEYDOWN */, vk)
#define SendKeyMsg_UP(hwnd,vk)      SendKeyMsg(hwnd, 0x0101 /* WM_KEYUP */, vk)

extern unsigned long  g_dwIMELevel;                 // 0x0143CD1C
extern unsigned short g_szCompositionString[256];   // 0x0143CF10
extern unsigned long  g_IMECursorChars;             // 0x0143E898
extern bool           g_bInsertMode;                // 0x013AC749

static __declspec(noinline) void SendControlKeys(unsigned int vk, unsigned int num)   // 0x00CB26B0
{
    if (num == 0)
        return;
    for (unsigned int i = 0; i < num; i++)
    {
        SendKeyMsg_DOWN(g_hwndCurr, vk);
    }
    SendKeyMsg_UP(g_hwndCurr, vk);
}

static __declspec(noinline) void __stdcall CancelCompString(HWND hwnd, bool bUseBackSpace, int iNewStrLen)
{
    if (g_dwIMELevel != 3)
        return;
    int cc = lstrlenW(g_szCompositionString);
    int i;
    // move caret to the end of composition string
    SendControlKeys(0x27 /* VK_RIGHT */, cc - g_IMECursorChars);

    if (bUseBackSpace || g_bInsertMode)
        iNewStrLen = 0;

    if (iNewStrLen < cc)
    {
        for (i = 0; i < cc - iNewStrLen; i++)
        {
            SendKeyMsg_DOWN(hwnd, 0x08 /* VK_BACK */);
            SendKeyMsg(hwnd, 0x0102 /* WM_CHAR */, 8);  // Backspace character
        }
        SendKeyMsg_UP(hwnd, 0x08 /* VK_BACK */);
    }
    else
        iNewStrLen = cc;

    SendControlKeys(0x25 /* VK_LEFT */, iNewStrLen);
}

// Same-TU caller (the CancelCompString calls of ImeUi.cpp ImeUi_ProcessMessage / ImeUi_FinalizeString), present so the statics
// get retail's private conventions. Not graded.
void ImeUi_CancelCompString_caller(HWND hwnd, bool bUseBackSpace)
{
    CancelCompString(hwnd, bUseBackSpace, 0);
}