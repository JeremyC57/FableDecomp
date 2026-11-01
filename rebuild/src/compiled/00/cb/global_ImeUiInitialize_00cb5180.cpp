// ImeUi_Initialize(HWND hwnd, bool bDisable) -- DirectX SDK (2004) ImeUi.cpp: record the main window, pick the Unicode or ANSI IMM entry points from the OS version, load imm32.dll's private IMC lock exports and disable its text frame service, fetch the window's original input context once, then (unless disabled, no context, or a drawing/memory callback is missing) set the caret blink time and defaults, the input locale, the IME private API and support level 2, refresh the toggle state and leave the IME attached off
// Leaf: ?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB5180, 623 bytes, __fastcall (hwnd in ecx, bDisable in dl, homed in the one frame
//   word), plain `ret`; ImeUi.cpp is a /Gr TU.
// Identity (proven): FableWin.pdb ?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z (ImeUi.cpp's exported entry; the landed
//   CIME::Initialise 0x00405650 calls it as "ImeUI_Initialize" after installing the callbacks). Body: the 2004 DirectX SDK
//   ImeUi_Initialize -- older than the DXUT text: no `if (g_bInitialized) return true` guard, the OS-version `bUnicodeImm`
//   choice between the W entry points and the WA_ wrappers, imm32 in a local handle, a once-flag around
//   ImeUi_SetWindow / ImmGetContext, the DrawRect / Malloc / Free / DrawFans callback test, SetImeApi() and
//   ImeUi_SetSupportLevel(2) in place of OnInputLangChangeWorker and the TSF sinks.
// Callees: GetVersionExA [0x0143FE70], LoadLibraryA [0x0143FE40], GetProcAddress [0x0143FE44] (cached in edi),
//   the ImmDisableTextFrameService export (-1), IsWindowUnicode [0x014402B0], imm32 ImmGetContext / ImmReleaseContext
//   (thunks 0x00CB5540 / 0x00CB5534), GetCaretBlinkTime [0x014402B8], CheckInputLocale 0x00CB37A0, SetImeApi 0x00CB36E0,
//   ImeUi_FinalizeString 0x00CB2C60 and SetCompStringColor 0x00CB2D20 (inside the inlined ImeUi_SetSupportLevel(2),
//   0x00CB2DD0, with the static SetSupportLevel 0x00CB2D90 inlined into it), CheckToggleState 0x00CB47C0, ImeUi_EnableIme
//   0x00CB5130. Function addresses stored: ImmGetCompositionStringW 0x00CB5570, ImmGetCandidateListW 0x00CB556A,
//   ImmGetCandidateListA 0x00CB551C, GetCandidateList 0x00CB2670, WA_ImmGetCompositionString 0x00CB2440,
//   WA_GetCandidateList 0x00CB2310, SendCompString 0x00CB25F0 (Unicode) / its ANSI twin 0x00CB2230, SendMessageW
//   [0x014402B4]. Strings: "imm32.dll" 0x012BFBC8, "ImmLockIMC" 0x012BFBBC, "ImmUnlockIMC" 0x012BFBAC, "ImmLockIMCC"
//   0x012BFBA0, "ImmUnlockIMCC" 0x012BFB90, "ImmDisableTextFrameService" 0x012BFB74.
// Globals: g_hwndMain 0x0143E510, g_osi 0x0143CD68 (OSVERSIONINFOA), _ImmLockIMC 0x0143E638, _ImmUnlockIMC 0x0143CD04,
//   _ImmLockIMCC 0x0143E620, _ImmUnlockIMCC 0x0143E680, _ImmGetCompositionString 0x0143CE04, _ImmGetCandidateList
//   0x0143CD5C, _GetCandidateList 0x0143CD00, _SendCompString 0x0143E61C, _SendMessage 0x0143E8C4, g_bInitialized
//   0x0143E8AE, the once flag 0x0143E8DD (descriptive name, only this function touches it), g_hwndCurr 0x0143CF0C,
//   g_himcOrg 0x0143E634, ImeUiCallback_DrawRect 0x0143CD64, ImeUiCallback_Malloc 0x0143E514, ImeUiCallback_Free
//   0x0143CF08, ImeUiCallback_DrawFans 0x0143CDFC (CIME::Initialise's "g_ImeDrawFans"), g_bDisableImeCompletely
//   0x0143E89C, g_uCaretBlinkTime 0x0143E8A4, g_CaretInfo 0x0143CD2C, g_hklCurrent 0x0143E8B8, g_dwIMELevelSaved
//   0x0143CD20, g_dwIMELevel 0x0143CD1C. Variable names are the SDK source's (no PDB data symbols).
// Shape: `bUnicodeImm` in bl, the imm32 handle in esi, GetProcAddress in edi; `if (!g_himcOrg) bDisable = true;` folds
//   into a direct jump to the shared failure tail, which stores `g_bInitialized = bDisable` from bl.
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

extern "C" __declspec(dllimport) int __stdcall GetVersionExA(OSVERSIONINFOA* lpVersionInformation);        // IAT 0x0143FE70
extern "C" __declspec(dllimport) HMODULE __stdcall LoadLibraryA(const char* lpLibFileName);              // IAT 0x0143FE40
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE hModule, const char* lpProcName); // IAT 0x0143FE44
extern "C" __declspec(dllimport) int __stdcall IsWindowUnicode(HWND hwnd);                               // IAT 0x014402B0
extern "C" __declspec(dllimport) long __stdcall SendMessageW(HWND hwnd, unsigned int msg, unsigned int wp, long lp); // IAT 0x014402B4
extern "C" __declspec(dllimport) unsigned int __stdcall GetCaretBlinkTime();                             // IAT 0x014402B8
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