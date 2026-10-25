// CheckToggleState() -- DXUT ImeUi.cpp: refresh the input locale, decide whether a (Chinese) IME is active and read its conversion/open status into g_dwState (ON / ENGLISH / OFF)
// Leaf: ?CheckToggleState@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB47C0, 216 bytes (+ int3 padding). No arguments (file-static in
// ImeUi.cpp, FableWin.pdb "CheckToggleState"; external here so the TU emits it). The manifest label
// "ImeUi_QueryInputStatus" is descriptive.
// Callees: CheckInputLocale @ 0x00CB37A0 (no arguments), imm32 ImmIsIME / ImmGetContext / ImmGetConversionStatus /
// ImmGetOpenStatus / ImmReleaseContext (import thunks 0x00CB5558 / 0x00CB5540 / 0x00CB5552 / 0x00CB554C / 0x00CB5534).
// Globals: g_hklCurrent 0x0143E8B8, the IME flag 0x0143CD28 (written here and re-read after ImmGetContext, so a
// static, not a local -- descriptive name g_bIme), g_bChineseIME 0x0143CD29, g_hwndCurr 0x0143CF0C, g_dwState
// 0x0143E8B0.
// Shape: the 2004/2005 SDK body (no UI-less branch, bIme = ImmIsIME() != 0 without the TIP hack); the ternaries
// become `and dl,1 ; neg ; sbb ; add 2` and a store pair around the open-status test.
struct HWND__ { int unused; };   typedef HWND__* HWND;
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;
struct HKL__  { int unused; };   typedef HKL__*  HKL;

extern "C" int __stdcall ImmIsIME(HKL hkl);                                                 // import thunk 0x00CB5558
extern "C" HIMC __stdcall ImmGetContext(HWND hwnd);                                         // import thunk 0x00CB5540
extern "C" int __stdcall ImmGetConversionStatus(HIMC himc, unsigned long* lpfdwConversion,
                                                unsigned long* lpfdwSentence);              // import thunk 0x00CB5552
extern "C" int __stdcall ImmGetOpenStatus(HIMC himc);                                       // import thunk 0x00CB554C
extern "C" int __stdcall ImmReleaseContext(HWND hwnd, HIMC himc);                           // import thunk 0x00CB5534

void __fastcall CheckInputLocale();                 // 0x00CB37A0

extern HKL           g_hklCurrent;                  // 0x0143E8B8
extern bool          g_bIme;                        // 0x0143CD28 (descriptive name)
extern bool          g_bChineseIME;                 // 0x0143CD29
extern HWND          g_hwndCurr;                    // 0x0143CF0C
extern unsigned long g_dwState;                     // 0x0143E8B0

#define GETLANG()       ((unsigned short)((unsigned long)(g_hklCurrent) & 0xffff))
#define GETPRIMLANG()   ((unsigned short)(GETLANG() & 0x3ff))
#define LANG_CHINESE    0x04

void __fastcall CheckToggleState()
{
    CheckInputLocale();
    g_bIme = ImmIsIME(g_hklCurrent) != 0;
    g_bChineseIME = (GETPRIMLANG() == LANG_CHINESE) && g_bIme;

    HIMC himc;
    if (0 != (himc = ImmGetContext(g_hwndCurr)))
    {
        if (g_bChineseIME)
        {
            unsigned long dwConvMode, dwSentMode;
            ImmGetConversionStatus(himc, &dwConvMode, &dwSentMode);
            g_dwState = (dwConvMode & 1 /* IME_CMODE_NATIVE */) ? 1 /* IMEUI_STATE_ON */ : 2 /* IMEUI_STATE_ENGLISH */;
        }
        else
        {
            g_dwState = (g_bIme && ImmGetOpenStatus(himc) != 0) ? 1 /* IMEUI_STATE_ON */ : 0 /* IMEUI_STATE_OFF */;
        }
        ImmReleaseContext(g_hwndCurr, himc);
    }
    else
        g_dwState = 0;                              // IMEUI_STATE_OFF
}