// Behaviour test for 0x0099D7D0 ConvertStaticToHyperlink: parent subclassed once, SS_NOTIFY added, control subclassed,
// "Old_Proc"/"Old_Font"/"Font"/"Static" props set, and the underlined font selected.
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

struct HWND__ { long style; long proc; HWND__* parent; long font; const char* propNames[8]; long propValues[8]; int props; };
typedef HWND__* HWND;
typedef long (*WNDPROC)(HWND, unsigned int, unsigned int, long);
struct LOGFONTA { long lfHeight; unsigned char lfItalic, lfUnderline; };
static long ParentProc(HWND, unsigned int, unsigned int, long) { return 0; }
static long LinkProc(HWND, unsigned int, unsigned int, long) { return 0; }
static long OriginalProc(HWND, unsigned int, unsigned int, long) { return 1; }
static LOGFONTA g_fonts[8];
static int g_fontCount = 1;                              // font handle 1 = the original font
static HWND GetParent(HWND w) { return w->parent; }
static long GetWindowLongA(HWND w, int index) { return index == -4 ? w->proc : w->style; }
static long SetWindowLongA(HWND w, int index, long value) { long old = GetWindowLongA(w, index); if (index == -4) w->proc = value; else w->style = value; return old; }
static int SetPropA(HWND w, const char* name, long data) { w->propNames[w->props] = name; w->propValues[w->props++] = data; return 1; }
static long GetPropValue(HWND w, const char* name) { for (int i = w->props - 1; i >= 0; --i) if (std::strcmp(w->propNames[i], name) == 0) return w->propValues[i]; return -1; }
static long SendMessageA(HWND w, unsigned int message, unsigned int wParam, long) { if (message == 0x31) return w->font; if (message == 0x30) w->font = (long)wParam; return 0; }
static int GetObjectA(long font, int size, void* out) { std::memcpy(out, &g_fonts[font], size); return size; }
static long CreateFontIndirectA(const LOGFONTA* font) { g_fonts[++g_fontCount] = *font; return g_fontCount; }
static int ConvertStaticToHyperlink(HWND control)
{
    HWND parent = GetParent(control);
    if (parent != 0) {
        WNDPROC parentProc = (WNDPROC)GetWindowLongA(parent, -4);
        if (parentProc != ParentProc) {
            SetPropA(parent, "Old_Proc", (long)parentProc);
            SetWindowLongA(parent, -4, (long)ParentProc);
        }
    }
    unsigned long style = GetWindowLongA(control, -16);
    SetWindowLongA(control, -16, style | 0x100);
    WNDPROC controlProc = (WNDPROC)GetWindowLongA(control, -4);
    SetPropA(control, "Old_Proc", (long)controlProc);
    SetWindowLongA(control, -4, (long)LinkProc);
    long originalFont = SendMessageA(control, 0x31, 0, 0);
    SetPropA(control, "Old_Font", originalFont);
    LOGFONTA logFont;
    GetObjectA(originalFont, sizeof(logFont), &logFont);
    logFont.lfUnderline = 1;
    long underlineFont = CreateFontIndirectA(&logFont);
    SetPropA(control, "Font", underlineFont);
    SendMessageA(control, 0x30, (unsigned int)underlineFont, 0);
    SetPropA(control, "Static", 1);
    return 1;
}
int main()
{
    static HWND__ dialog, link1, link2;
    dialog.proc = (long)OriginalProc;
    link1.parent = link2.parent = &dialog;
    link1.proc = link2.proc = (long)OriginalProc;
    link1.style = link2.style = 0x50000000;
    link1.font = link2.font = 1;
    g_fonts[1].lfHeight = -12;
    CHECK(ConvertStaticToHyperlink(&link1) == 1);
    CHECK(ConvertStaticToHyperlink(&link2) == 1);
    CHECK(dialog.proc == (long)ParentProc && dialog.props == 1 && GetPropValue(&dialog, "Old_Proc") == (long)OriginalProc);
    CHECK(link1.style == 0x50000100 && link1.proc == (long)LinkProc && GetPropValue(&link1, "Old_Proc") == (long)OriginalProc);
    CHECK(GetPropValue(&link1, "Old_Font") == 1 && GetPropValue(&link1, "Font") == link1.font && link1.font == 2);
    CHECK(g_fonts[2].lfUnderline == 1 && g_fonts[2].lfHeight == -12 && GetPropValue(&link1, "Static") == 1);
    CHECK(link2.font == 3 && link2.props == 4);
    if (g_failed) { std::printf("0099d7d0_TEST FAIL\n"); return 1; }
    std::printf("0099d7d0_TEST PASS\n");
    return 0;
}