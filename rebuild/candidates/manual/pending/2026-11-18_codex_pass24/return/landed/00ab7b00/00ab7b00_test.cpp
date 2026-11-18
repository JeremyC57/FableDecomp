#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "engine/C2DExtentsF.h"
#include <cassert>
#include <cstdio>

typedef unsigned short FableFontWideChar;

struct CGlyph
{
    short Offset;
    short Advance;
};

struct GlyphBank
{
    unsigned short FirstGlyph;
    unsigned short NoGlyphs;
    CGlyph* Glyphs;
};

class CStaticFontBank
{
public:
    unsigned int FontHeight;
    int MinChar;
    int MaxChar;
    GlyphBank GlyphBanks[1024];

    CStaticFontBank()
        : FontHeight(9), MinChar(10), MaxChar(0x0101)
    {
        for (unsigned int i = 0; i < 1024; ++i) {
            GlyphBanks[i].FirstGlyph = 0;
            GlyphBanks[i].NoGlyphs = 0;
            GlyphBanks[i].Glyphs = 0;
        }
    }

    CGlyph* FindGlyph(unsigned short ch) const
    {
        const unsigned int bankIndex = ch >> 6;
        const GlyphBank& bank = GlyphBanks[bankIndex];
        if (!bank.Glyphs)
            return 0;
        const unsigned int index = (ch & 63) - bank.FirstGlyph;
        if (index >= bank.NoGlyphs)
            return 0;
        return &bank.Glyphs[index];
    }

    C2DExtentsF GetStringExtents(const FableFontWideChar*, bool) const;
};

// The selected implementation is reproduced verbatim for this standalone fixture.
C2DExtentsF CStaticFontBank::GetStringExtents(const FableFontWideChar* string, bool first_line_only) const
{
    C2DExtentsF extents(0.0f, 0.0f);
    float sx = 0.0f;
    float sy = (float)(unsigned long)(FontHeight + 1);
    while (*string) {
        FableFontWideChar letter = *string++;
        if (letter == 0x0a) {
            if (first_line_only)
                return extents;
            sx = 0.0f;
            sy += (float)(unsigned long)(FontHeight + 1);
        }
        if ((long)letter < MinChar || (long)letter > MaxChar)
            continue;
        CGlyph* pglyph = FindGlyph(letter);
        if (pglyph)
            sx = (pglyph->Offset + sx) + pglyph->Advance;
        if (sx > extents.X)
            extents.X = sx;
        if (sy > extents.Y)
            extents.Y = sy;
    }
    return extents;
}

static void Check(const CStaticFontBank& bank, const FableFontWideChar* text,
                  bool first_line_only, float width, float height)
{
    const C2DExtentsF extents = bank.GetStringExtents(text, first_line_only);
    assert(extents.X == width && extents.Y == height);
}

int main()
{
    CStaticFontBank bank;
    CGlyph latin[] = { {1, 5}, {-2, 8}, {-7, 1} };
    CGlyph extended[] = { {-1, 9}, {2, 3}, {900, 900} };
    bank.GlyphBanks[1].Glyphs = latin;
    bank.GlyphBanks[1].FirstGlyph = 33;
    bank.GlyphBanks[1].NoGlyphs = 3;
    bank.GlyphBanks[4].Glyphs = extended;
    bank.GlyphBanks[4].NoGlyphs = 3;

    const FableFontWideChar empty[] = {0};
    const FableFontWideChar beyond[] = {0x0102, 9, 0};
    const FableFontWideChar no_glyph[] = {'@', 'd', 0x0080, 0};
    const FableFontWideChar upper[] = {0x0101, 0};
    const FableFontWideChar negative[] = {'c', 0};
    const FableFontWideChar width[] = {'a', 'b', 'c', 0x0100, 0};
    const FableFontWideChar two_lines[] = {'a', 10, 'b', 0x0100, 0};
    const FableFontWideChar final_break[] = {'a', 10, 0};
    const FableFontWideChar leading_break[] = {10, 'a', 0};

    Check(bank, empty, false, 0, 0);
    Check(bank, beyond, false, 0, 0);
    Check(bank, no_glyph, false, 0, 10);
    Check(bank, upper, false, 5, 10);
    Check(bank, negative, false, 0, 10);
    Check(bank, width, false, 14, 10);
    Check(bank, two_lines, false, 14, 20);
    Check(bank, two_lines, true, 6, 10);
    Check(bank, final_break, false, 6, 20);
    Check(bank, leading_break, false, 6, 20);
    Check(bank, leading_break, true, 0, 0);
    bank.MinChar = 11;
    Check(bank, final_break, false, 6, 10);
    bank.FontHeight = 0x7fffffffu;
    Check(bank, upper, false, 5, 2147483648.0f);
    bank.FontHeight = 0xffffffffu;
    Check(bank, upper, false, 5, 0);
    std::puts("00ab7b00_TEST PASS");
    return 0;
}
