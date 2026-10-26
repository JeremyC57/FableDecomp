#include <cstdio>
#include <string.h>
#include <new>
static int g_base = 0, g_str = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
class CWideString { public: void* PStringData; CWideString() : PStringData(0) { ++g_str; } };
class CDiskFileWin32 : public CBase { public:
    virtual void v00() { }
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened; CWideString Pathname;
    CDiskFileWin32() { Opened = false; }
};
int main()
{
    unsigned char raw[sizeof(CDiskFileWin32)]; memset(raw, 0xee, sizeof(raw));
    CDiskFileWin32* f = new (raw) CDiskFileWin32;
    if (g_base != 1 || g_str != 1 || f->Opened || f->Pathname.PStringData != 0 || sizeof(*f) != 0x1c || (char*)&f->Opened - (char*)f != 0x14 || (char*)&f->Pathname - (char*)f != 0x18) { std::printf("00415c30_TEST FAIL\n"); return 1; }
    std::printf("00415c30_TEST PASS\n"); return 0;
}