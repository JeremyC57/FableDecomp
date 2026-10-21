// Behaviour test for CStreamingFontBank::GetStringExtents (0x009A14E0): a copy of the logic over a fake glyph table.
#include <cstdio>

struct C2DExtentsF {
    float X, Y;
    C2DExtentsF(float x, float y) : X(x), Y(y) {}
};
struct SGlyphAdjustment { unsigned char Scale; unsigned char Spare; };
static SGlyphAdjustment g_table[4] = { {100, 0}, {50, 0}, {200, 0}, {0, 0} };
static SGlyphAdjustment* g_pGlyphAdjustments = g_table;

struct Glyph { unsigned int AdjustmentIndex; unsigned int Advance; };

struct FontBank {
    long m_maxHeight;
    Glyph glyphs[128];
    bool present[128];
    unsigned short lastLookup;
    const Glyph* GlyphLookup(unsigned short ch) const
    {
        if (ch < 128 && present[ch]) return &glyphs[ch];
        return 0;
    }
    static int RoundToInt(float f) { return (int)(f < 0.0f ? f - 0.5f : f + 0.5f); }
    C2DExtentsF GetStringExtents(const unsigned short* str, bool singleLine) const
    {
        C2DExtentsF extents(0.0f, 0.0f);
        float x = 0.0f;
        float y = (float)(unsigned long)(m_maxHeight + 1);
        while (*str) {
            unsigned short c = *str++;
            if (c == 0x0a) {
                if (singleLine)
                    return C2DExtentsF(extents.X, extents.Y);
                x = 0.0f;
                y += (float)(unsigned long)(m_maxHeight + 1);
            } else if (c == 0xa0) {
                c = 0x20;
            }
            const Glyph* g = GlyphLookup(c);
            if (g)
                x += (signed char)RoundToInt((float)g->Advance * ((float)g_pGlyphAdjustments[g->AdjustmentIndex].Scale * 0.01f));
            if (x > extents.X) extents.X = x;
            if (y > extents.Y) extents.Y = y;
        }
        return C2DExtentsF(extents.X, extents.Y);
    }
};

static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    static FontBank fb;
    fb.m_maxHeight = 9;            // line height 10
    for (int i = 0; i < 128; ++i) fb.present[i] = false;
    fb.present['a'] = true; fb.glyphs['a'].AdjustmentIndex = 0; fb.glyphs['a'].Advance = 7;   // 7 * 1.0 = 7
    fb.present['b'] = true; fb.glyphs['b'].AdjustmentIndex = 1; fb.glyphs['b'].Advance = 5;   // 5 * 0.5 = 2.5 -> 3
    fb.present[' '] = true; fb.glyphs[' '].AdjustmentIndex = 2; fb.glyphs[' '].Advance = 2;   // 2 * 2.0 = 4
    unsigned short s1[] = { 'a', 'b', 'a', 0 };
    C2DExtentsF e = fb.GetStringExtents(s1, false);
    if (!near(e.X, 17.0f) || !near(e.Y, 10.0f)) { std::printf("009a14e0_TEST FAIL\n"); return 1; }
    unsigned short s2[] = { 'a', 0x0a, 'a', 'a', 0xa0, 0 };   // nbsp measures as a space
    e = fb.GetStringExtents(s2, false);
    if (!near(e.X, 18.0f) || !near(e.Y, 20.0f)) { std::printf("009a14e0_TEST FAIL\n"); return 1; }
    e = fb.GetStringExtents(s2, true);                          // stops at the newline
    if (!near(e.X, 7.0f) || !near(e.Y, 10.0f)) { std::printf("009a14e0_TEST FAIL\n"); return 1; }
    unsigned short s3[] = { 0 };
    e = fb.GetStringExtents(s3, false);
    if (!near(e.X, 0.0f) || !near(e.Y, 0.0f)) { std::printf("009a14e0_TEST FAIL\n"); return 1; }
    std::printf("009a14e0_TEST PASS\n");
    return 0;
}
