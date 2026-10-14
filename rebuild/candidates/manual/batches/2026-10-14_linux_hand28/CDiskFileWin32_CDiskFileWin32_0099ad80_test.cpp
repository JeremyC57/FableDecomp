#include <cstdio>
static int g_base = 0, g_str = 0; static const void* g_openPath = 0; static long g_openAccess = -1; static unsigned long g_openFlags = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CWideString { void* PStringData; CWideString() : PStringData(0) { ++g_str; } };
enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CDiskFileWin32 : CBase {
    virtual void v00() { }
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened; CWideString Pathname;
    CDiskFileWin32(const CWideString& p, FileAccessType a, unsigned long fl) { Open(p, a, fl); }
    void Open(const CWideString& p, FileAccessType a, unsigned long fl) { g_openPath = &p; g_openAccess = a; g_openFlags = fl; Opened = true; }
};
int main()
{
    CWideString path; CDiskFileWin32 f(path, FILE_ACCESS_READ, 5);
    if (g_base != 1 || g_str != 2 || g_openPath != &path || g_openAccess != 1 || g_openFlags != 5 || !f.Opened || sizeof(f) != 0x1c) { std::printf("0099ad80_TEST FAIL\n"); return 1; }
    std::printf("0099ad80_TEST PASS\n"); return 0;
}
