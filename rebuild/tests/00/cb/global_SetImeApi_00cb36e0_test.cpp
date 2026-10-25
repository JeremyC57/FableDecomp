// Behaviour test for 0x00CB36E0 SetImeApi (DXUT ImeUi.cpp): clear both IME entry points, then, when the current layout
// names an IME file that loads, fetch its GetReadingString / ShowReadingWindow exports and, if the latter exists, hide the
// reading window on the current window's input context (released afterwards). The Win32/IMM calls are stubs here.
#include <cstdio>
#include <cstring>

typedef int (*FARPROC)();
static int g_fileNameLen = 0;
static int g_loadOk = 0;
static int g_hasShow = 0;
static int g_contextOk = 0;
static int g_showCalls = 0, g_showArg = -1, g_releases = 0, g_loads = 0;
static int s_module;

static unsigned int ImmGetIMEFileNameA(int hkl, char* name, unsigned int len)
{
    if (hkl == 0 || len != 260 || g_fileNameLen == 0) return 0;
    std::strcpy(name, "ime.dll");
    return (unsigned int)g_fileNameLen;
}
static void* LoadLibraryA(const char* name) { ++g_loads; return (g_loadOk && std::strcmp(name, "ime.dll") == 0) ? &s_module : 0; }
static int ReadingStub() { return 1; }
static FARPROC GetProcAddress(void*, const char* proc)
{
    if (std::strcmp(proc, "GetReadingString") == 0) return ReadingStub;
    if (std::strcmp(proc, "ShowReadingWindow") == 0 && g_hasShow) return ReadingStub;
    return 0;
}
static int ImmGetContext(int hwnd) { return g_contextOk ? hwnd + 100 : 0; }
static int ImmReleaseContext(int, int) { ++g_releases; return 1; }
static int ShowReadingWindowImpl(int himc, int show) { ++g_showCalls; g_showArg = show; return himc != 0; }

static FARPROC _GetReadingString;
static int (*_ShowReadingWindow)(int, int);
static int g_hklCurrent;
static int g_hwndCurr;

static void SetImeApi()
{
    _GetReadingString = 0;
    _ShowReadingWindow = 0;
    char szImeFile[260 + 1];
    int kl = g_hklCurrent;
    if (ImmGetIMEFileNameA(kl, szImeFile, sizeof(szImeFile) / sizeof(szImeFile[0]) - 1) <= 0)
        return;
    void* hIme = LoadLibraryA(szImeFile);
    if (!hIme)
        return;
    _GetReadingString = GetProcAddress(hIme, "GetReadingString");
    _ShowReadingWindow = GetProcAddress(hIme, "ShowReadingWindow") ? ShowReadingWindowImpl : 0;
    if (_ShowReadingWindow) {
        int himc;
        if (0 != (himc = ImmGetContext(g_hwndCurr))) {
            _ShowReadingWindow(himc, false);
            ImmReleaseContext(g_hwndCurr, himc);
        }
    }
}

int main()
{
    int ok = 1;
    _GetReadingString = ReadingStub; g_hklCurrent = 0;
    SetImeApi();                                              // no layout: entry points cleared, nothing loaded
    if (_GetReadingString != 0 || g_loads != 0) ok = 0;
    g_hklCurrent = 7; g_fileNameLen = 7; g_loadOk = 0;
    SetImeApi();                                              // load fails
    if (g_loads != 1 || _GetReadingString != 0) ok = 0;
    g_loadOk = 1; g_hasShow = 0;
    SetImeApi();                                              // no ShowReadingWindow export
    if (_GetReadingString == 0 || _ShowReadingWindow != 0 || g_showCalls != 0) ok = 0;
    g_hasShow = 1; g_contextOk = 1; g_hwndCurr = 5;
    SetImeApi();                                              // reading window hidden, context released
    if (_ShowReadingWindow == 0 || g_showCalls != 1 || g_showArg != 0 || g_releases != 1) ok = 0;
    g_contextOk = 0;
    SetImeApi();                                              // no context: no call
    if (g_showCalls != 1 || g_releases != 1) ok = 0;
    std::printf(ok ? "00cb36e0_TEST PASS\n" : "00cb36e0_TEST FAIL\n");
    return ok ? 0 : 1;
}