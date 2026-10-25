// Behaviour test for 0x00CB47C0 ImeUi::CheckToggleState: after CheckInputLocale, g_bIme = ImmIsIME, g_bChineseIME for a
// Chinese primary language; with a context, a Chinese IME's state comes from the native conversion bit (ON/ENGLISH),
// any other IME's from its open status (ON/OFF); no context means OFF.
#include <cstdio>

static unsigned int g_hklCurrent = 0, g_nextHkl = 0;
static bool g_bIme = false, g_bChineseIME = false;
static int g_hwndObj = 0, g_himcObj = 0;
static void* g_hwndCurr = &g_hwndObj;
static void* g_himc = &g_himcObj;
static unsigned int g_dwState = 99;
static bool g_isIme = false, g_open = false;
static unsigned int g_conv = 0;
static int g_releases = 0, g_locales = 0;

static void CheckInputLocale() { ++g_locales; g_hklCurrent = g_nextHkl; }
static int ImmIsIME_(unsigned int hkl) { return hkl == g_nextHkl && g_isIme; }
static void* ImmGetContext_(void* hwnd) { return hwnd == g_hwndCurr ? g_himc : 0; }
static int ImmGetConversionStatus_(void*, unsigned int* conv, unsigned int* sent) { *conv = g_conv; *sent = 0; return 1; }
static int ImmGetOpenStatus_(void*) { return g_open; }
static int ImmReleaseContext_(void*, void*) { ++g_releases; return 1; }

static void CheckToggleState()
{
    CheckInputLocale();
    g_bIme = ImmIsIME_(g_hklCurrent) != 0;
    g_bChineseIME = ((unsigned short)((unsigned short)(g_hklCurrent & 0xffff) & 0x3ff) == 0x04) && g_bIme;
    void* himc;
    if (0 != (himc = ImmGetContext_(g_hwndCurr))) {
        if (g_bChineseIME) {
            unsigned int dwConvMode, dwSentMode;
            ImmGetConversionStatus_(himc, &dwConvMode, &dwSentMode);
            g_dwState = (dwConvMode & 1) ? 1 : 2;
        } else {
            g_dwState = (g_bIme && ImmGetOpenStatus_(himc) != 0) ? 1 : 0;
        }
        ImmReleaseContext_(g_hwndCurr, himc);
    } else
        g_dwState = 0;
}

int main()
{
    bool ok = true;
    g_nextHkl = 0x08040804; g_isIme = true; g_conv = 1;           // Simplified Chinese IME, native mode
    CheckToggleState();
    ok = ok && g_bIme && g_bChineseIME && g_dwState == 1 && g_releases == 1 && g_locales == 1;
    g_conv = 0;                                                     // alphanumeric mode
    CheckToggleState();
    ok = ok && g_dwState == 2;
    g_nextHkl = 0x04110411; g_open = true;                         // Japanese IME, open
    CheckToggleState();
    ok = ok && g_bIme && !g_bChineseIME && g_dwState == 1;
    g_open = false;
    CheckToggleState();
    ok = ok && g_dwState == 0;
    g_nextHkl = 0x04040404; g_isIme = false; g_open = true;        // Chinese layout without an IME
    CheckToggleState();
    ok = ok && !g_bIme && !g_bChineseIME && g_dwState == 0 && g_releases == 5;
    g_isIme = true; g_himc = 0; g_dwState = 7;                     // no input context
    CheckToggleState();
    ok = ok && g_bChineseIME && g_dwState == 0 && g_releases == 5;
    if (!ok) { std::printf("00cb47c0_TEST FAIL\n"); return 1; }
    std::printf("00cb47c0_TEST PASS\n");
    return 0;
}