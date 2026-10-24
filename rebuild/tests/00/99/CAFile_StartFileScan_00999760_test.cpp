// Behaviour test for 0x00999760 CAFile::StartFileScan(..., wchar_t*): a successful _wfindfirst stores the handle, clears
// Stopped and writes drive + directory of the expanded pattern + the found name; a failure sets Stopped.
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

// Directory-scan stand-ins: a fixed listing returned one name at a time.
struct CFileScanInfo { bool Stopped; long SearchHandle; };
static const char* g_listing[] = { "a.txt", "b.txt" };
static void CopyAsciiW(wchar16* out, const char* s) { unsigned i = 0; for (; s[i]; ++i) out[i] = (unsigned char)s[i]; out[i] = 0; }
// _wsplitpath / wcscpy / wcscat stand-ins over 16-bit characters.
static void SplitPathW(const wchar16* path, wchar16* drive, wchar16* dir, wchar16* name, wchar16* ext)
{
    unsigned n = WLen(path), start = 0;
    if (drive) { drive[0] = 0; }
    if (n >= 2 && path[1] == ':') { if (drive) { drive[0] = path[0]; drive[1] = ':'; drive[2] = 0; } start = 2; }
    unsigned lastSlash = start, i;
    for (i = start; i < n; ++i) if (path[i] == '\\' || path[i] == '/') lastSlash = i + 1;
    if (dir) { for (i = start; i < lastSlash; ++i) dir[i - start] = path[i]; dir[lastSlash - start] = 0; }
    unsigned dot = n;
    for (i = lastSlash; i < n; ++i) if (path[i] == '.') dot = i;
    if (name) { for (i = lastSlash; i < dot; ++i) name[i - lastSlash] = path[i]; name[dot - lastSlash] = 0; }
    if (ext) { for (i = dot; i < n; ++i) ext[i - dot] = path[i]; ext[n - dot] = 0; }
}
static void SplitPathA(const char* path, char* drive, char* dir, char* name, char* ext)
{
    unsigned n = (unsigned)std::strlen(path), start = 0, i;
    if (drive) drive[0] = 0;
    if (n >= 2 && path[1] == ':') { if (drive) { drive[0] = path[0]; drive[1] = ':'; drive[2] = 0; } start = 2; }
    unsigned lastSlash = start;
    for (i = start; i < n; ++i) if (path[i] == '\\' || path[i] == '/') lastSlash = i + 1;
    if (dir) { for (i = start; i < lastSlash; ++i) dir[i - start] = path[i]; dir[lastSlash - start] = 0; }
    unsigned dot = n;
    for (i = lastSlash; i < n; ++i) if (path[i] == '.') dot = i;
    if (name) { for (i = lastSlash; i < dot; ++i) name[i - lastSlash] = path[i]; name[dot - lastSlash] = 0; }
    if (ext) { for (i = dot; i < n; ++i) ext[i - dot] = path[i]; ext[n - dot] = 0; }
}
static void CopyW(wchar16* d, const wchar16* s) { unsigned i = 0; for (; s[i]; ++i) d[i] = s[i]; d[i] = 0; }
static void CatW(wchar16* d, const wchar16* s) { CopyW(d + WLen(d), s); }
struct WFindData { unsigned attrib; wchar16 name[260]; };
static long FindFirstW(const wchar16* spec, WFindData* data)   // _wfindfirst stand-in: only *.txt in C:\Game\docs matches
{
    WStr s(spec);
    if (!s.Eq("C:\\Game\\docs\\*.txt"))
        return -1;
    CopyAsciiW(data->name, "notes.txt");
    return 42;
}
struct CAFile {
    static bool StartFileScan(const WStr& pattern, CFileScanInfo& info, wchar16* firstFile)
    {
        wchar16 drive[3];
        wchar16 dir[256];
        WFindData data;
        WStr path = ExpandPath(pattern, GetCurrentPath());
        info.SearchHandle = FindFirstW(path.Text(), &data);
        if (info.SearchHandle != -1) {
            SplitPathW(path.Text(), drive, dir, 0, 0);
            CopyW(firstFile, drive);
            CatW(firstFile, dir);
            CatW(firstFile, data.name);
            info.Stopped = false;
            return true;
        }
        info.Stopped = true;
        return false;
    }
};
int main()
{
    CFileScanInfo info;
    wchar16 out[262];
    info.Stopped = true;
    CHECK(CAFile::StartFileScan(WStr("docs\\*.txt"), info, out));
    CHECK(WStr(out).Eq("C:\\Game\\docs\\notes.txt") && info.SearchHandle == 42 && !info.Stopped);
    CHECK(!CAFile::StartFileScan(WStr("docs\\*.bin"), info, out));
    CHECK(info.SearchHandle == -1 && info.Stopped);
    if (g_failed) { std::printf("00999760_TEST FAIL\n"); return 1; }
    std::printf("00999760_TEST PASS\n");
    return 0;
}