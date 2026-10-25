// Behaviour test for 0x00CB5130 ImeUi::ImeUi_EnableIme: nothing before init or without a window; a completely disabled
// IME forces bEnable false; on the main window the original context is (re)associated or detached; the flag is stored
// and CheckToggleState runs when enabling.
#include <cstdio>

static bool g_bInitialized = false, g_bImeEnabled = false;
static int g_mainObj = 0, g_otherObj = 0, g_himcOrgObj = 0;
static void* g_hwndCurr = 0;
static void* g_hwndMain = &g_mainObj;
static void* g_himcOrg = &g_himcOrgObj;
static unsigned int g_bDisableImeCompletely = 0;
static int g_assoc = 0, g_toggles = 0;
static void* g_lastHimc = (void*)1;
static void* ImmAssociateContext_(void* hwnd, void* himc) { ++g_assoc; g_lastHimc = himc; (void)hwnd; return 0; }
static void CheckToggleState() { ++g_toggles; }

static void ImeUi_EnableIme(bool bEnable)
{
    if (!g_bInitialized || !g_hwndCurr)
        return;
    if (g_bDisableImeCompletely)
        bEnable = false;
    if (g_hwndCurr == g_hwndMain) {
        void* himcDbg = ImmAssociateContext_(g_hwndCurr, bEnable ? g_himcOrg : 0);
        (void)himcDbg;
    }
    g_bImeEnabled = bEnable;
    if (bEnable)
        CheckToggleState();
}

int main()
{
    bool ok = true;
    ImeUi_EnableIme(true);
    ok = ok && g_assoc == 0 && !g_bImeEnabled;
    g_bInitialized = true;
    ImeUi_EnableIme(true);                                  // no current window
    ok = ok && g_assoc == 0 && !g_bImeEnabled;
    g_hwndCurr = g_hwndMain;
    ImeUi_EnableIme(true);
    ok = ok && g_assoc == 1 && g_lastHimc == g_himcOrg && g_bImeEnabled && g_toggles == 1;
    ImeUi_EnableIme(false);
    ok = ok && g_assoc == 2 && g_lastHimc == 0 && !g_bImeEnabled && g_toggles == 1;
    g_hwndCurr = &g_otherObj;                               // not the main window: no association
    ImeUi_EnableIme(true);
    ok = ok && g_assoc == 2 && g_bImeEnabled && g_toggles == 2;
    g_bDisableImeCompletely = 1; g_hwndCurr = g_hwndMain;
    ImeUi_EnableIme(true);                                  // forced off
    ok = ok && g_assoc == 3 && g_lastHimc == 0 && !g_bImeEnabled && g_toggles == 2;
    if (!ok) { std::printf("00cb5130_TEST FAIL\n"); return 1; }
    std::printf("00cb5130_TEST PASS\n");
    return 0;
}