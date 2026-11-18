// CStaticFontBank::GetStringExtents(const FableFontWideChar* string, bool first_line_only) -- measure the widest pen position and the line height
// Fidelity: associate the glyph offset with the current pen position before adding its advance at 0x00AB7BB2.
// Leaf: ?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB7B00, 287 bytes, thiscall virtual (slot 1), `ret 0Ch`, C2DExtentsF by hidden pointer.
// The parameter and local names are from Ego_d. Glyph metrics are signed.
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
            sx = (pglyph->Offset + sx) + pglyph->Advance;
        if (sx > extents.X)
            extents.X = sx;
        if (sy > extents.Y)
            extents.Y = sy;
    }
    return extents;
}
