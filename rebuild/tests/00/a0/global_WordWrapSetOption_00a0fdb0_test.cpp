// Behaviour test for 0x00A0FDB0 WordWrap_SetOption: stores code page/options, records whether the OS is NT, and caches the code page's default character (wide and narrow).
#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __fastcall
#endif

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
static unsigned long g_platform = 2;
static int g_cpOk = 1;
static int g_sizeSeen = 0;
static int Stub_GetVersionExA(_OSVERSIONINFOA* info) { g_sizeSeen = (int)info->dwOSVersionInfoSize; info->dwPlatformId = g_platform; return 1; }
static int Stub_GetCPInfoExA(unsigned int codePage, unsigned long flags, _cpinfoexA* info)
{
    if (!g_cpOk || flags != 0) return 0;
    info->UnicodeDefaultChar = (unsigned short)(codePage == 1252 ? '?' : 0x30FB);
    return 1;
}
static int Stub_WideCharToMultiByte(unsigned int codePage, unsigned long flags, const unsigned short* wide, int wideCount,
                                    char* multi, int multiSize, const char* defaultChar, int* usedDefault)
{
    if (flags != 0 || wideCount != -1 || multiSize != 3 || defaultChar != 0) return 0;
    *usedDefault = 0;
    multi[0] = (char)(wide[0] < 0x80 ? wide[0] : (codePage == 932 ? 'K' : '?'));
    multi[1] = 0;
    return 2;
}

unsigned int   g_WordWrapCodePage = 0;
unsigned long  g_WordWrapOptions = 0;
int            g_WordWrapIsNT = 0;
unsigned short g_WordWrapDefaultCharW[2];
char           g_WordWrapDefaultCharA[3];

void __fastcall WordWrap_SetOption(unsigned int codePage, unsigned long options)
{
    int usedDefault;
    _OSVERSIONINFOA version;
    _cpinfoexA codePageInfo;

    g_WordWrapCodePage = codePage;
    g_WordWrapOptions = options;
    version.dwOSVersionInfoSize = sizeof(version);
    Stub_GetVersionExA(&version);
    g_WordWrapIsNT = version.dwPlatformId == 2;
    if (!Stub_GetCPInfoExA(g_WordWrapCodePage, 0, &codePageInfo))
        codePageInfo.UnicodeDefaultChar = 0;
    g_WordWrapDefaultCharW[0] = codePageInfo.UnicodeDefaultChar;
    g_WordWrapDefaultCharW[1] = 0;
    g_WordWrapDefaultCharA[0] = g_WordWrapDefaultCharA[1] = g_WordWrapDefaultCharA[2] = 0;
    Stub_WideCharToMultiByte(g_WordWrapCodePage, 0, g_WordWrapDefaultCharW, -1, g_WordWrapDefaultCharA, 3, 0, &usedDefault);
}

int main()
{
    WordWrap_SetOption(1252, 1);
    if (g_WordWrapCodePage != 1252 || g_WordWrapOptions != 1 || g_WordWrapIsNT != 1 || g_sizeSeen != (int)sizeof(_OSVERSIONINFOA)
        || g_WordWrapDefaultCharW[0] != '?' || g_WordWrapDefaultCharW[1] != 0 || std::strcmp(g_WordWrapDefaultCharA, "?") != 0)
        { std::printf("00a0fdb0_TEST FAIL\n"); return 1; }
    g_platform = 1;
    WordWrap_SetOption(932, 0);
    if (g_WordWrapCodePage != 932 || g_WordWrapOptions != 0 || g_WordWrapIsNT != 0
        || g_WordWrapDefaultCharW[0] != 0x30FB || std::strcmp(g_WordWrapDefaultCharA, "K") != 0)
        { std::printf("00a0fdb0_TEST FAIL\n"); return 1; }
    g_cpOk = 0;
    WordWrap_SetOption(65001, 3);
    if (g_WordWrapDefaultCharW[0] != 0 || g_WordWrapDefaultCharA[0] != 0 || g_WordWrapOptions != 3)
        { std::printf("00a0fdb0_TEST FAIL\n"); return 1; }
    std::printf("00a0fdb0_TEST PASS\n");
    return 0;
}