// Behaviour test for 0x0054F8E0 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __fastcall
#endif
// NUISystem::CText::FitInWindow(CWideString& text, const C2DBoxF& window) -- word-wrap the text to the window width line by line, rebuilding it with explicit newlines, and return the wrapped height in font lines (one font height when the window is empty)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054F8E0, length 560 bytes, `ret 8`.
//   FableWin.pdb: ?FitInWindow@CText@NUISystem@@IAEJAAVCWideString@@ABVC2DBoxF@@@Z.
// Callees: CWideString ctor() 0x0099AED0 (catalog "CCharString" is a byte twin), GetAsCharArray()
//          0x0099AF10, Contains 0x0099AF20, FindCharPos 0x0099B070, GetLength 0x0099B220, ~CWideString 0x0099B510,
//          ctor(const unsigned short*) 0x0099B6B0, SetAsSubString(text, length) 0x0099B770 (PDB ?SetAsSubString@CWideString@@QAEXPB_WJ@Z; catalog "CCharString_Replace"),
//          operator=(const CWideString&) 0x0099B7D0, operator=(const unsigned short*) 0x0099B800, operator+= 0x0099B8D0,
//          operator+(const CWideString&, const unsigned short*) 0x0099BF30 (__fastcall, hidden result first);
//          the word-wrap helpers SetOption 0x00A0FDB0, SetCallback 0x00A0FD90, FindNextLineW 0x00A10390 (__fastcall),
//          __ftol2 0x00BFEA70; the width callback NUISystem::CText measure function 0x0054F4B0.
// Strings: kNL 0x01231728; 0.0f 0x0122DEDC.
// Layout: retail NUISystem::CText = Ego_r.pdb minus 0x30 (landed ctor 0x0054F5C0): PFont (CCountedPointer<CFontBank>)
//         +0x160; the font bank keeps its line height at +4.
// Shape: the per-line width is `(long)(window.BRX - left)` with `float left = window.TLX;` read first in the loop body:
//        retail loads TLX, keeps it on the x87 stack through __ftol2 and drops it with `fstp st(0)`; the same named
//        local also restores the inner newline loop's head alignment. Exits are written out in full.
class CWideString { public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const unsigned short* text);                // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
    const unsigned short* GetAsCharArray() const;                 // 0x0099AF10
    bool Contains(const CWideString& text) const;           // 0x0099AF20
    long FindCharPos(unsigned short c) const;               // 0x0099B070
    long GetLength() const;                                 // 0x0099B220
    void SetAsSubString(const unsigned short* text, long length);   // 0x0099B770
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator=(const unsigned short* text);     // 0x0099B800
    const CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30

typedef unsigned int (__fastcall* TWordWrapCharWidthCallback)(unsigned short c);
typedef unsigned int (__fastcall* TWordWrapCallback2)();
void __fastcall WordWrap_SetOption(long option, long value);                            // 0x00A0FDB0
void __fastcall WordWrap_SetCallback(TWordWrapCharWidthCallback charWidth, TWordWrapCallback2 callback2);   // 0x00A0FD90
const unsigned short* __fastcall WordWrap_FindNextLineW(const unsigned short* text, unsigned int width, const unsigned short** lineEnd);   // 0x00A10390

static const unsigned short kNL[2] = { 10, 0 };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CCPPointerInfo;
class CFontBank { public:
    void* _vf;
    long Height;                                            // +0x04
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

namespace NUISystem {
struct CComponentInterface {
    virtual void v000() { }
};
struct CDrawableInterface {
    virtual void v000() { }
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000() { }
};
class CDraggableInto : public CComponentInterface, public CDrawableInterface, public CDraggableInterface { public:
    unsigned char _pad1c[0x15c - 0x1c];
};
class CText : public CDraggableInto { public:
    CWideString            Value;                  // +0x15c
    CCountedPointer<CFontBank> PFont;              // +0x160
    virtual void v000() { }
    static unsigned int __fastcall GetCharWidthW(unsigned short c);   // 0x0054F4B0
    long FitInWindow(CWideString& text, const C2DBoxF& window);
};

long CText::FitInWindow(CWideString& text, const C2DBoxF& window)
{
    long height = 0;
    CWideString result;
    const unsigned short* start = text.GetAsCharArray();
    if (window.BRX - window.TLX <= 0.0f && window.BRY - window.TLY <= 0.0f)
        return PFont->Height;
    WordWrap_SetOption(0, 1);
    WordWrap_SetCallback(GetCharWidthW, 0);
    const unsigned short* next;
    do {
        float left = window.TLX;
        const unsigned short* lineEnd = 0;
        next = WordWrap_FindNextLineW(start, (long)(window.BRX - left), &lineEnd);
        CWideString line;
        if (next)
            line.SetAsSubString(start, next - start);
        else
            line = start;
        if (!line.Contains(kNL) && next)
            result += line + kNL;
        else
            result += line;
        if (line.Contains(kNL)) {
            long pos = line.FindCharPos((unsigned short)10);
            for (long i = 0; i < line.GetLength() - pos - 1; ++i)
                height += PFont->Height;
        }
        height += PFont->Height;
        start = next;
    } while (next);
    text = result;
    return height;
}
}

typedef unsigned short wch;
static long wlen(const wch* s) { long n = 0; while (s[n]) ++n; return n; }
static wch* wdup(const wch* s, long n) { wch* d = (wch*)std::malloc((n + 1) * sizeof(wch)); for (long i = 0; i < n; ++i) d[i] = s[i]; d[n] = 0; return d; }
static const wch kEmpty[1] = { 0 };
CWideString::CWideString() : PStringData(wdup(kEmpty, 0)) { }
CWideString::CWideString(const wch* t) : PStringData(wdup(t, wlen(t))) { }
CWideString::~CWideString() { std::free(PStringData); }
const wch* CWideString::GetAsCharArray() const { return (const wch*)PStringData; }
bool CWideString::Contains(const CWideString& t) const {
    const wch* a = GetAsCharArray(); const wch* b = t.GetAsCharArray(); long na = wlen(a), nb = wlen(b);
    for (long i = 0; i + nb <= na; ++i) { long j = 0; while (j < nb && a[i + j] == b[j]) ++j; if (j == nb) return true; }
    return false;
}
long CWideString::FindCharPos(wch c) const { const wch* a = GetAsCharArray(); for (long i = 0; a[i]; ++i) if (a[i] == c) return i; return -1; }
long CWideString::GetLength() const { return wlen(GetAsCharArray()); }
void CWideString::SetAsSubString(const wch* t, long n) { std::free(PStringData); PStringData = wdup(t, n); }
const CWideString& CWideString::operator=(const CWideString& o) { if (this != &o) { std::free(PStringData); PStringData = wdup(o.GetAsCharArray(), o.GetLength()); } return *this; }
const CWideString& CWideString::operator=(const wch* t) { std::free(PStringData); PStringData = wdup(t, wlen(t)); return *this; }
const CWideString& CWideString::operator+=(const CWideString& o) {
    long na = GetLength(), nb = o.GetLength(); wch* d = (wch*)std::malloc((na + nb + 1) * sizeof(wch));
    const wch* a = GetAsCharArray(); const wch* b = o.GetAsCharArray();
    for (long i = 0; i < na; ++i) d[i] = a[i]; for (long i = 0; i < nb; ++i) d[na + i] = b[i]; d[na + nb] = 0;
    std::free(PStringData); PStringData = d; return *this;
}
CWideString __fastcall operator+(const CWideString& l, const wch* r) { CWideString t; t = l; CWideString rr(r); t += rr; return t; }
static int g_opt, g_cb;
void __fastcall WordWrap_SetOption(long, long) { ++g_opt; }
void __fastcall WordWrap_SetCallback(TWordWrapCharWidthCallback, TWordWrapCallback2) { ++g_cb; }
const wch* __fastcall WordWrap_FindNextLineW(const wch* t, unsigned int width, const wch** end) { const wch* n = wlen(t) > (long)width ? t + width : 0; *end = n; return n; }
unsigned int __fastcall NUISystem::CText::GetCharWidthW(unsigned short) { return 1; }
static bool eq(const CWideString& s, const char* c) { const wch* a = s.GetAsCharArray(); long i = 0; for (; c[i]; ++i) if (a[i] != (wch)c[i]) return false; return a[i] == 0; }
static wch* W(const char* c) { long n = 0; while (c[n]) ++n; wch* d = (wch*)std::malloc((n + 1) * sizeof(wch)); for (long i = 0; i <= n; ++i) d[i] = (wch)c[i]; return d; }

int main()
{
    NUISystem::CText t; CFontBank font; font.Height = 7; t.PFont.Data = &font; t.PFont.Info = 0;
    C2DBoxF w; w.TLX = 1.0f; w.BRX = 4.5f; w.TLY = 0.0f; w.BRY = 10.0f;
    CWideString s(W("abcdefg"));
    long h = t.FitInWindow(s, w);
    if (h != 21 || !eq(s, "abc\ndef\ng") || g_opt != 1 || g_cb != 1) { std::printf("0054f8e0_TEST FAIL\n"); return 1; }
    CWideString s2(W("ab\ncd")); w.BRX = 20.0f;
    h = t.FitInWindow(s2, w);
    if (h != 21 || !eq(s2, "ab\ncd")) { std::printf("0054f8e0_TEST FAIL\n"); return 1; }
    C2DBoxF e; e.TLX = 2.0f; e.BRX = 2.0f; e.TLY = 5.0f; e.BRY = 4.0f;
    CWideString s3(W("xyz"));
    h = t.FitInWindow(s3, e);
    if (h != 7 || !eq(s3, "xyz") || g_opt != 2) { std::printf("0054f8e0_TEST FAIL\n"); return 1; }
    std::printf("0054f8e0_TEST PASS\n");
    return 0;
}