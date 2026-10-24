// WordWrap_SetOption(unsigned int codePage, unsigned long options) -- remember the code page and option bits, note whether the OS is NT, and cache the code page's default character in wide and multibyte form
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0FDB0, length 196 bytes (+ int3 padding), /GS frame (cookie 0x0139C8A8).
// Callees: KERNEL32 GetVersionExA (IAT 0x0143FE70), GetCPInfoExA (IAT 0x0143FE6C), WideCharToMultiByte (IAT 0x0143FD98),
// __security_check_cookie @ 0x00BFE9F9.
// Globals (the DirectX-sample word-wrap module's statics): 0x01397098 code page, 0x013CE760 option bits (bit 0 tested by
// IsNonEndingChar 0x00A10170, bit 2 by IsEastAsianChar 0x00A10240), 0x013CE764 "running on NT", 0x013CE768 the default
// character as a 2-WCHAR string, 0x013CE76C its 3-byte multibyte form.
// Callers: CSubtitleRenderer::FormatTextForVWindow (landed 0x0048F50C: `WordWrap_SetOption(0, 1)`), NUISystem::CText::FitInWindow,
// CProgressDisplay::StartProgress, CMovieSubtitles::ParseSubtitles.
// Shape: `__fastcall` (code page in ecx, options in edx, plain `ret`) with a /GS buffer (CPINFOEXA's CodePageName[260]):
// frame = the WideCharToMultiByte used-default flag at +0, OSVERSIONINFOA (0x94) at +4, CPINFOEXA (0x11C) at +0x98, cookie at
// +0x1B4; the code page global is re-read for each API call; a failed GetCPInfoExA zeroes UnicodeDefaultChar; the
// multibyte buffer is cleared by a chained `c[0] = c[1] = c[2] = 0` (stored +2, +1, +0 like retail).
// Leaf: ?WordWrap_SetOption@@YIXIK@Z
// Flags: /GS
struct _OSVERSIONINFOA {
    unsigned long dwOSVersionInfoSize;
    unsigned long dwMajorVersion;
    unsigned long dwMinorVersion;
    unsigned long dwBuildNumber;
    unsigned long dwPlatformId;
    char          szCSDVersion[128];
};
struct _cpinfoexA {
    unsigned int   MaxCharSize;
    unsigned char  DefaultChar[2];
    unsigned char  LeadByte[12];
    unsigned short UnicodeDefaultChar;
    unsigned int   CodePage;
    char           CodePageName[260];
};
extern "C" __declspec(dllimport) int __stdcall GetVersionExA(_OSVERSIONINFOA* info);                                  // IAT 0x0143FE70
extern "C" __declspec(dllimport) int __stdcall GetCPInfoExA(unsigned int codePage, unsigned long flags, _cpinfoexA* info); // IAT 0x0143FE6C
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int codePage, unsigned long flags,
                                                                  const unsigned short* wide, int wideCount, char* multi,
                                                                  int multiSize, const char* defaultChar, int* usedDefault);   // IAT 0x0143FD98

extern unsigned int   g_WordWrapCodePage;            // 0x01397098
extern unsigned long  g_WordWrapOptions;             // 0x013CE760
extern int            g_WordWrapIsNT;                // 0x013CE764
extern unsigned short g_WordWrapDefaultCharW[2];     // 0x013CE768
extern char           g_WordWrapDefaultCharA[3];     // 0x013CE76C

void __fastcall WordWrap_SetOption(unsigned int codePage, unsigned long options)
{
    int usedDefault;
    _OSVERSIONINFOA version;
    _cpinfoexA codePageInfo;

    g_WordWrapCodePage = codePage;
    g_WordWrapOptions = options;
    version.dwOSVersionInfoSize = sizeof(version);
    GetVersionExA(&version);
    g_WordWrapIsNT = version.dwPlatformId == 2;   // VER_PLATFORM_WIN32_NT
    if (!GetCPInfoExA(g_WordWrapCodePage, 0, &codePageInfo))
        codePageInfo.UnicodeDefaultChar = 0;
    g_WordWrapDefaultCharW[0] = codePageInfo.UnicodeDefaultChar;
    g_WordWrapDefaultCharW[1] = 0;
    g_WordWrapDefaultCharA[0] = g_WordWrapDefaultCharA[1] = g_WordWrapDefaultCharA[2] = 0;
    WideCharToMultiByte(g_WordWrapCodePage, 0, g_WordWrapDefaultCharW, -1, g_WordWrapDefaultCharA, 3, 0, &usedDefault);
}