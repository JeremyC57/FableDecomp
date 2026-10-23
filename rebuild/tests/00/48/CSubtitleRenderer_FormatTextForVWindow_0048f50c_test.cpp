// 0048f50c behaviour test: CSubtitleRenderer::FormatTextForVWindow (match.cpp body, host-modelled)
#include <stdio.h>
#include <string>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
#define NEAR(a, b) (fabs((double)(a) - (double)(b)) < 1e-4)

typedef unsigned short wchar;
typedef std::basic_string<wchar> wstr;
static wstr W(const char* s) { wstr r; while (*s) r += (wchar)*s++; return r; }

class CWideString {
public:
    wstr s;
    CWideString() {}
    CWideString(const wstr& v) : s(v) {}
    wchar* GetBuffer() const { return (wchar*)s.c_str(); }
    bool operator!=(const wchar* text) const { return s != wstr(text); }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct C2DExtentsI { long X, Y; };
struct C2DExtentsF { float Width, Height; };

static float GFAXToVX(float x) { return x * 2.0f; }
static float GFAYToVY(float y) { return y * 3.0f; }
static double g_VirtualScreenWidth = 1000.0;
static void GFLimit(float& value, const float& minimum, const float& maximum)
{ if (value < minimum) value = minimum; if (value > maximum) value = maximum; }

class CFontBank {
public:
    float GetMaxHeight() const { return 5.0f; }
    C2DExtentsF GetVStringExtents(const CWideString& text) const { C2DExtentsF e = { 10.0f * text.s.size(), 1.0f }; return e; }
    float GetVFontMaxHeight() const { return GFAYToVY(GetMaxHeight()); }
};
static const CFontBank* g_pWordWrapFont = 0;
static long WordWrapCharWidth(wchar) { return 1; }
static int g_opt = 0; static void* g_cb = 0;
static void WordWrap_SetOption(long, long value) { g_opt = (int)value; }
static void WordWrap_SetCallback(long (*charWidth)(wchar), void*) { g_cb = (void*)charWidth; }

struct CWStringParser {
    const wchar* String; unsigned long StringPos;
    CWStringParser(wchar* text) : String(text), StringPos(0) {}
    void Set(wchar* text) { String = text; StringPos = 0; }
    void SkipWhitespace() { while (String[StringPos] == ' ' || String[StringPos] == '\n') ++StringPos; }
    CWideString ReadAsStringUntilCharacter(wchar stop)
    { wstr r; while (String[StringPos] && String[StringPos] != stop) r += String[StringPos++]; return CWideString(r); }
};
struct CDisplayManager { C2DExtentsI GetRenderTargetDimensions() const { C2DExtentsI e = { 1280, 720 }; return e; } };
static CDisplayManager g_dm; static CDisplayManager* g_pDisplayManager = &g_dm;
struct CPlayerGuiDef { float TextBoxMaxWidth, TextBoxMinWidth; };
static CPlayerGuiDef g_gui = { 50.0f, 10.0f };
static const CPlayerGuiDef* PeekGuiDef() { return &g_gui; }

static wstr g_wrapped;
class CSubtitleRenderer {
public:
    unsigned long DisplayFlags;
    CWideString WordWrapText(const C2DBoxF&, const CFontBank&, const CWideString&) const { return CWideString(g_wrapped); }
    void FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText,
                              C2DBoxF& outBox) const
    {
        g_pWordWrapFont = &font;
        WordWrap_SetOption(0, 1);
        WordWrap_SetCallback(WordWrapCharWidth, 0);
        outText = WordWrapText(box, font, text);
        float maxWidth = 0.0f;
        long lines = 0;
        CWStringParser parser(text.GetBuffer());
        parser.Set(outText.GetBuffer());
        static const wchar empty[1] = { 0 };
        CWideString line = parser.ReadAsStringUntilCharacter(L'\n');
        while (line != empty) {
            float width = font.GetVStringExtents(line).Width;
            if (width > maxWidth)
                maxWidth = width;
            ++lines;
            parser.SkipWhitespace();
            line = parser.ReadAsStringUntilCharacter(L'\n');
        }
        if (!(DisplayFlags & 2) || (DisplayFlags & 0x28))
            ++lines;
        if (lines == 0)
            lines = 1;
        float scale = g_pDisplayManager->GetRenderTargetDimensions().X * (1.0f / 640.0f);
        const CPlayerGuiDef* def = PeekGuiDef();
        GFLimit(maxWidth, GFAXToVX(def->TextBoxMinWidth) * scale, GFAXToVX(def->TextBoxMaxWidth) * scale);
        outBox = box;
        outBox.TLX = (float)((g_VirtualScreenWidth - maxWidth) * 0.5);
        outBox.BRX = outBox.TLX + maxWidth;
        outBox.TLY = outBox.BRY - font.GetVFontMaxHeight() * lines;
    }
};

int main()
{
    CFontBank font; CSubtitleRenderer r; C2DBoxF box = { 1, 2, 3, 400 }, out; CWideString outText;
    CWideString text(W("ignored"));
    // two lines (widest 12 chars = 120 -> clamped to max 50*2*2 = 200? no: 120 within [40, 200]); flags 0 -> one extra line
    g_wrapped = W("hello world!\nabc");
    r.DisplayFlags = 0;
    r.FormatTextForVWindow(box, font, text, outText, out);
    CHECK(outText.s == g_wrapped && g_pWordWrapFont == &font && g_opt == 1 && g_cb != 0);
    CHECK(NEAR(out.TLX, (1000.0 - 120.0) * 0.5) && NEAR(out.BRX, out.TLX + 120.0f) && NEAR(out.BRY, 400.0f));
    CHECK(NEAR(out.TLY, 400.0f - 15.0f * 3));
    // flag 2 alone: no extra line; short text clamps up to the minimum width 10*2*2 = 40
    g_wrapped = W("ab");
    r.DisplayFlags = 2;
    r.FormatTextForVWindow(box, font, text, outText, out);
    CHECK(NEAR(out.BRX - out.TLX, 40.0f) && NEAR(out.TLY, 400.0f - 15.0f * 1));
    // flags 2|8: extra line again; long line clamps down to 200
    g_wrapped = W("abcdefghijklmnopqrstuvwxyz");
    r.DisplayFlags = 0x0a;
    r.FormatTextForVWindow(box, font, text, outText, out);
    CHECK(NEAR(out.BRX - out.TLX, 200.0f) && NEAR(out.TLX, 400.0f) && NEAR(out.TLY, 400.0f - 15.0f * 2));
    // empty text with flag 2: zero lines becomes one
    g_wrapped = W("");
    r.DisplayFlags = 2;
    r.FormatTextForVWindow(box, font, text, outText, out);
    CHECK(NEAR(out.TLY, 400.0f - 15.0f) && NEAR(out.BRX - out.TLX, 40.0f) && NEAR(out.TLY + 0, out.TLY));

    if (!g_fail) printf("0048f50c_TEST PASS\n");
    return g_fail;
}