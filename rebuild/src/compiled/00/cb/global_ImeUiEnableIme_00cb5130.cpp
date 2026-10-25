// ImeUi_EnableIme(bool bEnable) -- DXUT ImeUi.cpp: attach (or detach) the original input context to the main window, record the enabled flag and refresh the toggle state when enabling
// Leaf: ?ImeUi_EnableIme@@YIX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5130, 80 bytes. __fastcall (bEnable in cl); FableWin.pdb
// ?ImeUi_EnableIme@@YIX_N@Z. The manifest label is right.
// Callees: imm32 ImmAssociateContext (import thunk 0x00CB5564), CheckToggleState @ 0x00CB47C0 (tail `jmp`).
// Globals: g_bInitialized 0x0143E8AE, g_hwndCurr 0x0143CF0C, g_bDisableImeCompletely 0x0143E89C (a DWORD),
// g_hwndMain 0x0143E510, g_himcOrg 0x0143E634, g_bImeEnabled 0x0143E890.
// Shape: the 2004/2005 SDK body (no TSF EnableUiUpdates call); `bEnable ? g_himcOrg : NULL` is `neg ; sbb ; and`.
struct HWND__ { int unused; };   typedef HWND__* HWND;
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;

extern "C" HIMC __stdcall ImmAssociateContext(HWND hwnd, HIMC himc);                        // import thunk 0x00CB5564

void __fastcall CheckToggleState();                 // 0x00CB47C0

extern bool          g_bInitialized;                // 0x0143E8AE
extern HWND          g_hwndCurr;                    // 0x0143CF0C
extern unsigned long g_bDisableImeCompletely;       // 0x0143E89C
extern HWND          g_hwndMain;                    // 0x0143E510
extern HIMC          g_himcOrg;                     // 0x0143E634
extern bool          g_bImeEnabled;                 // 0x0143E890

void __fastcall ImeUi_EnableIme(bool bEnable)
{
    if (!g_bInitialized || !g_hwndCurr)
        return;
    if (g_bDisableImeCompletely)
        bEnable = false;

    if (g_hwndCurr == g_hwndMain)
    {
        HIMC himcDbg;
        himcDbg = ImmAssociateContext(g_hwndCurr, bEnable ? g_himcOrg : 0);
        (void)himcDbg;
    }
    g_bImeEnabled = bEnable;
    if (bEnable)
    {
        CheckToggleState();
    }
}