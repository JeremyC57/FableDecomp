// Behaviour test for CStaticFontBank::GetStringExtents (0x00AB7B00), g++ model of the parked source.
#include <cstdio>
struct Glyph { short Offset, Advance; };
struct Font { unsigned long FontHeight; long MinChar, MaxChar; Glyph glyphs[128]; bool has[128]; };
struct Ext { float X, Y; };
static Ext Extents(const Font& f, const wchar_t* text, bool singleLine)
{
    float maxHeight = 0, lineHeight = (float)(f.FontHeight + 1), maxWidth = 0, width = 0, height = lineHeight;
    while (*text) {
        wchar_t c = *text++;
        if (c == L'\n') { if (singleLine) break; width = 0; height = lineHeight + height; }
        if ((long)c >= f.MinChar && (long)c <= f.MaxChar) {
            if (c < 128 && f.has[c]) width = f.glyphs[c].Offset + width + f.glyphs[c].Advance;
            if (width > maxWidth) maxWidth = width;
            if (height > maxHeight) maxHeight = height;
        }
    }
    Ext e = { maxWidth, maxHeight }; return e;
}
int main()
{
    Font f = {}; f.FontHeight = 9; f.MinChar = 10; f.MaxChar = 126;
    f.glyphs['a'].Offset = 1; f.glyphs['a'].Advance = 5; f.has['a'] = true;
    f.glyphs['b'].Offset = 0; f.glyphs['b'].Advance = 7; f.has['b'] = true;
    Ext e = Extents(f, L"ab", false);
    int ok = e.X == 13 && e.Y == 10;
    e = Extents(f, L"a\nabab", false); ok &= e.X == 26 && e.Y == 20;
    e = Extents(f, L"ab\nabab", true); ok &= e.X == 13 && e.Y == 10;
    e = Extents(f, L"", false); ok &= e.X == 0 && e.Y == 0;
    std::printf(ok ? "00ab7b00_TEST PASS\n" : "00ab7b00_TEST FAIL\n");
    return ok ? 0 : 1;
}
