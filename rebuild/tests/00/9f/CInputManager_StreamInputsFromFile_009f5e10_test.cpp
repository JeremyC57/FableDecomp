#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static const void* g_name = 0; static long g_access = 0, g_flags = 0; static const void* g_version = 0; static const void* g_file = 0;
class CWideString { public: void* PStringData; ~CWideString() { std::strcat(g_seq, "w"); } };
class CVersion { public: long a; };
class CBaseClassNonCopyable {
public:   virtual ~CBaseClassNonCopyable() { std::strcat(g_seq, "b"); } };
class CDiskFileWin32 : public CBaseClassNonCopyable { public:
    virtual void Open(const CWideString&, long, unsigned long) { }
    virtual void Close() { std::strcat(g_seq, "c"); }
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened; CWideString Pathname;
    CDiskFileWin32(const CWideString& pathname, long access, unsigned long flags) { std::strcat(g_seq, "O"); g_name = &pathname; g_access = access; g_flags = flags; }
    ~CDiskFileWin32() { Close(); }
};
class CInputManager { public:
    virtual void v00() { }
    unsigned char _pad04[0x34]; bool Loading; bool Saving;
    void LoadAllEventPackages(const CVersion& version, CDiskFileWin32& file) { std::strcat(g_seq, "L"); g_version = &version; g_file = &file; }
    void StreamInputsFromFile(const CVersion& version, const CWideString& fileName) { CDiskFileWin32 file(fileName, 1, 2); LoadAllEventPackages(version, file); Loading = true; }
};
int main()
{
    CInputManager m; m.Loading = false; CVersion v; CWideString n; m.StreamInputsFromFile(v, n);
    if (std::strcmp(g_seq, "OLcwb") || g_name != &n || g_access != 1 || g_flags != 2 || g_version != &v || g_file == 0 || !m.Loading) { std::printf("009f5e10_TEST FAIL\n"); return 1; }
    std::printf("009f5e10_TEST PASS\n");
    return 0;
}