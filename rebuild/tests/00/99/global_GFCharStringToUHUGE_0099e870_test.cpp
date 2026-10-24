// Behaviour test for 0x0099E870 GFCharStringToUHUGE: unsigned 64-bit decimal; '-' is ignored, '.' stops, other
// non-digits are skipped.
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

// CCharString accessors as the GF parsers use them: a null block reads as "" with length 0.
#include <cctype>
static long GetLength(const CStr& s) { return s.d ? (long)std::strlen(s.d) : 0; }
static char GetChar(const CStr& s, unsigned long i) { return s.Text()[i]; }
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static u64 GFCharStringToUHUGE(const CStr& text)
{
    u64 value = 0;
    unsigned long i = 0;
    char c = GetChar(text, i);
    while (i < (unsigned long)GetLength(text)) {
        if (c == '-')
            ;
        else if (c == '.')
            break;
        else if (std::isdigit((unsigned char)c))
            value = value * 10 + (c - '0');
        c = GetChar(text, ++i);
    }
    return value;
}
int main()
{
    u64 big = GFCharStringToUHUGE(CStr("18446744073709551615"));
    CHECK((unsigned long)(big >> 32) == 0xFFFFFFFFUL && (unsigned long)(big & 0xFFFFFFFFUL) == 0xFFFFFFFFUL);
    u64 mid = GFCharStringToUHUGE(CStr("4294967296"));
    CHECK((unsigned long)(mid >> 32) == 1 && (unsigned long)(mid & 0xFFFFFFFFUL) == 0);
    CHECK(GFCharStringToUHUGE(CStr("-5")) == 5);
    CHECK(GFCharStringToUHUGE(CStr("7.9")) == 7);
    CHECK(GFCharStringToUHUGE(CStr()) == 0);
    if (g_failed) { std::printf("0099e870_TEST FAIL\n"); return 1; }
    std::printf("0099e870_TEST PASS\n");
    return 0;
}