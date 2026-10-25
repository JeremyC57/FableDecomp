// SetImeApi() -- DXUT ImeUi.cpp: look up the current IME's private GetReadingString / ShowReadingWindow exports and hide its reading window
// Leaf: ?SetImeApi@@YIXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB36E0, 179 bytes (+ int3 padding). No arguments (file-static in
// ImeUi.cpp, FableWin.pdb "SetImeApi"; external here so the TU emits it). /GS frame: __security_cookie @ 0x0139C8A8,
// __security_check_cookie @ 0x00BFE9F9. The manifest label is right.
// Callees: imm32 ImmGetIMEFileNameA / ImmGetContext / ImmReleaseContext (import thunks 0x00CB5528 / 0x00CB5540 /
// 0x00CB5534), kernel32 LoadLibraryA / GetProcAddress (IAT 0x0143FE40 / 0x0143FE44, the latter cached in edi),
// the _ShowReadingWindow pointer.
// Globals: _GetReadingString 0x0143E684, _ShowReadingWindow 0x0143CE00, g_hklCurrent 0x0143E8B8, g_hwndCurr 0x0143CF0C.
// Shape: the 2004/2005 SDK body: no UI-less early return, `char szImeFile[MAX_PATH + 1]`, ImmGetIMEFileNameA(..,
// sizeof - 1) `<= 0` (unsigned: `jbe`), plain LoadLibraryA. The four globals are ImeUi.cpp's file statics DEFINED in this
// TU: VC7.1 then loads g_hklCurrent into ecx between the /GS cookie load and store, as retail; declared `extern`, the load
// comes after the cookie store in eax (one byte short).
struct HWND__ { int unused; };   typedef HWND__* HWND;
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;
struct HKL__  { int unused; };   typedef HKL__*  HKL;
struct HINSTANCE__ { int unused; };   typedef HINSTANCE__* HMODULE;
typedef int (__stdcall* FARPROC)();

extern "C" unsigned int __stdcall ImmGetIMEFileNameA(HKL hkl, char* lpszFileName, unsigned int uBufLen);   // import thunk 0x00CB5528
extern "C" HIMC __stdcall ImmGetContext(HWND hwnd);                                         // import thunk 0x00CB5540
extern "C" int __stdcall ImmReleaseContext(HWND hwnd, HIMC himc);                           // import thunk 0x00CB5534
extern "C" __declspec(dllimport) HMODULE __stdcall LoadLibraryA(const char* lpLibFileName);              // IAT 0x0143FE40
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE hModule, const char* lpProcName); // IAT 0x0143FE44

// ImeUi.cpp file statics (defined in this TU as in the SDK source).
static unsigned int (__stdcall* _GetReadingString)(HIMC himc, unsigned int uReadingBufLen, unsigned short* lpwReadingBuf,
                                                   int* pnErrorIndex, int* pfIsVertical, unsigned int* puMaxReadingLen);  // 0x0143E684
static int (__stdcall* _ShowReadingWindow)(HIMC himc, int bShow);                         // 0x0143CE00
static HKL  g_hklCurrent;                           // 0x0143E8B8
static HWND g_hwndCurr;                             // 0x0143CF0C

void __fastcall SetImeApi()
{
    _GetReadingString = 0;
    _ShowReadingWindow = 0;

    char szImeFile[260 /* MAX_PATH */ + 1];
    HKL kl = g_hklCurrent;
    if (ImmGetIMEFileNameA(kl, szImeFile, sizeof(szImeFile) / sizeof(szImeFile[0]) - 1) <= 0)
        return;
    HMODULE hIme = LoadLibraryA(szImeFile);
    if (!hIme)
        return;
    _GetReadingString = (unsigned int (__stdcall*)(HIMC, unsigned int, unsigned short*, int*, int*, unsigned int*))
        (GetProcAddress(hIme, "GetReadingString"));
    _ShowReadingWindow = (int (__stdcall*)(HIMC himc, int))
        (GetProcAddress(hIme, "ShowReadingWindow"));

    if (_ShowReadingWindow)
    {
        HIMC himc;
        if (0 != (himc = ImmGetContext(g_hwndCurr)))
        {
            _ShowReadingWindow(himc, false);
            ImmReleaseContext(g_hwndCurr, himc);
        }
    }
}