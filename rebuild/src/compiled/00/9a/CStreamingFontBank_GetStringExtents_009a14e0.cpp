// CStreamingFontBank::GetStringExtents(const wchar_t*, bool) -- measure a wide string's width/height in pixels (stop at the first newline when single-line)
// Leaf: GetStringExtents
// Retail 0x009A14E0, 384 bytes (virtual, const, hidden C2DExtentsF result: `ret 0Ch`). Callees: CStreamingFontBank::GlyphLookup
// 0x009A1140 (__fastcall, ushort char), __ftol2 0x00BFEA70. Globals: the glyph adjustment table pointer 0x013BDA40.
// Pools: 2^32 0x0122DCB4 (unsigned-to-float fix-up), 0.01f 0x01230A5C, 0.0f 0x0122DEDC, 0.5f 0x0122F59C.
// Layout: engine/CStreamingFontBank.h (m_maxHeight +0x04); the glyph lookup record is the PDB CStreamingGlyphLookup
// (indices dword +4 whose top byte is AdjustmentIndex, data dword +0xC whose bits 23..30 are Advance).
// Shape: the line height is `(unsigned long)(m_maxHeight + 1)` converted to float (fild + 2^32 fix-up); a newline adds a
// line and resets the pen but still looks the newline glyph up; U+00A0 measures as a space; each glyph advances the pen
// by the rounded `scale * 0.01f * Advance` truncated to a signed char; running maxima are kept for both axes.
// Shape lessons: the two maxima are one C2DExtentsF local declared (with the pen) BEFORE the line height, so the zero
// stores sit between the fild and the fix-up; the loop walks the parameter itself (`while (*str) { c = *str++; ...}`,
// giving retail's `xor eax,eax ; mov ax,[esi]`); `Advance * (Scale * 0.01f)` keeps the scale product first; both exits
// build the result through the two-float ctor so the early return keeps its own tail.
// The lane has no native wchar_t: the string is `const unsigned short*` (retail mangles `PB_W`).
#include "engine/CStreamingFontBank.h"

class C2DExtentsF { public:
    float X, Y;
    C2DExtentsF(float x, float y) : X(x), Y(y) {}
};

class CStreamingGlyph { public:
    unsigned long GlyphIndex : 24;
    unsigned long AdjustmentIndex : 8;
};
class CStreamingGlyphData { public:
    signed char   OffsetX, OffsetY, Width, Height;
    unsigned long MemOffset : 23;
    unsigned long Advance : 8;
    unsigned long BigChar : 1;
};
struct CStreamingGlyphLookup {
    unsigned short      Packed;     // font:4 / cachePos:12
    unsigned short      ch;         // +2
    CStreamingGlyph     indices;    // +4
    CStreamingGlyphData data;       // +8
};

struct SGlyphAdjustment {
    unsigned char Scale;
    unsigned char Spare;
};
extern SGlyphAdjustment* g_pGlyphAdjustments;   // 0x013BDA40

struct CStreamingFontBankX : CStreamingFontBank {
    CStreamingGlyphLookup* GlyphLookup(unsigned short ch) const;   // 0x009A1140
    C2DExtentsF GetStringExtents(const unsigned short* str, bool singleLine) const;
};

static inline int RoundToInt(float f)
{
    return (int)(f < 0.0f ? f - 0.5f : f + 0.5f);
}

C2DExtentsF CStreamingFontBankX::GetStringExtents(const unsigned short* str, bool singleLine) const
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
        CStreamingGlyphLookup* g = GlyphLookup(c);
        if (g)
            x += (signed char)RoundToInt((float)g->data.Advance * ((float)g_pGlyphAdjustments[g->indices.AdjustmentIndex].Scale * 0.01f));
        if (x > extents.X)
            extents.X = x;
        if (y > extents.Y)
            extents.Y = y;
    }
    return C2DExtentsF(extents.X, extents.Y);
}