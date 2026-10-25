// Behaviour test for IsWindows9x (0x00C5DA70): true when any of the four 9x flags is set.
#include <cstdio>
static bool g_isWindows95, g_isWindows98, g_isWindows98SE, g_isWindowsMe;
static bool IsWindows9x() { return g_isWindows95 || g_isWindows98 || g_isWindows98SE || g_isWindowsMe; }
int main()
{
    int ok = !IsWindows9x();
    g_isWindows98SE = true; ok = ok && IsWindows9x(); g_isWindows98SE = false;
    g_isWindowsMe = true; ok = ok && IsWindows9x(); g_isWindowsMe = false;
    g_isWindows95 = true; ok = ok && IsWindows9x(); g_isWindows95 = false;
    g_isWindows98 = true; ok = ok && IsWindows9x(); g_isWindows98 = false;
    ok = ok && !IsWindows9x();
    if (!ok) { std::printf("00c5da70_TEST FAIL\n"); return 1; }
    std::printf("00c5da70_TEST PASS\n");
    return 0;
}