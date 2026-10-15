// Behaviour test for CDriveManager::DriveExists @ 0x009E4AE0 (_wchdir replaced by a recording fake)
#include <cstdio>
typedef unsigned short wchar16;
static wchar16 g_cwd[8]; static int g_chdirs = 0; static bool g_driveD = true;
static int FakeChdir(const wchar16* path)
{
    ++g_chdirs;
    if (path[1] == 0x3a && path[2] == 0x5c && path[3] == 0 && path[0] == 'Z') return -1;   // no such drive
    if (path[0] == 'D' && !g_driveD) return -1;
    int i = 0; while (path[i]) { g_cwd[i] = path[i]; ++i; } g_cwd[i] = 0;
    return 0;
}
struct CWideString {
    wchar16 m_data[8];
    operator const wchar16*() { return m_data; }
};
static CWideString GetCurrentWorkingDirectory() { CWideString s; int i = 0; while ((s.m_data[i] = g_cwd[i]) != 0) ++i; return s; }
struct CDriveManager {
    long DriveExists(wchar16 driveLetter)
    {
        CWideString workingDirectory = GetCurrentWorkingDirectory();
        wchar16 root[4];
        root[0] = driveLetter; root[1] = 0x3a; root[2] = 0x5c; root[3] = 0;
        if (FakeChdir(root) == 0) {
            FakeChdir(workingDirectory);
            return 1;
        }
        return 3;
    }
};
int main()
{
    g_cwd[0] = 'C'; g_cwd[1] = 0x3a; g_cwd[2] = 0x5c; g_cwd[3] = 'x'; g_cwd[4] = 0;
    CDriveManager m;
    if (m.DriveExists('D') != 1 || g_chdirs != 2 || g_cwd[0] != 'C' || g_cwd[3] != 'x') { std::printf("009e4ae0_TEST FAIL\n"); return 1; }
    if (m.DriveExists('Z') != 3 || g_chdirs != 3 || g_cwd[0] != 'C') { std::printf("009e4ae0_TEST FAIL\n"); return 1; }
    std::printf("009e4ae0_TEST PASS\n");
    return 0;
}
