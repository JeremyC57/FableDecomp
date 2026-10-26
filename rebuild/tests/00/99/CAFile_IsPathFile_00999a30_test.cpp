// Behaviour test for 0x00999A30 CAFile::IsPathFile: the path must exist and must not carry FILE_ATTRIBUTE_DIRECTORY.
#include <cstdio>
#include <cstring>
#include <cstdlib>

typedef unsigned short wchar16;
static int g_failed = 0;
#define CHECK(cond) do { if (!(cond)) { std::printf("check failed at line %d\n", __LINE__); g_failed = 1; } } while (0)

// Test-side value strings (plain C++98 stand-ins for CWideString / CCharString).
static unsigned WLen(const wchar16* s) { unsigned n = 0; if (s) while (s[n]) ++n; return n; }
class WStr {
public:
    wchar16* d;
    WStr() : d(0) { }
    WStr(const char* s) : d(0) { SetA(s); }
    WStr(const wchar16* s) : d(0) { SetW(s); }
    WStr(const WStr& o) : d(0) { SetW(o.d); }
    ~WStr() { std::free(d); }
    WStr& operator=(const WStr& o) { if (this != &o) { wchar16* old = d; d = 0; SetW(o.d); std::free(old); } return *this; }
    WStr& operator=(const wchar16* s) { wchar16* old = d; d = 0; SetW(s); std::free(old); return *this; }
    void SetW(const wchar16* s)
    {
        unsigned n = WLen(s);
        if (n == 0) { d = 0; return; }
        d = (wchar16*)std::malloc((n + 1) * sizeof(wchar16));
        for (unsigned i = 0; i < n; ++i) d[i] = s[i];
        d[n] = 0;
    }
    void SetA(const char* s)
    {
        unsigned n = s ? (unsigned)std::strlen(s) : 0;
        if (n == 0) { d = 0; return; }
        d = (wchar16*)std::malloc((n + 1) * sizeof(wchar16));
        for (unsigned i = 0; i < n; ++i) d[i] = (unsigned char)s[i];
        d[n] = 0;
    }
    const wchar16* Text() const { static const wchar16 empty = 0; return d ? d : &empty; }
    unsigned Length() const { return WLen(d); }
    WStr operator+(const WStr& o) const
    {
        unsigned a = Length(), b = o.Length();
        wchar16* t = (wchar16*)std::malloc((a + b + 1) * sizeof(wchar16));
        for (unsigned i = 0; i < a; ++i) t[i] = d[i];
        for (unsigned j = 0; j < b; ++j) t[a + j] = o.d[j];
        t[a + b] = 0;
        WStr r(t);
        std::free(t);
        return r;
    }
    bool Eq(const char* s) const
    {
        unsigned n = (unsigned)std::strlen(s);
        if (Length() != n) return false;
        for (unsigned i = 0; i < n; ++i) if (d[i] != (unsigned char)s[i]) return false;
        return true;
    }
};
class CStr {
public:
    char* d;
    CStr() : d(0) { }
    CStr(const char* s) : d(0) { Set(s); }
    CStr(const CStr& o) : d(0) { Set(o.d); }
    ~CStr() { std::free(d); }
    CStr& operator=(const CStr& o) { if (this != &o) { char* old = d; d = 0; Set(o.d); std::free(old); } return *this; }
    void Set(const char* s)
    {
        unsigned n = s ? (unsigned)std::strlen(s) : 0;
        if (n == 0) { d = 0; return; }
        d = (char*)std::malloc(n + 1);
        std::memcpy(d, s, n + 1);
    }
    const char* Text() const { return d ? d : ""; }
    bool Eq(const char* s) const { return std::strcmp(Text(), s) == 0; }
};
static WStr Widen(const CStr& s) { return WStr(s.Text()); }
static CStr Narrow(const WStr& s)
{
    unsigned n = s.Length();
    char* t = (char*)std::malloc(n + 1);
    for (unsigned i = 0; i < n; ++i) t[i] = (char)s.d[i];
    t[n] = 0;
    CStr r(t);
    std::free(t);
    return r;
}

// Path stand-ins shared by the CAFile tests: the current path is C:\Game; ExpandPath joins a relative path onto it.
static WStr GetCurrentPath() { return WStr("C:\\Game"); }
static WStr ExpandPath(const WStr& path, const WStr& base)
{
    const wchar16* p = path.Text();
    if (WLen(p) >= 2 && p[1] == ':')
        return path;
    return base + WStr("\\") + path;
}

// File-attribute stand-ins: a tiny table keyed by the full path.
struct SFakeFile { const char* path; unsigned long attributes; unsigned long writeLow; unsigned long writeHigh; };
static SFakeFile g_files[] = {
    { "C:\\Game\\readme.txt", 0x01, 0x89ABCDEFUL, 0x01D23456UL },
    { "C:\\Game\\save.dat",   0x20, 0x00000001UL, 0x00000000UL },
    { "C:\\Game\\data",       0x10, 0, 0 },
};
static SFakeFile* FindFake(const WStr& fullPath)
{
    for (unsigned i = 0; i < sizeof(g_files) / sizeof(g_files[0]); ++i)
        if (fullPath.Eq(g_files[i].path))
            return &g_files[i];
    return 0;
}
static unsigned long GetFileAttributesW(const WStr& fullPath)
{
    SFakeFile* f = FindFake(fullPath);
    return f ? f->attributes : 0xFFFFFFFFUL;
}
static int SetFileAttributesW(const WStr& fullPath, unsigned long attributes)
{
    SFakeFile* f = FindFake(fullPath);
    if (!f) return 0;
    f->attributes = attributes;
    return 1;
}
struct FILETIME { unsigned long dwLowDateTime; unsigned long dwHighDateTime; };
struct WIN32_FILE_ATTRIBUTE_DATA { unsigned long dwFileAttributes; FILETIME ftCreationTime; FILETIME ftLastAccessTime;
                                   FILETIME ftLastWriteTime; unsigned long nFileSizeHigh; unsigned long nFileSizeLow; };
static int GetFileAttributesExW(const WStr& fullPath, int, WIN32_FILE_ATTRIBUTE_DATA* data)
{
    SFakeFile* f = FindFake(fullPath);
    std::memset(data, 0, sizeof(*data));
    if (!f) return 0;
    data->dwFileAttributes = f->attributes;
    data->ftLastWriteTime.dwLowDateTime = f->writeLow;
    data->ftLastWriteTime.dwHighDateTime = f->writeHigh;
    return 1;
}
class CAFile { public:
    static bool PathExists(const WStr& path)          // stand-in for 0x00999230
    {
        return GetFileAttributesW(ExpandPath(path, GetCurrentPath())) != 0xFFFFFFFFUL;
    }
    static bool IsPathFile(const WStr& path)
    {
        WStr fullPath;
        fullPath = ExpandPath(path, GetCurrentPath());
        if (PathExists(path) && !(GetFileAttributesW(fullPath) & 0x10))
            return true;
        return false;
    }
};
int main()
{
    CHECK(CAFile::IsPathFile(WStr("readme.txt")));
    CHECK(CAFile::IsPathFile(WStr("save.dat")));
    CHECK(!CAFile::IsPathFile(WStr("data")));
    CHECK(!CAFile::IsPathFile(WStr("missing.bin")));
    if (g_failed) { std::printf("00999a30_TEST FAIL\n"); return 1; }
    std::printf("00999a30_TEST PASS\n");
    return 0;
}