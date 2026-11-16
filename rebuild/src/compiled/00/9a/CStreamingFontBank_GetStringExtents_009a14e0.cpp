// CStreamingFontBank::GetStringExtents(const wchar_t*, bool) -- measure a wide string's width/height in pixels (stop at the first newline when single-line)
// Leaf: ?GetStringExtents@CStreamingFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z
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
// Port (NF40, round 4): the PDB member (Ego_r.pdb / FableWin.pdb ?GetStringExtents@CStreamingFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z,
//   vtable 0x0129AB54 slot 1) defined on engine/CStreamingFontBank.h (FABLE_RETAIL_CSTREAMINGFONTBANK + _GLYPHLOOKUP over
//   engine/CFontBank.h: the +0x04 line height is CFontBank::FontHeight; PASS22_STREAMING_FONT_CTOR for the PB_W parameter) with engine/C2DExtentsF.h, CStreamingGlyph.h,
//   CStreamingGlyphData.h, CStreamingGlyphAdjustment.h. CStreamingGlyphLookup has no shared header (same local model as the
//   landed Render 0x009A25D0). The 0x013BDA40 adjustment table keeps this row's extern spelling (see NOTES).
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK_GLYPHLOOKUP
#define FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#include "engine/CStreamingFontBank.h"
#include "engine/C2DExtentsF.h"
#include "engine/CStreamingGlyph.h"
#include "engine/CStreamingGlyphData.h"
#include "engine/CStreamingGlyphAdjustment.h"

struct CStreamingGlyphLookup {         // Ego_r.pdb, 16 bytes
    unsigned short font:4;
    unsigned short cachePos:12;
    unsigned short ch;
    CStreamingGlyph indices;
    CStreamingGlyphData data;
};
extern CStreamingGlyphAdjustment* g_pGlyphAdjustments;   // 0x013BDA40

static inline int RoundToInt(float f)
{
    return (int)(f < 0.0f ? f - 0.5f : f + 0.5f);
}

C2DExtentsF CStreamingFontBank::GetStringExtents(const FableFontWideChar* str, bool singleLine) const
{
    C2DExtentsF extents(0.0f, 0.0f);
    float x = 0.0f;
    float y = (float)(unsigned long)(FontHeight + 1);
    while (*str) {
        unsigned short c = *str++;
        if (c == 0x0a) {
            if (singleLine)
                return C2DExtentsF(extents.X, extents.Y);
            x = 0.0f;
            y += (float)(unsigned long)(FontHeight + 1);
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
