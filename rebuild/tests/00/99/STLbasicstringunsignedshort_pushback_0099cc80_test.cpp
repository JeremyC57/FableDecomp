// Behaviour test for 0x0099CC80 basic_string<unsigned short>::push_back(c) = append(1, c): capacity doubles from a full
// string and the terminator follows the new character.
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

// STLport basic_string<unsigned short> model (12 bytes: start, finish, end of storage).
struct WideStringRep {
    wchar16* _M_start;
    wchar16* _M_finish;
    wchar16* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start) - 1; }
    void _M_allocate_block(unsigned int n)
    {
        _M_start = (wchar16*)std::malloc(n * sizeof(wchar16));
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
    void reserve(unsigned int res_arg)
    {
        unsigned int sz = size();
        unsigned int n = (res_arg < sz ? sz : res_arg) + 1;
        wchar16* p = (wchar16*)std::malloc(n * sizeof(wchar16));
        for (unsigned int i = 0; i < sz; ++i) p[i] = _M_start[i];
        p[sz] = 0;
        std::free(_M_start);
        _M_start = p; _M_finish = p + sz; _M_end_of_storage = p + n;
    }
    WideStringRep& append(unsigned int n, wchar16 c)
    {
        if (size() + n > capacity())
            reserve(size() + (size() < n ? n : size()));
        if (n > 0) {
            for (unsigned int i = 1; i < n; ++i) _M_finish[i] = c;
            _M_finish[n] = 0;
            *_M_finish = c;
            _M_finish += n;
        }
        return *this;
    }
};
int main()
{
    WideStringRep s;
    s._M_allocate_block(1);
    s._M_start[0] = 0;
    const char* word = "push";
    for (unsigned i = 0; word[i]; ++i) s.append(1, (unsigned char)word[i]);
    CHECK(WStr(s._M_start).Eq("push") && s.size() == 4 && s.capacity() >= 4 && *s._M_finish == 0);
    std::free(s._M_start);
    if (g_failed) { std::printf("0099cc80_TEST FAIL\n"); return 1; }
    std::printf("0099cc80_TEST PASS\n");
    return 0;
}