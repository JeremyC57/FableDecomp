// CancelCompString(HWND hwnd, bool bUseBackSpace, int iNewStrLen) -- DXUT ImeUi.cpp (level 3): move the caret to the end of the composition, backspace over it (down to iNewStrLen in overwrite mode) and move the caret back left
// Grade: DIFFER(210v215) -- LTCG custom callee: every SendControlKeys 0x00CB26B0 call passes num in EAX and vk in EDI; the row itself is stack-only and callee-pop (`ret 0Ch`, the __stdcall shape)
// Leaf: ?CancelCompString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2710, 213 bytes (+ int3 padding). File-static in ImeUi.cpp
// (FableWin.pdb "CancelCompString"); callers 0x00CB4AFD / 0x00CB4B4E / 0x00CB4CC8. The manifest label
// "ImeUi_ProcessComposition" is descriptive.
// Callees: kernel32 lstrlenW (IAT 0x0143FE48; GetCharCount was lstrlen in this SDK drop), SendControlKeys
// @ 0x00CB26B0, the _SendMessage pointer 0x0143E8C4.
// Globals: g_dwIMELevel 0x0143CD1C, g_szCompositionString 0x0143CF10, g_IMECursorChars 0x0143E898,
// g_bInsertMode 0x013AC749, bIsSendingKeyMessage 0x0143E8AD.
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

void __fastcall SendControlKeys(unsigned int vk, unsigned int num);   // 0x00CB26B0 (LTCG: num in eax, vk in edi)

void __stdcall CancelCompString(HWND hwnd, bool bUseBackSpace, int iNewStrLen)
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
