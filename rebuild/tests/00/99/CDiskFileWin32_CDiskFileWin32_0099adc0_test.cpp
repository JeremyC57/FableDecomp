#include <cstdio>
static int g_base = 0, g_str = 0; static const void* g_openPath = 0; static long g_openAccess = -1; static unsigned long g_openFlags = 0; static bool g_openedAtOpen = true;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CWideString { void* PStringData; CWideString() : PStringData(0) { ++g_str; } };
enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CDiskFileWin32 : CBase {
    virtual void v00() { }
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened; CWideString Pathname;
    CDiskFileWin32() { Opened = true; }
    CDiskFileWin32(const CDiskFileWin32& o) : Opened(false) { Open(o.Pathname, FILE_ACCESS_READ, 2); }
    void Open(const CWideString& p, FileAccessType a, unsigned long fl) { g_openedAtOpen = Opened; g_openPath = &p; g_openAccess = a; g_openFlags = fl; }
};
int main()
{
    CDiskFileWin32 a; CDiskFileWin32 b(a);
    if (g_base != 2 || g_str != 2 || g_openPath != &a.Pathname || g_openAccess != 1 || g_openFlags != 2 || g_openedAtOpen || b.Opened) { std::printf("0099adc0_TEST FAIL\n"); return 1; }
    std::printf("0099adc0_TEST PASS\n"); return 0;
}