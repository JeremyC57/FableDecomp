// Test for 0x00CB5180 ImeUi_Initialize: fake OS version, imm32 exports, window kind and callbacks; checks the entry-point
// selection, the once-only context fetch, the failure tails and the call sequence of a successful initialisation.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#endif
// The test's own Win32 doubles (the CRT start-up links the real kernel32 ones, so they carry test names here).
#define GetVersionExA     Test_GetVersionExA
#define LoadLibraryA      Test_LoadLibraryA
#define GetProcAddress    Test_GetProcAddress
#define IsWindowUnicode   Test_IsWindowUnicode
#define SendMessageW      Test_SendMessageW
#define GetCaretBlinkTime Test_GetCaretBlinkTime
struct HWND__ { int unused; };   typedef HWND__* HWND;
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;
struct HIMCC__ { int unused; };  typedef HIMCC__* HIMCC;
struct HKL__  { int unused; };   typedef HKL__*  HKL;
struct HINSTANCE__ { int unused; };   typedef HINSTANCE__* HMODULE;
typedef int (__stdcall* FARPROC)();
struct tagRECT { long left, top, right, bottom; };   typedef tagRECT RECT;
struct tagCANDIDATELIST;   typedef tagCANDIDATELIST* LPCANDIDATELIST;
struct tagINPUTCONTEXT2;   typedef tagINPUTCONTEXT2* LPINPUTCONTEXT2;
class CImeUiFont_Base;
struct IMEUI_VERTEX;
struct OSVERSIONINFOA {                             // 0x94 bytes
    unsigned long dwOSVersionInfoSize;
    unsigned long dwMajorVersion;
    unsigned long dwMinorVersion;
    unsigned long dwBuildNumber;
    unsigned long dwPlatformId;
    char          szCSDVersion[128];
};
struct CompStringAttribute {
    unsigned int     caretX, caretY;
    CImeUiFont_Base* pFont;
    unsigned long    colorComp;                     // +0x0c
    unsigned long    colorCand;
    RECT             margins;
};
#define VER_PLATFORM_WIN32_NT 2
#define LANG_KOREAN           0x12
#define GETLANG()       ((unsigned short)((unsigned long)(g_hklCurrent) & 0xffff))
#define GETPRIMLANG()   ((unsigned short)(GETLANG() & 0x3ff))

extern "C" int __stdcall GetVersionExA(OSVERSIONINFOA* lpVersionInformation);        // IAT 0x0143FE70
extern "C" HMODULE __stdcall LoadLibraryA(const char* lpLibFileName);              // IAT 0x0143FE40
extern "C" FARPROC __stdcall GetProcAddress(HMODULE hModule, const char* lpProcName); // IAT 0x0143FE44
extern "C" int __stdcall IsWindowUnicode(HWND hwnd);                               // IAT 0x014402B0
extern "C" long __stdcall SendMessageW(HWND hwnd, unsigned int msg, unsigned int wp, long lp); // IAT 0x014402B4
extern "C" unsigned int __stdcall GetCaretBlinkTime();                             // IAT 0x014402B8
extern "C" HIMC __stdcall ImmGetContext(HWND hwnd);                                         // import thunk 0x00CB5540
extern "C" int __stdcall ImmReleaseContext(HWND hwnd, HIMC himc);                           // import thunk 0x00CB5534
extern "C" long __stdcall ImmGetCompositionStringW(HIMC himc, unsigned long dwIndex, void* lpBuf, unsigned long dwBufLen); // thunk 0x00CB5570
extern "C" unsigned long __stdcall ImmGetCandidateListW(HIMC himc, unsigned long deIndex, LPCANDIDATELIST lpCandList,
                                                        unsigned long dwBufLen);                           // thunk 0x00CB556A
extern "C" unsigned long __stdcall ImmGetCandidateListA(HIMC himc, unsigned long deIndex, LPCANDIDATELIST lpCandList,
                                                        unsigned long dwBufLen);                           // thunk 0x00CB551C

// ImeUi.cpp statics and entries in this TU (retail addresses).
unsigned long __fastcall GetCandidateList(HIMC himc, unsigned long dwIndex, LPCANDIDATELIST* ppCandList);     // 0x00CB2670
unsigned long __fastcall WA_GetCandidateList(HIMC himc, unsigned long dwIndex, LPCANDIDATELIST* ppCandList);  // 0x00CB2310
long __stdcall WA_ImmGetCompositionString(HIMC himc, unsigned long dwIndex, void* lpBuf, unsigned long dwBufLen); // 0x00CB2440
void __fastcall SendCompString();                   // 0x00CB25F0 (Unicode window)
void __fastcall SendCompString_Ansi();              // 0x00CB2230 (ANSI window; descriptive name)
void __fastcall CheckInputLocale();                 // 0x00CB37A0
void __fastcall SetImeApi();                        // 0x00CB36E0
void __fastcall ImeUi_FinalizeString(bool bSend);   // 0x00CB2C60
void __fastcall SetCompStringColor();               // 0x00CB2D20
void __fastcall CheckToggleState();                 // 0x00CB47C0
void __fastcall ImeUi_EnableIme(bool bEnable);      // 0x00CB5130

extern HWND          g_hwndMain;                    // 0x0143E510
extern HWND          g_hwndCurr;                    // 0x0143CF0C
extern HIMC          g_himcOrg;                     // 0x0143E634
extern HKL           g_hklCurrent;                  // 0x0143E8B8
extern bool          g_bInitialized;                // 0x0143E8AE
extern unsigned long g_bDisableImeCompletely;       // 0x0143E89C
extern OSVERSIONINFOA g_osi;                        // 0x0143CD68
extern unsigned int  g_uCaretBlinkTime;             // 0x0143E8A4
extern CompStringAttribute g_CaretInfo;             // 0x0143CD2C
extern unsigned long g_dwIMELevelSaved;             // 0x0143CD20
extern unsigned long g_dwIMELevel;                  // 0x0143CD1C
extern LPINPUTCONTEXT2 (__stdcall* _ImmLockIMC)(HIMC hIMC);                               // 0x0143E638
extern int (__stdcall* _ImmUnlockIMC)(HIMC hIMC);                                         // 0x0143CD04
extern void* (__stdcall* _ImmLockIMCC)(HIMCC hIMCC);                                      // 0x0143E620
extern int (__stdcall* _ImmUnlockIMCC)(HIMCC hIMCC);                                      // 0x0143E680
extern long (__stdcall* _ImmGetCompositionString)(HIMC himc, unsigned long dwIndex, void* lpBuf, unsigned long dwBufLen); // 0x0143CE04
extern unsigned long (__stdcall* _ImmGetCandidateList)(HIMC himc, unsigned long deIndex, LPCANDIDATELIST lpCandList,
                                                       unsigned long dwBufLen);           // 0x0143CD5C
extern unsigned long (__fastcall* _GetCandidateList)(HIMC himc, unsigned long dwIndex, LPCANDIDATELIST* ppCandList); // 0x0143CD00
extern void (__fastcall* _SendCompString)();        // 0x0143E61C
extern long (__stdcall* _SendMessage)(HWND hwnd, unsigned int msg, unsigned int wp, long lp);   // 0x0143E8C4
extern void (__fastcall* ImeUiCallback_DrawRect)(int x1, int y1, int x2, int y2, unsigned long color);   // 0x0143CD64
extern void* (__fastcall* ImeUiCallback_Malloc)(unsigned int bytes);                    // 0x0143E514
extern void (__fastcall* ImeUiCallback_Free)(void* ptr);                                // 0x0143CF08
extern void (__fastcall* ImeUiCallback_DrawFans)(const IMEUI_VERTEX* paVertex, unsigned int uNum);   // 0x0143CDFC

void __fastcall ImeUi_SetWindow(HWND hwnd)
{
    g_hwndCurr = hwnd;
}

static void SetSupportLevel(unsigned long dwImeLevel)   // 0x00CB2D90
{
    if (dwImeLevel < 2 || 3 < dwImeLevel)
        return;
    if (GETPRIMLANG() == LANG_KOREAN)
    {
        dwImeLevel = 3;
    }
    g_dwIMELevel = dwImeLevel;
    // cancel current composition string.
    ImeUi_FinalizeString(false);
    SetCompStringColor();
}

void __fastcall ImeUi_SetSupportLevel(unsigned long dwImeLevel)   // 0x00CB2DD0
{
    if (!g_bInitialized)
        return;
    g_dwIMELevelSaved = dwImeLevel;
    SetSupportLevel(dwImeLevel);
}

bool __fastcall ImeUi_Initialize(HWND hwnd, bool bDisable)
{
    g_hwndMain = hwnd;

    g_osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    GetVersionExA(&g_osi);

    bool bUnicodeImm = false;
    // IMM in Windows NT (including NT 4.0 and later) and Windows 98 and later supports Unicode IMM
    if (g_osi.dwPlatformId == VER_PLATFORM_WIN32_NT ||
        (g_osi.dwMajorVersion > 4) ||
        (g_osi.dwMajorVersion == 4) && (g_osi.dwMinorVersion > 0))
    {
        bUnicodeImm = true;
    }

    HMODULE hImmDll = LoadLibraryA("imm32.dll");
    if (hImmDll)
    {
        _ImmLockIMC = (LPINPUTCONTEXT2 (__stdcall*)(HIMC hIMC))GetProcAddress(hImmDll, "ImmLockIMC");
        _ImmUnlockIMC = (int (__stdcall*)(HIMC hIMC))GetProcAddress(hImmDll, "ImmUnlockIMC");
        _ImmLockIMCC = (void* (__stdcall*)(HIMCC hIMCC))GetProcAddress(hImmDll, "ImmLockIMCC");
        _ImmUnlockIMCC = (int (__stdcall*)(HIMCC hIMCC))GetProcAddress(hImmDll, "ImmUnlockIMCC");
        int (__stdcall* _ImmDisableTextFrameService)(unsigned long) =
            (int (__stdcall*)(unsigned long))GetProcAddress(hImmDll, "ImmDisableTextFrameService");
        if (_ImmDisableTextFrameService)
        {
            _ImmDisableTextFrameService((unsigned long)-1);
        }
    }
    else
    {
        g_bDisableImeCompletely = true;
        return false;
    }

    if (bUnicodeImm)
    {
        _ImmGetCompositionString = ImmGetCompositionStringW;
        _ImmGetCandidateList = ImmGetCandidateListW;
        _GetCandidateList = GetCandidateList;
    }
    else
    {
        _ImmGetCandidateList = ImmGetCandidateListA;
        _ImmGetCompositionString = WA_ImmGetCompositionString;
        _GetCandidateList = WA_GetCandidateList;
    }

    if (!IsWindowUnicode(hwnd))
    {
        _SendCompString = SendCompString_Ansi;
    }
    else
    {
        _SendCompString = SendCompString;
        _SendMessage = SendMessageW;
    }

    // turn init flag on so that subsequent calls to ImeUi functions work.
    g_bInitialized = true;

    static bool bImcObtained = false;               // 0x0143E8DD
    if (!bImcObtained)
    {
        bImcObtained = true;
        ImeUi_SetWindow(g_hwndMain);
        g_himcOrg = ImmGetContext(g_hwndMain);
        ImmReleaseContext(g_hwndMain, g_himcOrg);
    }

    if (!g_himcOrg)
    {
        bDisable = true;
    }

    // the following pointers to function has to be initialized before this function is called.
    if (bDisable ||
        !ImeUiCallback_DrawRect ||
        !ImeUiCallback_Malloc ||
        !ImeUiCallback_Free ||
        !ImeUiCallback_DrawFans)
    {
        g_bDisableImeCompletely = true;
        ImeUi_EnableIme(false);
        g_bInitialized = bDisable;
        return false;
    }

    g_uCaretBlinkTime = GetCaretBlinkTime();

    g_CaretInfo.caretX = 0;
    g_CaretInfo.caretY = 0;
    g_CaretInfo.pFont = 0;
    g_CaretInfo.colorComp = 0;
    g_CaretInfo.colorCand = 0;
    g_CaretInfo.margins.left = 0;
    g_CaretInfo.margins.right = 640;
    g_CaretInfo.margins.top = 0;
    g_CaretInfo.margins.bottom = 480;

    CheckInputLocale();
    SetImeApi();
    ImeUi_SetSupportLevel(2);

    CheckToggleState();
    ImeUi_EnableIme(false);

    return true;
}

// ---- globals ------------------------------------------------------------------------------------------------------------
HWND g_hwndMain; HWND g_hwndCurr; HIMC g_himcOrg; HKL g_hklCurrent; bool g_bInitialized; unsigned long g_bDisableImeCompletely;
OSVERSIONINFOA g_osi; unsigned int g_uCaretBlinkTime; CompStringAttribute g_CaretInfo; unsigned long g_dwIMELevelSaved;
unsigned long g_dwIMELevel;
LPINPUTCONTEXT2 (__stdcall* _ImmLockIMC)(HIMC); int (__stdcall* _ImmUnlockIMC)(HIMC); void* (__stdcall* _ImmLockIMCC)(HIMCC);
int (__stdcall* _ImmUnlockIMCC)(HIMCC);
long (__stdcall* _ImmGetCompositionString)(HIMC, unsigned long, void*, unsigned long);
unsigned long (__stdcall* _ImmGetCandidateList)(HIMC, unsigned long, LPCANDIDATELIST, unsigned long);
unsigned long (__fastcall* _GetCandidateList)(HIMC, unsigned long, LPCANDIDATELIST*);
void (__fastcall* _SendCompString)(); long (__stdcall* _SendMessage)(HWND, unsigned int, unsigned int, long);
void (__fastcall* ImeUiCallback_DrawRect)(int, int, int, int, unsigned long); void* (__fastcall* ImeUiCallback_Malloc)(unsigned int);
void (__fastcall* ImeUiCallback_Free)(void*); void (__fastcall* ImeUiCallback_DrawFans)(const IMEUI_VERTEX*, unsigned int);
// ---- doubles ------------------------------------------------------------------------------------------------------------
static char g_log[512]; static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static unsigned long g_platform = 2, g_major = 5, g_minor = 1; static int g_immLoads = 1; static int g_unicodeWindow = 1;
static HWND__ g_window; static HIMC__ g_context; static HINSTANCE__ g_immModule; static int g_disableTfsArg = 0;
extern "C" int __stdcall GetVersionExA(OSVERSIONINFOA* v)
{ if (v->dwOSVersionInfoSize != sizeof(OSVERSIONINFOA)) Log("badsize"); v->dwPlatformId = g_platform; v->dwMajorVersion = g_major; v->dwMinorVersion = g_minor; return 1; }
extern "C" HMODULE __stdcall LoadLibraryA(const char* name) { Log(name); return g_immLoads && strcmp(name, "imm32.dll") == 0 ? &g_immModule : 0; }
static LPINPUTCONTEXT2 __stdcall FakeLockIMC(HIMC) { return 0; }
static int __stdcall FakeUnlockIMC(HIMC) { return 1; }
static void* __stdcall FakeLockIMCC(HIMCC) { return 0; }
static int __stdcall FakeUnlockIMCC(HIMCC) { return 1; }
static int __stdcall FakeDisableTfs(unsigned long v) { g_disableTfsArg = (int)v; Log("DisableTFS"); return 1; }
extern "C" FARPROC __stdcall GetProcAddress(HMODULE m, const char* name)
{
    if (m != &g_immModule) return 0;
    if (!strcmp(name, "ImmLockIMC")) return (FARPROC)FakeLockIMC;
    if (!strcmp(name, "ImmUnlockIMC")) return (FARPROC)FakeUnlockIMC;
    if (!strcmp(name, "ImmLockIMCC")) return (FARPROC)FakeLockIMCC;
    if (!strcmp(name, "ImmUnlockIMCC")) return (FARPROC)FakeUnlockIMCC;
    if (!strcmp(name, "ImmDisableTextFrameService")) return (FARPROC)FakeDisableTfs;
    return 0;
}
extern "C" int __stdcall IsWindowUnicode(HWND) { return g_unicodeWindow; }
extern "C" long __stdcall SendMessageW(HWND, unsigned int, unsigned int, long) { return 0; }
extern "C" unsigned int __stdcall GetCaretBlinkTime() { Log("BlinkTime"); return 530; }
extern "C" HIMC __stdcall ImmGetContext(HWND h) { Log("GetContext"); return h == &g_window ? &g_context : 0; }
extern "C" int __stdcall ImmReleaseContext(HWND, HIMC h) { Log(h == &g_context ? "Release" : "ReleaseBad"); return 1; }
extern "C" long __stdcall ImmGetCompositionStringW(HIMC, unsigned long, void*, unsigned long) { return 0; }
extern "C" unsigned long __stdcall ImmGetCandidateListW(HIMC, unsigned long, LPCANDIDATELIST, unsigned long) { return 0; }
extern "C" unsigned long __stdcall ImmGetCandidateListA(HIMC, unsigned long, LPCANDIDATELIST, unsigned long) { return 0; }
unsigned long __fastcall GetCandidateList(HIMC, unsigned long, LPCANDIDATELIST*) { return 0; }
unsigned long __fastcall WA_GetCandidateList(HIMC, unsigned long, LPCANDIDATELIST*) { return 0; }
long __stdcall WA_ImmGetCompositionString(HIMC, unsigned long, void*, unsigned long) { return 0; }
void __fastcall SendCompString() { }
void __fastcall SendCompString_Ansi() { }
void __fastcall CheckInputLocale() { Log("CheckInputLocale"); }
void __fastcall SetImeApi() { Log("SetImeApi"); }
void __fastcall ImeUi_FinalizeString(bool bSend) { Log(bSend ? "Finalize(true)" : "Finalize(false)"); }
void __fastcall SetCompStringColor() { Log("SetCompStringColor"); }
void __fastcall CheckToggleState() { Log("CheckToggleState"); }
void __fastcall ImeUi_EnableIme(bool bEnable) { Log(bEnable ? "EnableIme(true)" : "EnableIme(false)"); }
static void __fastcall CbDrawRect(int, int, int, int, unsigned long) { }
static void* __fastcall CbMalloc(unsigned int) { return 0; }
static void __fastcall CbFree(void*) { }
static void __fastcall CbDrawFans(const IMEUI_VERTEX*, unsigned int) { }

int main(void)
{
    int ok = 1;
    ImeUiCallback_DrawRect = CbDrawRect; ImeUiCallback_Malloc = CbMalloc; ImeUiCallback_Free = CbFree; ImeUiCallback_DrawFans = CbDrawFans;
    g_hklCurrent = (HKL)0x04090409;
    memset(&g_CaretInfo, 0x55, sizeof(g_CaretInfo));
    // A: NT, Unicode window, everything present
    if (!ImeUi_Initialize(&g_window, false)) ok = 0;
    if (strcmp(g_log, "imm32.dll DisableTFS GetContext Release BlinkTime CheckInputLocale SetImeApi Finalize(false) SetCompStringColor CheckToggleState EnableIme(false) ") != 0) ok = 0;
    if (g_disableTfsArg != -1 || g_hwndMain != &g_window || g_hwndCurr != &g_window || g_himcOrg != &g_context) ok = 0;
    if (_ImmLockIMC != FakeLockIMC || _ImmUnlockIMC != FakeUnlockIMC || _ImmLockIMCC != FakeLockIMCC || _ImmUnlockIMCC != FakeUnlockIMCC) ok = 0;
    if (_ImmGetCompositionString != ImmGetCompositionStringW || _ImmGetCandidateList != ImmGetCandidateListW || _GetCandidateList != GetCandidateList) ok = 0;
    if (_SendCompString != SendCompString || _SendMessage != SendMessageW) ok = 0;
    if (!g_bInitialized || g_bDisableImeCompletely != 0 || g_uCaretBlinkTime != 530) ok = 0;
    if (g_CaretInfo.caretX != 0 || g_CaretInfo.caretY != 0 || g_CaretInfo.pFont != 0 || g_CaretInfo.colorComp != 0 || g_CaretInfo.colorCand != 0
        || g_CaretInfo.margins.left != 0 || g_CaretInfo.margins.right != 640 || g_CaretInfo.margins.top != 0 || g_CaretInfo.margins.bottom != 480) ok = 0;
    if (g_dwIMELevel != 2 || g_dwIMELevelSaved != 2) ok = 0;
    // B: disabled: the context is fetched once only, g_bInitialized follows bDisable
    g_log[0] = 0; g_bInitialized = false; _SendMessage = 0;
    if (ImeUi_Initialize(&g_window, true)) ok = 0;
    if (strcmp(g_log, "imm32.dll DisableTFS EnableIme(false) ") != 0 || g_bDisableImeCompletely != 1 || !g_bInitialized) ok = 0;
    // C: no imm32
    g_log[0] = 0; g_immLoads = 0; g_bDisableImeCompletely = 0;
    if (ImeUi_Initialize(&g_window, false) || g_bDisableImeCompletely != 1 || strcmp(g_log, "imm32.dll ") != 0) ok = 0;
    g_immLoads = 1;
    // D: Windows 95 (4.0, platform 1), ANSI window, a missing callback: the ANSI entry points, then the failure tail
    g_log[0] = 0; g_platform = 1; g_major = 4; g_minor = 0; g_unicodeWindow = 0; ImeUiCallback_DrawFans = 0; g_bDisableImeCompletely = 0;
    _SendMessage = SendMessageW;
    if (ImeUi_Initialize(&g_window, false) || g_bInitialized || g_bDisableImeCompletely != 1) ok = 0;
    if (_ImmGetCompositionString != WA_ImmGetCompositionString || _ImmGetCandidateList != ImmGetCandidateListA || _GetCandidateList != WA_GetCandidateList
        || _SendCompString != SendCompString_Ansi || _SendMessage != SendMessageW) ok = 0;
    // E: Windows 98 (4.10) counts as Unicode IMM; Korean layout gives level 3
    g_log[0] = 0; g_minor = 10; ImeUiCallback_DrawFans = CbDrawFans; g_bDisableImeCompletely = 0; g_hklCurrent = (HKL)0x04120412;
    if (!ImeUi_Initialize(&g_window, false) || _GetCandidateList != GetCandidateList || g_dwIMELevel != 3 || g_dwIMELevelSaved != 2) ok = 0;
    printf(ok ? "00cb5180_TEST PASS\n" : "00cb5180_TEST FAIL\n");
    return 0;
}