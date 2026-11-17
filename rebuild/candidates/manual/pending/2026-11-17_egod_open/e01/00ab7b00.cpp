// CStaticFontBank::GetStringExtents(const wchar_t* string, bool first_line_only) const -- the width and height of a string in this font: the widest pen position (each glyph adds its offset and advance) and the height of all lines (one line height plus one per newline; a newline returns at once when only the first line is wanted)
// Grade: DIFFER(287v287) -- x87-order: the glyph step loads Advance first (fild Advance ; fadd sx ; fiadd Offset) where retail loads Offset first (0x00AB7BB2..0x00AB7BCA); everything else identical
// Leaf: ?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB7B00, 287 bytes, thiscall virtual (slot 1), `ret 0Ch`, C2DExtentsF by hidden pointer.
// Locals: Ego_d.pdb (string, first_line_only; extents, sx, sy; loop block pglyph, letter); FindGlyph = FableWin ?FindGlyph@CStaticFontBank@@IBEPAVCGlyph@@G@Z (overlay inline).
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#define FABLE_RETAIL_CSTATICFONTBANK
#define FABLE_RETAIL_CSTATICFONTBANK_CTOR
#define FABLE_RETAIL_CSTATICFONTBANK_FINDGLYPH
#include <stddef.h>
#include "engine/CStaticFontBank.h"
#include "engine/C2DExtentsF.h"

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
            sx = sx + pglyph->Offset + pglyph->Advance;
        if (sx > extents.X)
            extents.X = sx;
        if (sy > extents.Y)
            extents.Y = sy;
    }
    return extents;
}
