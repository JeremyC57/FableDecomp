// Behaviour test for 0x00998E70 CAFile::CopyFileW(CAFile&, CAFile&): the destination is sized to the source, both are
// rewound, and the data moves in 4 KB blocks with one progress pump per block.
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

static int g_progressCalls = 0;
static void DisplayProgress(const CStr&, float, bool, bool) { ++g_progressCalls; }
class CAFile {
public:
    virtual ~CAFile() { }
    virtual void Read(void* destination, long size) = 0;
    virtual void Write(const void* source, long size) = 0;
    virtual void SetPosition(long position) = 0;
    virtual void SetLength(long length) = 0;
    virtual long GetLength() const = 0;
    static void CopyFileW(CAFile& source, CAFile& destination)
    {
        destination.SetLength(source.GetLength());
        destination.SetPosition(0);
        source.SetPosition(0);
        long bufferSize = 0x1000;
        unsigned char* buffer = (unsigned char*)std::calloc(bufferSize, 1);
        long remaining = source.GetLength();
        while (remaining > 0) {
            long chunk = remaining < bufferSize ? remaining : bufferSize;
            source.Read(buffer, chunk);
            destination.Write(buffer, chunk);
            remaining -= chunk;
            DisplayProgress(CStr(""), -1.0f, false, false);
        }
        std::free(buffer);
    }
};
class CMemoryFileTest : public CAFile {
public:
    unsigned char Data[20000];
    long Length, Position;
    CMemoryFileTest() : Length(0), Position(0) { std::memset(Data, 0, sizeof(Data)); }
    void Read(void* destination, long size) { std::memcpy(destination, Data + Position, size); Position += size; }
    void Write(const void* source, long size) { std::memcpy(Data + Position, source, size); Position += size; }
    void SetPosition(long position) { Position = position; }
    void SetLength(long length) { Length = length; }
    long GetLength() const { return Length; }
};
int main()
{
    static CMemoryFileTest source, destination;
    source.Length = 10000;
    for (long i = 0; i < 10000; ++i) source.Data[i] = (unsigned char)(i * 7 + 3);
    source.Position = 123;
    destination.Position = 55;
    CAFile::CopyFileW(source, destination);
    CHECK(destination.Length == 10000 && destination.Position == 10000);
    CHECK(std::memcmp(source.Data, destination.Data, 10000) == 0);
    CHECK(g_progressCalls == 3);
    if (g_failed) { std::printf("00998e70_TEST FAIL\n"); return 1; }
    std::printf("00998e70_TEST PASS\n");
    return 0;
}