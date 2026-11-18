// Behaviour test for 0x009BCEF0 CMovieSubtitles::GetCharWidthW(unsigned short c): the row's defines, includes and body verbatim
// over the shared headers, with recording doubles for CWideString (ctor / Sprintf / GetAsCharArray / dtor), the font bank's slot 1
// GetStringExtents and GFAXToVX / GFAYToVY. Checks: the receiver of every string call is the one local string; the format is
// L"%c" and the character is passed zero-extended (0x8001 stays 0x8001); the font called is the one in PFont.Data with the
// string's characters and `false`; X goes to GFAXToVX and only its result is returned, truncated (fractions dropped, 0 stays
// 0); Y goes to GFAYToVY exactly once; the string is destroyed exactly once, before the result is used.
#include <stdio.h>
#include <stdarg.h>
#define private public
#define protected public
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CMOVIESUBTITLES_GETCHARWIDTHW
#include "fable_string.h"
#include "engine/C2DExtentsF.h"
#include "engine/CFontBank.h"
#include "engine/CMovieSubtitles.h"
#undef private
#undef protected

unsigned int __fastcall CMovieSubtitles::GetCharWidthW(unsigned short c)
{
    CWideString char_text;
    char_text.Sprintf(L"%c", c);
    unsigned int res = PFont->GetVStringExtents(char_text).X;
    return res;
}

// ---- doubles ----
static const void* g_ctor_this; static const void* g_sprintf_this; static const void* g_chars_this; static const void* g_dtor_this;
static int g_ctors, g_dtors, g_sprintfs, g_xcalls, g_ycalls, g_extent_calls;
static unsigned g_char; static bool g_fmt_ok; static float g_xin, g_yin;
static const unsigned short g_chars[2] = { 'Q', 0 };
static const unsigned short* g_extent_text; static bool g_extent_wrap; static const void* g_extent_this;
static float g_ext_x, g_ext_y;
CWideString::CWideString() { g_ctor_this = this; ++g_ctors; PStringData = 0; }
CWideString::~CWideString() { g_dtor_this = this; ++g_dtors; }
void __cdecl CWideString::Sprintf(const unsigned short* format, ...)
{
    g_sprintf_this = this; ++g_sprintfs;
    g_fmt_ok = format[0] == '%' && format[1] == 'c' && format[2] == 0;
    va_list ap; va_start(ap, format); g_char = va_arg(ap, unsigned); va_end(ap);
}
const unsigned short* CWideString::GetAsCharArray() const { g_chars_this = this; return g_chars; }
float __fastcall GFAXToVX(float x) { ++g_xcalls; g_xin = x; return x * 0.5f; }
float __fastcall GFAYToVY(float y) { ++g_ycalls; g_yin = y; return y * 4.0f; }
CCharString::CCharString() : PStringData(0) { }
CCharString::~CCharString() { }
CFontBank::~CFontBank() { }
bool CFontBank::Preload(const CWideString&, bool) const { return false; }
class CTestFont : public CFontBank {
public:
    virtual C2DExtentsF GetStringExtents(const unsigned short* text, bool wrap) const
    {
        ++g_extent_calls; g_extent_this = this; g_extent_text = text; g_extent_wrap = wrap;
        if (g_dtors != 0) g_extent_text = 0;                       // the string must still be alive here
        return C2DExtentsF(g_ext_x, g_ext_y);
    }
    virtual float GetFontMaxHeight() const { return 0.0f; }
    virtual void PrepareToRender(bool) const { }
    virtual void Render(const CWideString&, const C3DVector&, float, const CRGBColour&, unsigned long, const CTexture*) const { }
};
CCountedPointer<CFontBank> CMovieSubtitles::PFont;

static int fail(int n) { printf("009bcef0_TEST FAIL %d\n", n); return 1; }
static void reset() { g_ctors = g_dtors = g_sprintfs = g_xcalls = g_ycalls = g_extent_calls = 0; g_ctor_this = g_sprintf_this = g_chars_this = g_dtor_this = 0; }

int main()
{
    CTestFont font, other;
    CMovieSubtitles::PFont.Data = &font;
    reset(); g_ext_x = 17.0f; g_ext_y = 3.0f;
    unsigned int w = CMovieSubtitles::GetCharWidthW(0x8001);
    if (w != 8) return fail(1);                                    // 17 * 0.5 = 8.5 truncated
    if (g_ctors != 1 || g_dtors != 1 || g_sprintfs != 1) return fail(2);
    if (g_sprintf_this != g_ctor_this || g_chars_this != g_ctor_this || g_dtor_this != g_ctor_this) return fail(3);
    if (!g_fmt_ok || g_char != 0x8001) return fail(4);           // zero-extended character
    if (g_extent_calls != 1 || g_extent_this != &font || g_extent_text != g_chars || g_extent_wrap) return fail(5);
    if (g_xcalls != 1 || g_xin != 17.0f || g_ycalls != 1 || g_yin != 3.0f) return fail(6);
    reset(); g_ext_x = 0.0f; g_ext_y = 100.0f;                    // Y never leaks into the result
    if (CMovieSubtitles::GetCharWidthW('A') != 0 || g_char != 'A') return fail(7);
    reset(); g_ext_x = 7.0f; g_ext_y = 0.0f;                      // exact: 3.5 -> 3
    CMovieSubtitles::PFont.Data = &other;
    if (CMovieSubtitles::GetCharWidthW('w') != 3 || g_extent_this != &other) return fail(8);
    reset(); g_ext_x = 4.0f; g_ext_y = 1.0f;                      // whole value stays whole
    if (CMovieSubtitles::GetCharWidthW(0xFFFF) != 2 || g_char != 0xFFFF) return fail(9);
    CMovieSubtitles::PFont.Data = 0;
    printf("009bcef0_TEST PASS\n");
    return 0;
}