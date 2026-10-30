#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#endif
// ImeUi_ToggleLanguageBar(int bRestore) -- DXUT ImeUi.cpp: minimise/hide the floating TSF language bar (and close the IME status window) when the game takes focus, restore both when it gives focus back
// Leaf: ?ImeUi_ToggleLanguageBar@@YIXH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB3570, 348 bytes. Name: `?ImeUi_ToggleLanguageBar@@YIXH@Z` in both the Ego_r.pdb
// and FableWin.pdb name sets (public __fastcall, BOOL argument in ecx, as retail's `mov ebp,ecx`); body = the DX SDK
// DXUT ImeUi.cpp function. Sole caller 0x00404CBC. Manifest label correct.
// Callees: ImmGetDefaultIMEWnd import thunk 0x00CB5546 (imm32 through the non-dllimport thunk, like the landed ImeUi
// rows), dllimport SendMessageA [0x014402F0], CoInitialize [0x01440644], CoCreateInstance [0x01440640],
// CoUninitialize [0x01440648], lstrlenA [0x0143FD88]; ITfLangBarMgr::ShowFloating (slot 0x28),
// GetShowFloatingStatus (0x2C), Release (8).
// Globals: function-local statics prevRestore 0x013AC814 (= TRUE) and iShowStatusWindow 0x013AC810 (= -1) in .data;
// g_dwPrevFloat 0x0143CD24; g_osi (OSVERSIONINFOA) 0x0143CD68 (dwMajorVersion +4 = 0x0143CD6C, dwMinorVersion +8,
// dwPlatformId +0x10 = 0x0143CD78, szCSDVersion +0x14 = 0x0143CD7C); g_hwndCurr 0x0143CF0C; uuid.lib
// CLSID_TF_LangBarMgr 0x012C0C74 / IID_ITfLangBarMgr 0x012C0FF4 (bytes checked against msctf.h).
struct HWND__ { int unused; };   typedef HWND__* HWND;
typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned long ULONG;
typedef long HRESULT;
struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
typedef _GUID GUID;
typedef _GUID IID;
typedef _GUID CLSID;

typedef struct _OSVERSIONINFOA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    char  szCSDVersion[128];
} OSVERSIONINFOA;

struct IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObject) = 0;
    virtual ULONG __stdcall AddRef() = 0;
    virtual ULONG __stdcall Release() = 0;
};
struct ITfLangBarMgr : public IUnknown {
    virtual HRESULT __stdcall AdviseEventSink(void* pSink, HWND hwnd, DWORD dwFlags, DWORD* pdwCookie) = 0;
    virtual HRESULT __stdcall UnadviseEventSink(DWORD dwCookie) = 0;
    virtual HRESULT __stdcall GetThreadMarshalInterface(DWORD dwThreadId, DWORD dwType, const IID& riid, IUnknown** ppunk) = 0;
    virtual HRESULT __stdcall GetThreadLangBarItemMgr(DWORD dwThreadId, void** pplbi, DWORD* pdwThreadid) = 0;
    virtual HRESULT __stdcall GetInputProcessorProfiles(DWORD dwThreadId, void** ppaip, DWORD* pdwThreadid) = 0;
    virtual HRESULT __stdcall RestoreLastFocus(DWORD* pdwThreadId, BOOL fPrev) = 0;
    virtual HRESULT __stdcall SetModalInput(void* pSink, DWORD dwThreadId, DWORD dwFlags) = 0;
    virtual HRESULT __stdcall ShowFloating(DWORD dwFlags) = 0;
    virtual HRESULT __stdcall GetShowFloatingStatus(DWORD* pdwFlags) = 0;
};


static int g_log[64]; static int g_nlog = 0;
static void logv(int v) { if (g_nlog < 64) g_log[g_nlog++] = v; }
static HWND__ g_imeWnd;
static HWND g_defWnd = &g_imeWnd;
static int g_lstrlen = 0;
static HRESULT g_coinit = 0, g_cocreate = 0;
static DWORD g_floatStatus = 0;

struct CFakeLangBar : public ITfLangBarMgr {
    HRESULT __stdcall QueryInterface(const IID&, void**) { return 0; }
    ULONG __stdcall AddRef() { return 1; }
    ULONG __stdcall Release() { logv(900); return 0; }
    HRESULT __stdcall AdviseEventSink(void*, HWND, DWORD, DWORD*) { return 0; }
    HRESULT __stdcall UnadviseEventSink(DWORD) { return 0; }
    HRESULT __stdcall GetThreadMarshalInterface(DWORD, DWORD, const IID&, IUnknown**) { return 0; }
    HRESULT __stdcall GetThreadLangBarItemMgr(DWORD, void**, DWORD*) { return 0; }
    HRESULT __stdcall GetInputProcessorProfiles(DWORD, void**, DWORD*) { return 0; }
    HRESULT __stdcall RestoreLastFocus(DWORD*, BOOL) { return 0; }
    HRESULT __stdcall SetModalInput(void*, DWORD, DWORD) { return 0; }
    HRESULT __stdcall ShowFloating(DWORD f) { logv(1000 + (int)f); return 0; }
    HRESULT __stdcall GetShowFloatingStatus(DWORD* p) { *p = g_floatStatus; logv(800); return 0; }
};
static CFakeLangBar g_lb;

extern "C" HWND __stdcall ImmGetDefaultIMEWnd(HWND) { return g_defWnd; }
extern "C" long __stdcall SendMessageA(HWND, unsigned int Msg, unsigned int wParam, long) { logv((int)(Msg * 0 + wParam)); return 0; }
extern "C" HRESULT __stdcall CoInitialize(void*) { logv(700); return g_coinit; }
extern "C" HRESULT __stdcall CoCreateInstance(const CLSID&, IUnknown*, DWORD, const IID&, void** ppv)
{ *ppv = g_cocreate >= 0 ? (void*)static_cast<ITfLangBarMgr*>(&g_lb) : 0; return g_cocreate; }
extern "C" void __stdcall CoUninitialize() { logv(701); }
extern "C" int __stdcall lstrlenA(const char* s) { ++g_lstrlen; int n = 0; while (s[n]) ++n; return n; }

extern "C" const CLSID CLSID_TF_LangBarMgr = { 0xebb08c45, 0x6c4a, 0x4fdc, { 0xae, 0x53, 0x4e, 0xb8, 0xc4, 0xc7, 0xdb, 0x8e } };
extern "C" const IID   IID_ITfLangBarMgr   = { 0x87955690, 0xe627, 0x11d2, { 0x8d, 0xdb, 0x00, 0x10, 0x5a, 0x27, 0x99, 0xb5 } };
OSVERSIONINFOA g_osi;
HWND           g_hwndCurr;
DWORD          g_dwPrevFloat;

#define TRUE  1
#define NULL  0
#define VER_PLATFORM_WIN32_NT   2
#define WM_IME_CONTROL          0x0283
#define IMC_CLOSESTATUSWINDOW   0x0021
#define IMC_OPENSTATUSWINDOW    0x0022
#define CLSCTX_INPROC_SERVER    0x1
#define TF_SFT_HIDDEN           0x00000008
#define TF_SFT_DESKBAND         0x00000800
#define SUCCEEDED(hr)           ((HRESULT)(hr) >= 0)

static inline bool IsNT() { return g_osi.dwPlatformId == VER_PLATFORM_WIN32_NT; }

void __fastcall ImeUi_ToggleLanguageBar(BOOL bRestore)
{
    static BOOL prevRestore = TRUE;
    bool bCheck = ( prevRestore == TRUE || bRestore == TRUE );
    prevRestore = bRestore;
    if ( !bCheck )
        return;

    static int iShowStatusWindow = -1;
    if ( iShowStatusWindow == -1 )
    {
        iShowStatusWindow = IsNT() && g_osi.dwMajorVersion >= 5 &&
            ( g_osi.dwMinorVersion > 1 || ( g_osi.dwMinorVersion == 1 && lstrlenA( g_osi.szCSDVersion ) ) ) ? 1 : 0;
    }
    HWND hwndImeDef = ImmGetDefaultIMEWnd(g_hwndCurr);
    if ( hwndImeDef && bRestore && iShowStatusWindow )
        SendMessageA(hwndImeDef, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0);
    HRESULT hr;
    hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        ITfLangBarMgr* plbm = NULL;
        hr = CoCreateInstance(CLSID_TF_LangBarMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfLangBarMgr, (void**)&plbm);
        if (SUCCEEDED(hr) && plbm)
        {
            DWORD dwCur;
            ULONG uRc;
            if (SUCCEEDED(hr))
            {
                if (bRestore)
                {
                    if (g_dwPrevFloat)
                        hr = plbm->ShowFloating(g_dwPrevFloat);
                }
                else
                {
                    hr = plbm->GetShowFloatingStatus(&dwCur);
                    if (SUCCEEDED(hr))
                        g_dwPrevFloat = dwCur;
                    if ( !( g_dwPrevFloat & TF_SFT_DESKBAND ) )
                    {
                        hr = plbm->ShowFloating(TF_SFT_HIDDEN);
                    }
                }
            }
            uRc = plbm->Release();
        }
        CoUninitialize();
    }

    if ( hwndImeDef && !bRestore )
    {
        // The following OPENSTATUSWINDOW is required to hide ATOK16 toolbar (FS9:#7546)
        SendMessageA(hwndImeDef, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0);
        SendMessageA(hwndImeDef, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0);
    }
}

static bool expect(const int* v, int n)
{
    if (g_nlog != n) return false;
    for (int i = 0; i < n; ++i) if (g_log[i] != v[i]) return false;
    return true;
}

int main()
{
    g_osi.dwPlatformId = 2; g_osi.dwMajorVersion = 5; g_osi.dwMinorVersion = 1; std::strcpy(g_osi.szCSDVersion, "Service Pack 2");
    // 1: game takes focus (bRestore = 0): GetShowFloatingStatus, hide, release, uninit, OPEN then CLOSE status window
    g_floatStatus = 0x1; g_nlog = 0;
    ImeUi_ToggleLanguageBar(0);
    { int e[] = { 700, 800, 1008, 900, 701, 0x22, 0x21 }; if (!expect(e, 7) || g_dwPrevFloat != 1 || g_lstrlen != 1) { std::printf("00cb3570_TEST FAIL 1\n"); return 1; } }
    // 2: again with bRestore = 0: prevRestore was 0 and bRestore is 0, nothing happens
    g_nlog = 0;
    ImeUi_ToggleLanguageBar(0);
    if (g_nlog != 0) { std::printf("00cb3570_TEST FAIL 2\n"); return 1; }
    // 3: restore: open status window (cached iShowStatusWindow = 1, lstrlenA not called again), ShowFloating(prev)
    g_nlog = 0;
    ImeUi_ToggleLanguageBar(1);
    { int e[] = { 0x22, 700, 1001, 900, 701 }; if (!expect(e, 5) || g_lstrlen != 1) { std::printf("00cb3570_TEST FAIL 3\n"); return 1; } }
    // 4: lose focus with a deskband language bar: status read, no hide
    g_floatStatus = 0x800; g_nlog = 0;
    ImeUi_ToggleLanguageBar(0);
    { int e[] = { 700, 800, 900, 701, 0x22, 0x21 }; if (!expect(e, 6) || g_dwPrevFloat != 0x800) { std::printf("00cb3570_TEST FAIL 4\n"); return 1; } }
    // 5: CoCreateInstance fails and no default IME window: only CoInitialize / CoUninitialize
    g_cocreate = -1; g_defWnd = 0; g_nlog = 0;
    ImeUi_ToggleLanguageBar(1);
    { int e[] = { 700, 701 }; if (!expect(e, 2)) { std::printf("00cb3570_TEST FAIL 5\n"); return 1; } }
    std::printf("00cb3570_TEST PASS\n");
    return 0;
}