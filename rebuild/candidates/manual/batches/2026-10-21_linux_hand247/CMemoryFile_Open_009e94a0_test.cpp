// Behaviour test for CMemoryFile::Open @ 0x009E94A0: existing file read whole, missing read-only file throws type 10, missing write file opens empty.
#include <cstdio>
#include <cstring>

struct CWideString { const char* path; };
static const char* g_existing = "a.bin";
static unsigned char g_disk[5] = { 1, 2, 3, 4, 5 };
static CWideString ConvertPathToFullPath(const CWideString& p) { return p; }
static bool PathExists(const CWideString& p) { return std::strcmp(p.path, g_existing) == 0; }

struct CBBBFileException { int Type; };

struct CDiskFileWin32 {
    long Length;
    CDiskFileWin32(const CWideString&, int, unsigned long) : Length(5) {}
    void Read(void* buffer, long size, void*) { std::memcpy(buffer, g_disk, size); }
};

struct CBuffer {
    unsigned char data[16]; unsigned long count;
    unsigned char* begin() { return data; }
    unsigned long size() const { return count; }
    void resize(unsigned long n, const unsigned char& v) { for (unsigned long i = count; i < n; ++i) data[i] = v; count = n; }
};

struct CMemoryFile {
    long AccessType, Position, Length; CBuffer DataBuffer; bool Opened; CWideString Pathname;
    void Open(const CWideString& pathname, int access, unsigned long flags);
};

void CMemoryFile::Open(const CWideString& pathname, int access, unsigned long flags)
{
    CWideString fullPath = ConvertPathToFullPath(pathname);
    if (PathExists(fullPath)) {
        CDiskFileWin32 file(pathname, access, flags);
        Length = file.Length;
        DataBuffer.resize(file.Length, 0);
        if (Length > 0)
            file.Read(DataBuffer.begin(), file.Length, 0);
        AccessType = access;
    } else {
        if (access == 1) {
            CBBBFileException e; e.Type = 10;
            throw e;
        }
        AccessType = access;
    }
    Pathname = pathname;
    Position = 0;
    Length = DataBuffer.size();
    Opened = true;
}

int main()
{
    CMemoryFile f; f.AccessType = 0; f.Position = 9; f.Length = 0; f.DataBuffer.count = 0; f.Opened = false;
    CWideString name; name.path = "a.bin";
    f.Open(name, 1, 2);
    int ok = f.Opened && f.Length == 5 && f.Position == 0 && f.AccessType == 1 && f.DataBuffer.data[4] == 5 && f.Pathname.path == name.path;
    CMemoryFile g; g.AccessType = 0; g.Position = 3; g.Length = 7; g.DataBuffer.count = 0; g.Opened = false;
    CWideString missing; missing.path = "b.bin";
    int thrown = 0;
    try { g.Open(missing, 1, 2); } catch (CBBBFileException& e) { thrown = e.Type; }
    ok = ok && thrown == 10 && !g.Opened;
    g.Open(missing, 4, 2);
    ok = ok && g.Opened && g.Length == 0 && g.Position == 0 && g.AccessType == 4;
    if (!ok) { std::printf("009e94a0_TEST FAIL\n"); return 1; }
    std::printf("009e94a0_TEST PASS\n");
    return 0;
}
