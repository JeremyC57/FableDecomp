// Test for GetImeId 0x00CB28F0: the layout gate (only the three legacy CHT/CHS layouts), the version word build, the known
// version table per language, the cached (version | language, file version LS) pair and the zeroing on every failure.
#include <cstdio>
typedef unsigned long DWORD; typedef unsigned int UINT; typedef unsigned char BYTE;
#define MAKEIMEVERSION(major, minor) ((DWORD)(((BYTE)(major) << 24) | ((BYTE)(minor) << 16)))
static DWORD g_hkl; static DWORD g_fileVersionMS; static DWORD g_fileVersionLS; static bool g_hasReadingString; static bool g_fileKnown;
static DWORD GetImeId(UINT uIndex) {
    static DWORD hklPrev = 0; static DWORD dwRet[2] = { 0, 0 };
    if (uIndex >= 2) return 0;
    if (hklPrev == g_hkl) return dwRet[uIndex];
    hklPrev = g_hkl;
    DWORD dwLang = g_hkl & 0xffff;
    if ((g_hkl == 0xE0080404 || g_hkl == 0xE0090404 || g_hkl == 0xE00E0804) && (g_hasReadingString || g_fileKnown)) {
        DWORD dwVer = g_fileVersionMS;
        dwVer = (dwVer & 0x00ff0000) << 8 | (dwVer & 0x000000ff) << 16;
        if (g_hasReadingString ||
            dwLang == 0x0404 && (dwVer == MAKEIMEVERSION(4, 2) || dwVer == MAKEIMEVERSION(4, 3) || dwVer == MAKEIMEVERSION(4, 4) ||
                                 dwVer == MAKEIMEVERSION(5, 0) || dwVer == MAKEIMEVERSION(5, 1) || dwVer == MAKEIMEVERSION(5, 2) ||
                                 dwVer == MAKEIMEVERSION(6, 0)) ||
            dwLang == 0x0804 && (dwVer == MAKEIMEVERSION(4, 1) || dwVer == MAKEIMEVERSION(4, 2) || dwVer == MAKEIMEVERSION(5, 3))) {
            dwRet[0] = dwVer | dwLang; dwRet[1] = g_fileVersionLS; return dwRet[0];
        }
    }
    dwRet[0] = dwRet[1] = 0; return dwRet[uIndex];
}
int main() {
    bool ok = GetImeId(2) == 0;
    g_hkl = 0xE0080404; g_fileVersionMS = 0x00050002; g_fileVersionLS = 0x1234; g_fileKnown = true;   // 5.2 New Phonetic
    ok = ok && GetImeId(0) == 0x05020404 && GetImeId(1) == 0x1234;                                     // cached pair
    g_hkl = 0xE00E0804; g_fileVersionMS = 0x00040002;                                                  // MSPY 4.2
    ok = ok && GetImeId(0) == 0x04020804;
    g_hkl = 0xE00E0804 + 0; g_fileVersionMS = 0x00040003; g_hkl = 0xE0090404;                          // CHT 4.3 ok
    ok = ok && GetImeId(0) == 0x04030404;
    g_hkl = 0xE00E0804; g_fileVersionMS = 0x00040003;                                                  // CHS 4.3 unknown -> zero
    ok = ok && GetImeId(0) == 0 && GetImeId(1) == 0;
    g_hkl = 0x04090409; g_hasReadingString = true;                                                     // not a legacy layout
    ok = ok && GetImeId(1) == 0;
    g_hkl = 0xE0080404; g_fileKnown = false; g_fileVersionMS = 0x00090009;                             // reading-string API: any file/version
    ok = ok && GetImeId(0) == 0x09090404;
    if (ok) printf("00cb28f0_TEST PASS\n"); else printf("00cb28f0_TEST FAIL\n");
    return ok ? 0 : 1;
}