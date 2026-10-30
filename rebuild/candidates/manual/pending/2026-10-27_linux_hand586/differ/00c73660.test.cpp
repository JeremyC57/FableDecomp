// behaviour test for the parked 0x00C73660 DetectWindowsVersion (copy of the logic over stub version calls)
#include <cstdio>
#include <cstring>
struct _OSVERSIONINFOA { unsigned int dwOSVersionInfoSize, dwMajorVersion, dwMinorVersion, dwBuildNumber, dwPlatformId; char szCSDVersion[128]; };   // unsigned int: 0x94 bytes on a 64-bit host too
static unsigned long g_version = 0, g_build9x = 0;
static unsigned long GetVersion(void) { return g_version; }
static int GetVersionExA(_OSVERSIONINFOA* info) { if (info->dwOSVersionInfoSize != 0x94) return 0; info->dwBuildNumber = g_build9x; return 1; }
static unsigned char g_defStringInit;
static bool g_isWin32s, g_isWindows95, g_isWindows98, g_isWindows98SE, g_isWindowsMe, g_isWindowsNT3, g_isWindowsNT4, g_isWindows2000, g_isWindowsXP;
static unsigned long g_windowsBuildNumber, g_windowsMajorVersion, g_windowsMinorVersion;
static void DetectWindowsVersion()
{
    unsigned long version = GetVersion();
    unsigned long major = (unsigned char)version;
    unsigned long minor = (unsigned char)((unsigned short)version >> 8);
    g_windowsMajorVersion = major;
    g_windowsMinorVersion = minor;
    if (version < 0x80000000) {
        g_windowsBuildNumber = version >> 16;
        switch (major) {
        case 3: g_isWindowsNT3 = true; break;
        case 4: g_isWindowsNT4 = true; break;
        case 5: if (minor == 0) g_isWindows2000 = true; else if (minor == 1) g_isWindowsXP = true; break;
        }
    } else if (major < 4) {
        g_windowsBuildNumber = (version >> 16) & 0x7FFF;
        g_isWin32s = true;
    } else {
        _OSVERSIONINFOA info;
        info.dwOSVersionInfoSize = sizeof(info);
        GetVersionExA(&info);
        g_windowsBuildNumber = (unsigned short)info.dwBuildNumber;
        if (g_windowsMinorVersion == 0) g_isWindows95 = true;
        else if (g_windowsMinorVersion <= 10 && g_windowsBuildNumber <= 2222) {
            if (g_windowsBuildNumber == 2222) g_isWindows98SE = true; else g_isWindows98 = true;
        } else g_isWindowsMe = true;
    }
    g_defStringInit = 1;
}
static void Reset() { g_defStringInit = 0; g_isWin32s = g_isWindows95 = g_isWindows98 = g_isWindows98SE = g_isWindowsMe = g_isWindowsNT3 = g_isWindowsNT4 = g_isWindows2000 = g_isWindowsXP = false; }
int main()
{
    Reset(); g_version = (2600u << 16) | (1 << 8) | 5;          // XP 5.1.2600
    DetectWindowsVersion();
    if (!g_isWindowsXP || g_isWindows2000 || g_windowsBuildNumber != 2600 || g_windowsMajorVersion != 5 || g_windowsMinorVersion != 1 || !g_defStringInit) { std::printf("00c73660_TEST FAIL\n"); return 1; }
    Reset(); g_version = (1381u << 16) | 4;                     // NT 4.0
    DetectWindowsVersion();
    if (!g_isWindowsNT4 || g_windowsBuildNumber != 1381) { std::printf("00c73660_TEST FAIL\n"); return 1; }
    Reset(); g_version = 0x80000000u | (10u << 8) | 4; g_build9x = 0x040A08AE;   // 98 SE: 4.10.2222
    DetectWindowsVersion();
    if (!g_isWindows98SE || g_isWindows98 || g_windowsBuildNumber != 2222) { std::printf("00c73660_TEST FAIL\n"); return 1; }
    Reset(); g_build9x = 1998;                                  // 98: 4.10.1998
    DetectWindowsVersion();
    if (!g_isWindows98 || g_isWindows98SE) { std::printf("00c73660_TEST FAIL\n"); return 1; }
    Reset(); g_version = 0x80000000u | (90u << 8) | 4; g_build9x = 3000;          // Me: 4.90
    DetectWindowsVersion();
    if (!g_isWindowsMe) { std::printf("00c73660_TEST FAIL\n"); return 1; }
    Reset(); g_version = 0x80000000u | 4; g_build9x = 950;                         // 95: 4.0
    DetectWindowsVersion();
    if (!g_isWindows95) { std::printf("00c73660_TEST FAIL\n"); return 1; }
    Reset(); g_version = 0x80000000u | (0x8123u << 16) | (11u << 8) | 3;          // Win32s 3.11
    DetectWindowsVersion();
    if (!g_isWin32s || g_windowsBuildNumber != 0x0123) { std::printf("00c73660_TEST FAIL\n"); return 1; }
    std::printf("00c73660_TEST PASS\n");
    return 0;
}
