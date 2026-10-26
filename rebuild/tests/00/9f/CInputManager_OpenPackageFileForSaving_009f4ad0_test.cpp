#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
static const void* g_openName = 0; static long g_openAccess = 0, g_openFlags = 0;
static const void* g_writeData = 0; static long g_writeLength = 0; static void* g_writeProcess = (void*)1;
class CWideString { public: void* PStringData; };
class CVersion { public: long a, b, c; };
class CDiskFileWin32 { public:
    virtual void v00() { }
    virtual void Open(const CWideString& pathname, long access, unsigned long flags) { std::strcat(g_seq, "O"); g_openName = &pathname; g_openAccess = access; g_openFlags = flags; }
    virtual void Close() { }
    virtual void v03() { }
    virtual void Write(const void* data, long length, void* process) { std::strcat(g_seq, "W"); g_writeData = data; g_writeLength = length; g_writeProcess = process; }
    unsigned char _body[0x18];
};
class CInputManager { public:
    virtual void v00() { }
    unsigned char _pad04[0x34];
    bool Loading;
    bool Saving;
    unsigned char _pad3a[0x1e];
    long CurrentFilePackageIndex;
    long TimeStamp;
    CDiskFileWin32 PackageFile;
    void Stream(const CVersion& version, const CWideString& fileName)
    {
        PackageFile.Open(fileName, 4, 2);
        PackageFile.Write(&version, 12, 0);
        CurrentFilePackageIndex = 0;
        
    }
};
int main()
{
    CInputManager m; CVersion v; CWideString n; m.CurrentFilePackageIndex = 5; m.Saving = false; m.Loading = false;
    m.Stream(v, n);
    if (std::strcmp(g_seq, "OW") || g_openName != &n || g_openAccess != 4 || g_openFlags != 2 || g_writeData != &v || g_writeLength != 12 || g_writeProcess != 0 || m.CurrentFilePackageIndex != 0 ) { std::printf("009f4ad0_TEST FAIL\n"); return 1; }
    std::printf("009f4ad0_TEST PASS\n");
    return 0;
}