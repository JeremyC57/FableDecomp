#include <cstdio>
static int g_base = 0; static const void* g_openPath = 0; static long g_openAccess = -1; static unsigned long g_openFlags = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
class CWideString { public: void* PStringData; CWideString() : PStringData(0) { } };
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CAFile;
class CDiskFileWin32 : public CBase { public:
    virtual CAFile* Clone() const { return (CAFile*)new CDiskFileWin32(*this); }
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened; CWideString Pathname;
    CDiskFileWin32() { Opened = true; }
    CDiskFileWin32(const CDiskFileWin32& o) : Opened(false) { Open(o.Pathname, FILE_ACCESS_READ, 2); }
    void Open(const CWideString& p, FileAccessType a, unsigned long fl) { g_openPath = &p; g_openAccess = a; g_openFlags = fl; }
};
int main()
{
    CDiskFileWin32 a; CDiskFileWin32* c = (CDiskFileWin32*)a.Clone();
    if (c == 0 || c == &a || g_base != 2 || g_openPath != &a.Pathname || g_openAccess != 1 || g_openFlags != 2 || c->Opened || sizeof(*c) != 0x1c) { std::printf("0099ae00_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("0099ae00_TEST PASS\n"); return 0;
}