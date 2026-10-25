// ImeUi_IgnoreHotKey(const MSG* pmsg) -- DXUT ImeUi.cpp: for a VK_PROCESSKEY key-down, true when the real key is an IME hot key to swallow (VK_JUNJA, VK_RIGHT in a CHT candidate list, or a Ctrl/Shift/Alt + key from the hot-key table)
// Leaf: ?ImeUi_IgnoreHotKey@@YI_NPBUtagMSG@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2B90, 208 bytes. __fastcall (pmsg in ecx), bool in al; FableWin.pdb
// ?ImeUi_IgnoreHotKey@@YI_NPBUtagMSG@@@Z. The manifest label "ImeUI_CheckCompositionKeyMatch" is descriptive.
// Callees: imm32 ImmGetVirtualKey (import thunk 0x00CB552E), user32 GetKeyState (IAT 0x014403C4, kept in esi).
// Globals: g_bInitialized 0x0143E8AE, g_bCandList 0x0143E8AF, g_hklCurrent 0x0143E8B8, aHotKeys 0x013AC760 (22
// entries of {bool m_bCtrl, m_bShift, m_bAlt; UINT m_uVk}: this SDK drop has no ATOK2005 Ctrl+Tab row).
// Shape: the DXUT body of the 2004/2005 SDK -- no VK_HANJA special case (ENABLE_HANJA_KEY), the `& 0x8000 ? true :
// false` modifier tests become `shr 0Fh ; and 1`, the table walk is a byte-offset loop to 0xb0.
// Model: DXUT ImeUi.cpp (DirectX SDK "ImeUi" IME library, UNICODE build) as linked into Fable.exe; FableWin.pdb
// names its functions (`?ImeUi_*@@YI...` = __fastcall: the game TU is /Gr, so every plain function and function-
// pointer type defaults to __fastcall, while the WINAPI ones stay __stdcall). The file-static globals are declared
// extern here with their retail addresses; the imm32 calls go through the linker import thunks (not dllimport).
struct HWND__ { int unused; };   typedef HWND__* HWND;
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;
struct HKL__  { int unused; };   typedef HKL__*  HKL;
struct tagRECT { long left, top, right, bottom; };   typedef tagRECT RECT;
class CImeUiFont_Base;

struct IMEUI_APPEARANCE {                           // ImeUi.h, 0x40 bytes
    unsigned long    symbolColor;                   // +0x00
    unsigned long    symbolColorOff;                // +0x04
    unsigned long    symbolColorText;               // +0x08
    unsigned char    symbolHeight;                  // +0x0c
    unsigned char    symbolTranslucence;            // +0x0d
    unsigned char    symbolPlacement;               // +0x0e
    CImeUiFont_Base* symbolFont;                    // +0x10
    unsigned long    candColorBase;                 // +0x14
    unsigned long    candColorBorder;               // +0x18
    unsigned long    candColorText;                 // +0x1c
    unsigned long    compColorInput;                // +0x20
    unsigned long    compColorTargetConv;           // +0x24
    unsigned long    compColorConverted;            // +0x28
    unsigned long    compColorTargetNotConv;        // +0x2c
    unsigned long    compColorInputErr;             // +0x30
    unsigned char    compTranslucence;              // +0x34
    unsigned long    compColorText;                 // +0x38
    unsigned char    caretWidth;                    // +0x3c
    unsigned char    caretYMargin;                  // +0x3d
};

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

struct SImeHotKey {                                 // ImeUi.cpp's anonymous aHotKeys element
    bool         m_bCtrl;                           // +0x00
    bool         m_bShift;                          // +0x01
    bool         m_bAlt;                            // +0x02
    unsigned int m_uVk;                             // +0x04
};

extern "C" unsigned int __stdcall ImmGetVirtualKey(HWND hwnd);                    // import thunk 0x00CB552E
extern "C" __declspec(dllimport) short __stdcall GetKeyState(int nVirtKey);      // IAT 0x014403C4

extern bool       g_bInitialized;                   // 0x0143E8AE
extern bool       g_bCandList;                      // 0x0143E8AF
extern HKL        g_hklCurrent;                     // 0x0143E8B8
extern SImeHotKey aHotKeys[22];                     // 0x013AC760

#define COUNTOF(a)   (sizeof(a) / sizeof((a)[0]))
#define GETLANG()    ((unsigned short)((unsigned long)(g_hklCurrent) & 0xffff))
#define LANG_CHT     0x0404

bool __fastcall ImeUi_IgnoreHotKey(const MSG* pmsg)
{
    if (!g_bInitialized || !pmsg)
        return false;

    if (pmsg->wParam == 0xE5 /* VK_PROCESSKEY */ &&
        (pmsg->message == 0x0100 /* WM_KEYDOWN */ || pmsg->message == 0x0104 /* WM_SYSKEYDOWN */))
    {
        bool bCtrl, bShift, bAlt;
        unsigned int uVkReal = ImmGetVirtualKey(pmsg->hwnd);
        // special case #1 - VK_JUNJA toggles half/full width input mode in Korean IME.
        if (uVkReal == 0x17 /* VK_JUNJA */)
        {
            return true;
        }
        // special case #2 - disable right arrow key that switches the candidate list to expanded mode in CHT IME.
        if (uVkReal == 0x27 /* VK_RIGHT */ && g_bCandList && GETLANG() == LANG_CHT)
        {
            return true;
        }
        bCtrl = (GetKeyState(0x11 /* VK_CONTROL */) & 0x8000) ? true : false;
        bShift = (GetKeyState(0x10 /* VK_SHIFT */) & 0x8000) ? true : false;
        bAlt = (GetKeyState(0x12 /* VK_MENU */) & 0x8000) ? true : false;
        for (int i = 0; i < COUNTOF(aHotKeys); i++)
        {
            if (aHotKeys[i].m_bCtrl == bCtrl &&
                aHotKeys[i].m_bShift == bShift &&
                aHotKeys[i].m_bAlt == bAlt &&
                aHotKeys[i].m_uVk == uVkReal)
                return true;
        }
    }
    return false;
}