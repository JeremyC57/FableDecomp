// SetSupportLevel(unsigned long dwImeLevel) -- DXUT ImeUi.cpp: accept IME support level 2 or 3 (always 3 for Korean), cancel the current composition and recompute the composition colours
// Grade: DIFFER(49v50) -- LTCG custom convention: retail takes dwImeLevel in EAX (ecx is free for the `xor cl,cl` bSend argument)
// Leaf: ?SetSupportLevel@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2D90, 50 bytes. File-static in ImeUi.cpp (FableWin.pdb
// "SetSupportLevel"). The manifest label "ImeUI_OnModeChange" is descriptive.
// Callees: ImeUi_FinalizeString @ 0x00CB2C60 (bSend = false), SetCompStringColor @ 0x00CB2D20 (tail `jmp`).
// Globals: g_hklCurrent 0x0143E8B8, g_dwIMELevel 0x0143CD1C.
struct HKL__ { int unused; };   typedef HKL__* HKL;

extern HKL           g_hklCurrent;                  // 0x0143E8B8
extern unsigned long g_dwIMELevel;                  // 0x0143CD1C

void __fastcall ImeUi_FinalizeString(bool bSend);   // 0x00CB2C60
void __fastcall SetCompStringColor();               // 0x00CB2D20

#define GETPRIMLANG()   ((unsigned short)((unsigned long)(g_hklCurrent) & 0x3ff))

void __fastcall SetSupportLevel(unsigned long dwImeLevel)
{
    if (dwImeLevel < 2 || 3 < dwImeLevel)
        return;
    if (GETPRIMLANG() == 0x12 /* LANG_KOREAN */)
    {
        dwImeLevel = 3;
    }
    g_dwIMELevel = dwImeLevel;
    // cancel current composition string.
    ImeUi_FinalizeString(false);
    SetCompStringColor();
}
