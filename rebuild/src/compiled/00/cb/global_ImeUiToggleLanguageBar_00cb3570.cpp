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

extern "C" HWND __stdcall ImmGetDefaultIMEWnd(HWND hwnd);                                    // import thunk 0x00CB5546
extern "C" __declspec(dllimport) long __stdcall SendMessageA(HWND hWnd, unsigned int Msg, unsigned int wParam, long lParam);
extern "C" __declspec(dllimport) HRESULT __stdcall CoInitialize(void* pvReserved);
extern "C" __declspec(dllimport) HRESULT __stdcall CoCreateInstance(const CLSID& rclsid, IUnknown* pUnkOuter,
                                                                   DWORD dwClsContext, const IID& riid, void** ppv);
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();
extern "C" __declspec(dllimport) int __stdcall lstrlenA(const char* lpString);

extern "C" const CLSID CLSID_TF_LangBarMgr;         // 0x012C0C74 (uuid.lib)
extern "C" const IID   IID_ITfLangBarMgr;           // 0x012C0FF4 (uuid.lib)

extern OSVERSIONINFOA g_osi;                        // 0x0143CD68
extern HWND           g_hwndCurr;                   // 0x0143CF0C
extern DWORD          g_dwPrevFloat;                // 0x0143CD24

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