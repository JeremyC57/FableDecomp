// Behaviour test for 00497a60: the model below is a copy of src/00497a60.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
#ifdef _MSC_VER
typedef unsigned short wchar_t;
#else
#define wchar_t unsigned short
#endif

class C2DVector { public: float X, Y; };
class C2DBoxF { public:
    C2DVector TopLeft;                                  // +0x00
    C2DVector BottomRight;                              // +0x08
    float GetWidth() const { float left = TopLeft.X; float right = BottomRight.X; return right - left; }
};
class CFontBank { public: };

class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    operator const wchar_t*() const;                    // 0x0099AF10
    void SetAsSubString(const wchar_t* text, long length);   // 0x0099B770
    const CWideString& operator=(const wchar_t* text);  // 0x0099B800
    const CWideString& operator+=(const CWideString& other); // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30

const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* text, unsigned int width, const wchar_t** next);   // 0x00A10390

static const wchar_t s_newLine[] = { '\n', 0 };          // 0x01231728

struct CProgressDisplay {
    CWideString WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const;
};

CWideString CProgressDisplay::WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const
{
    (void)font;
    CWideString wrapped;
    const wchar_t* line = text;
    const wchar_t* end;
    do {
        const wchar_t* next = 0;
        end = WordWrap_FindNextLineW(line, (unsigned int)box.GetWidth(), &next);
        CWideString piece;
        if (end)
            piece.SetAsSubString(line, end - line);
        else
            piece = line;
        wrapped += piece + s_newLine;
        line = end;
    } while (end);
    return wrapped;
}

// ---- test doubles and driver ----
// CWideString double: PStringData -> a heap block {long len; wchar_t text[...]}, freed never (test only)
struct SBlock { long len; wchar_t text[256]; };
static SBlock* NewBlock(const wchar_t* t, long n) { SBlock* b = new SBlock; b->len = n; for (long i = 0; i < n; ++i) b->text[i] = t[i]; b->text[n] = 0; return b; }
static long Len(const wchar_t* t) { long n = 0; while (t[n]) ++n; return n; }
static int g_live = 0;
CWideString::CWideString() : PStringData(NewBlock((const wchar_t*)L"\0\0", 0)) { ++g_live; }
CWideString::CWideString(const CWideString& o) { SBlock* b = (SBlock*)o.PStringData; PStringData = NewBlock(b->text, b->len); ++g_live; }
CWideString::~CWideString() { --g_live; }
CWideString::operator const wchar_t*() const { return ((SBlock*)PStringData)->text; }
void CWideString::SetAsSubString(const wchar_t* t, long n) { PStringData = NewBlock(t, n); }
const CWideString& CWideString::operator=(const wchar_t* t) { PStringData = NewBlock(t, Len(t)); return *this; }
const CWideString& CWideString::operator+=(const CWideString& o)
{
    SBlock* a = (SBlock*)PStringData; SBlock* b = (SBlock*)o.PStringData;
    wchar_t tmp[256]; long n = 0;
    for (long i = 0; i < a->len; ++i) tmp[n++] = a->text[i];
    for (long i = 0; i < b->len; ++i) tmp[n++] = b->text[i];
    PStringData = NewBlock(tmp, n); return *this;
}
CWideString __fastcall operator+(const CWideString& l, const wchar_t* r) { CWideString s(l); CWideString t; t = r; s += t; return s; }
static unsigned int g_width = 0;
const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* t, unsigned int width, const wchar_t** next)
{
    g_width = width;
    if ((unsigned long)Len(t) <= width) { *next = 0; return 0; }
    *next = t + width; return t + width;
}
static bool Same(const CWideString& s, const char* want)
{
    const wchar_t* t = s; long n = Len(t);
    if (n != (long)std::strlen(want)) return false;
    for (long i = 0; i < n; ++i) if (t[i] != (wchar_t)want[i]) return false;
    return true;
}
static void SetText(CWideString& s, const char* a) { wchar_t buf[256]; long n = 0; while (a[n]) { buf[n] = (wchar_t)a[n]; ++n; } buf[n] = 0; s = buf; }
int main()
{
    CProgressDisplay display;
    C2DBoxF box; box.TopLeft.X = 10.0f; box.TopLeft.Y = 0.0f; box.BottomRight.X = 14.9f; box.BottomRight.Y = 5.0f;   // width 4.9 -> 4
    CFontBank font;
    CWideString text; SetText(text, "abcdefghij");
    bool ok = true;
    {
        CWideString r = display.WordWrapText(box, font, text);
        if (!Same(r, "abcd\nefgh\nij\n") || g_width != 4) ok = false;
    }
    SetText(text, "ab");
    {
        CWideString r = display.WordWrapText(box, font, text);
        if (!Same(r, "ab\n")) ok = false;
    }
    if (g_live != 1) ok = false;    // only 'text' remains alive
    if (!ok) { std::printf("00497a60_TEST FAIL\n"); return 1; }
    std::printf("00497a60_TEST PASS\n");
    return 0;
}