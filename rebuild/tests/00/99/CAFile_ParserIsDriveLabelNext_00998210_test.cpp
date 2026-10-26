// Behaviour test for 0x00998210 CAFile::ParserIsDriveLabelNext: peeks for "<name>:" and always restores the parser
// position.
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

// CStringParser stand-in: a scripted list of (type, text) items with a read position that can be saved and restored.
class CParsedItem { public:
    long Type;                       // 1 = symbol, 2 = name (EParsedItemType)
    long Value;
    CStr String;
    CParsedItem() : Type(0), Value(0) { }
    const CStr& GetString() const { return String; }
};
struct SScriptItem { long type; const char* text; };
class CStringParser { public:
    const SScriptItem* Items;
    unsigned long Count;
    unsigned long StringPos;
    CStringParser(const SScriptItem* items, unsigned long count) : Items(items), Count(count), StringPos(0) { }
    bool ReadNextItem(CParsedItem& item)
    {
        if (StringPos >= Count) return false;
        item.Type = Items[StringPos].type;
        item.String = CStr(Items[StringPos].text);
        ++StringPos;
        return true;
    }
    unsigned long GetStringPos() const { return StringPos; }
    void SetStringPos(unsigned long pos) { StringPos = pos; }
};
class CAFile { public:
    static bool ParserReadPastDriveLabel(CStringParser& parser, CParsedItem& item)   // stand-in 0x00998170
    {
        return parser.ReadNextItem(item) && item.Type == 2 && parser.ReadNextItem(item) && item.Type == 1 &&
               item.GetString().Eq(":");
    }
    static bool ParserIsDriveLabelNext(CStringParser& parser)
    {
        unsigned long pos = parser.GetStringPos();
        CParsedItem item;
        bool result = ParserReadPastDriveLabel(parser, item);
        parser.SetStringPos(pos);
        return result;
    }
};
int main()
{
    static const SScriptItem items[] = { { 2, "dir" }, { 1, "\\" }, { 2, "D" }, { 1, ":" } };
    CStringParser p(items, 4);
    CHECK(!CAFile::ParserIsDriveLabelNext(p) && p.StringPos == 0);
    p.StringPos = 2;
    CHECK(CAFile::ParserIsDriveLabelNext(p) && p.StringPos == 2);
    if (g_failed) { std::printf("00998210_TEST FAIL\n"); return 1; }
    std::printf("00998210_TEST PASS\n");
    return 0;
}