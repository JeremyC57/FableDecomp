// Behaviour test for 0x00999970 CAFile::ValidatePathnameIsOnWriteableDrive: nothing happens unless read-only drive
// emulation is on; then the narrow full path is split and its drive kept (the check itself is compiled out).
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
static bool DebugUseReadOnlyDrives = false;
static int g_splits = 0;
static CStr g_lastDrive;
struct CAFile {
    static WStr ConvertPathToFullPath(const WStr& path) { return ExpandPath(path, GetCurrentPath()); }
    static void ValidatePathnameIsOnWriteableDrive(const WStr& path)
    {
        if (DebugUseReadOnlyDrives) {
            CStr drive;
            CStr fullPath = Narrow(ConvertPathToFullPath(path));
            {
                char driveText[3];
                char dir[256];
                char name[256];
                char ext[256];
                SplitPathA(fullPath.Text(), driveText, dir, name, ext);
                ++g_splits;
                drive = CStr(driveText);
            }
            g_lastDrive = drive;                      // observation hook for the test only
        }
    }
};
int main()
{
    CAFile::ValidatePathnameIsOnWriteableDrive(WStr("a.txt"));
    CHECK(g_splits == 0);
    DebugUseReadOnlyDrives = true;
    CAFile::ValidatePathnameIsOnWriteableDrive(WStr("a.txt"));
    CHECK(g_splits == 1 && g_lastDrive.Eq("C:"));
    CAFile::ValidatePathnameIsOnWriteableDrive(WStr("Q:\\b.txt"));
    CHECK(g_splits == 2 && g_lastDrive.Eq("Q:"));
    if (g_failed) { std::printf("00999970_TEST FAIL\n"); return 1; }
    std::printf("00999970_TEST PASS\n");
    return 0;
}