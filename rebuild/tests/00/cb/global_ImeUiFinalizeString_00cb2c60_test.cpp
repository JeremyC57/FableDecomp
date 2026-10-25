// Behaviour test for 0x00CB2C60 ImeUi::ImeUi_FinalizeString: nothing before init / while already finalising / without
// an input context; else send the composition at level 2 when asked, clear the composition data, cancel the
// composition and close the candidate list, release the context.
#include <cstdio>
#include <cstring>

static bool g_bInitialized = false, g_bFinalizeProcessing = false;
static int g_hwndCurrObj = 0;
static void* g_hwndCurr = &g_hwndCurrObj;
static unsigned int g_dwIMELevel = 2;
static unsigned short g_szCompositionString[256];
static unsigned char g_szCompAttrString[256];
static unsigned int g_IMECursorBytes = 5, g_IMECursorChars = 3;
static int g_sent = 0, g_notifies = 0, g_releases = 0, g_lastAction = 0;
static bool g_sawProcessing = false;
static int g_himcObj = 0;
static void* g_himc = &g_himcObj;
static void SendCompString_() { ++g_sent; g_sawProcessing = g_bFinalizeProcessing && g_szCompositionString[0] == 'k'; }
static void (*_SendCompString)() = SendCompString_;
static void* ImmGetContext_(void* hwnd) { return hwnd == g_hwndCurr ? g_himc : 0; }
static int ImmNotifyIME_(void* himc, unsigned int dwAction, unsigned int dwIndex, unsigned int)
{
    if (himc == g_himc) { ++g_notifies; g_lastAction = (int)(dwAction * 16 + dwIndex); }
    return 1;
}
static int ImmReleaseContext_(void* hwnd, void* himc) { if (hwnd == g_hwndCurr && himc == g_himc) ++g_releases; return 1; }
static int lstrlenW_(const unsigned short* s) { int n = 0; while (s[n]) ++n; return n; }

inline void InitCompStringData()
{
    g_IMECursorBytes = 0;
    g_IMECursorChars = 0;
    std::memset(&g_szCompositionString, 0, sizeof(g_szCompositionString));
    std::memset(&g_szCompAttrString, 0, sizeof(g_szCompAttrString));
}

static void ImeUi_FinalizeString(bool bSend)
{
    void* himc;
    if (!g_bInitialized || g_bFinalizeProcessing || 0 == (himc = ImmGetContext_(g_hwndCurr)))
        return;
    g_bFinalizeProcessing = true;
    if (g_dwIMELevel == 2 && bSend) {
        int lRet = lstrlenW_(g_szCompositionString);
        (void)lRet;
        _SendCompString();
    }
    InitCompStringData();
    ImmNotifyIME_(himc, 0x15, 4, 0);
    ImmNotifyIME_(himc, 0x11, 0, 0);
    ImmReleaseContext_(g_hwndCurr, himc);
    g_bFinalizeProcessing = false;
}

int main()
{
    bool ok = true;
    g_szCompositionString[0] = 'k'; g_szCompAttrString[0] = 1;
    ImeUi_FinalizeString(true);                                       // not initialised
    ok = ok && g_sent == 0 && g_notifies == 0 && g_IMECursorBytes == 5;
    g_bInitialized = true;
    g_bFinalizeProcessing = true;
    ImeUi_FinalizeString(true);                                       // re-entry guard
    ok = ok && g_sent == 0 && g_notifies == 0;
    g_bFinalizeProcessing = false;
    ImeUi_FinalizeString(true);
    ok = ok && g_sent == 1 && g_sawProcessing && g_notifies == 2 && g_lastAction == 0x110 && g_releases == 1;
    ok = ok && g_IMECursorBytes == 0 && g_IMECursorChars == 0 && g_szCompositionString[0] == 0 && g_szCompAttrString[0] == 0;
    ok = ok && !g_bFinalizeProcessing;
    ImeUi_FinalizeString(false);                                      // no send
    ok = ok && g_sent == 1 && g_notifies == 4;
    g_dwIMELevel = 3;
    ImeUi_FinalizeString(true);                                       // level 3 never sends
    ok = ok && g_sent == 1 && g_notifies == 6;
    g_himc = 0;
    ImeUi_FinalizeString(true);                                       // no context
    ok = ok && g_notifies == 6 && g_releases == 3;
    if (!ok) { std::printf("00cb2c60_TEST FAIL\n"); return 1; }
    std::printf("00cb2c60_TEST PASS\n");
    return 0;
}