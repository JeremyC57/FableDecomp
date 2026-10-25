// ImeUi_FinalizeString(bool bSend) -- DXUT ImeUi.cpp: (non-reentrantly) optionally send the level-2 composition string to the app, then clear the composition data and cancel the IME composition and candidate list
// Leaf: ?ImeUi_FinalizeString@@YIX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2C60, 187 bytes (+ int3 padding). __fastcall (bSend in cl); FableWin.pdb
// ?ImeUi_FinalizeString@@YIX_N@Z. The manifest label "IME_ResetComposition" is descriptive.
// Callees: imm32 ImmGetContext / ImmNotifyIME / ImmReleaseContext (import thunks 0x00CB5540 / 0x00CB553A /
// 0x00CB5534), kernel32 lstrlenW (IAT 0x0143FE48), the _SendCompString pointer 0x0143E61C.
// Globals: g_bInitialized 0x0143E8AE, bProcessing 0x0143E8D4 (the function's static), g_hwndCurr 0x0143CF0C,
// g_dwIMELevel 0x0143CD1C, InitCompStringData's arrays/counters (0x0143CF10, 0x0143E518, 0x0143E894, 0x0143E898).
// Shape: the 2004/2005 SDK body: `lRet = lstrlen(g_szCompositionString)` survives as a call whose result is dead
// (the CHT trailing-space trim only changed lRet and folds away), InitCompStringData (landed out of line at
// 0x00CB27F0) is inlined in the DXUT order (counters first: inline they become immediate stores scheduled
// between the two `rep stosd`), there is no CloseCandidateList / UI-less branch yet.
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
#include <string.h>

extern "C" HIMC __stdcall ImmGetContext(HWND hwnd);                                         // import thunk 0x00CB5540
extern "C" int __stdcall ImmNotifyIME(HIMC himc, unsigned long dwAction, unsigned long dwIndex,
                                      unsigned long dwValue);                               // import thunk 0x00CB553A
extern "C" int __stdcall ImmReleaseContext(HWND hwnd, HIMC himc);                           // import thunk 0x00CB5534
extern "C" __declspec(dllimport) int __stdcall lstrlenW(const unsigned short* string);    // IAT 0x0143FE48

extern bool           g_bInitialized;               // 0x0143E8AE
extern bool           g_bFinalizeProcessing;        // 0x0143E8D4 (ImeUi_FinalizeString's `static bool bProcessing`)
extern HWND           g_hwndCurr;                   // 0x0143CF0C
extern unsigned long  g_dwIMELevel;                 // 0x0143CD1C
extern unsigned short g_szCompositionString[256];   // 0x0143CF10 (TCHAR, UNICODE)
extern unsigned char  g_szCompAttrString[256];      // 0x0143E518
extern unsigned long  g_IMECursorBytes;             // 0x0143E894
extern unsigned long  g_IMECursorChars;             // 0x0143E898
extern void (__fastcall* _SendCompString)();        // 0x0143E61C



inline void InitCompStringData()                    // landed out of line @ 0x00CB27F0
{
    g_IMECursorBytes = 0;
    g_IMECursorChars = 0;
    memset(&g_szCompositionString, 0, sizeof(g_szCompositionString));
    memset(&g_szCompAttrString, 0, sizeof(g_szCompAttrString));
}

void __fastcall ImeUi_FinalizeString(bool bSend)
{
    HIMC himc;
    if (!g_bInitialized || g_bFinalizeProcessing || 0 == (himc = ImmGetContext(g_hwndCurr)))
        return;
    g_bFinalizeProcessing = true;

    if (g_dwIMELevel == 2 && bSend)
    {
        // Send composition string to app.
        long lRet = lstrlenW(g_szCompositionString);
        (void)lRet;
        _SendCompString();
    }

    InitCompStringData();
    // clear composition string in IME
    ImmNotifyIME(himc, 0x15 /* NI_COMPOSITIONSTR */, 4 /* CPS_CANCEL */, 0);
    // the following line is necessary as Korean IME doesn't close cand list when comp string is cancelled.
    ImmNotifyIME(himc, 0x11 /* NI_CLOSECANDIDATE */, 0, 0);
    ImmReleaseContext(g_hwndCurr, himc);
    g_bFinalizeProcessing = false;
}