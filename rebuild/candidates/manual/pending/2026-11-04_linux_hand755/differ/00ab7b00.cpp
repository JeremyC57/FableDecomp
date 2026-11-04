// CStaticFontBank::GetStringExtents(const wchar_t* text, bool singleLine) const -- the width and height of a string in this font: the widest line (each glyph's offset plus advance) and the height of all lines (one font height plus one per newline; a newline ends the measure when only one line is wanted)
// Grade: DIFFER(287v287) -- x87 variable allocation: VC7.1 keeps the running line width on the x87 stack where retail keeps the widest height there (width / maxWidth / height / lineHeight in frame words); the callee-saved pushes follow the first-character test (retail pushes all four up front)
// Leaf: ?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z
// Flags: none
//   tried: compound adds; `height = lineHeight + height` and `width = Offset + width + Advance` (these fixed the size and the operand order); declaring the widest height last | next: widest height as the return object built up front, or the loop as for(;;) with the test at the bottom; lane gap: x87 variable allocation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB7B00, 285 bytes (+3 int3 padding), `ret 0Ch`. Ego_r.pdb
//   `?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z` (virtual, C2DExtentsF by hidden pointer).
// Float pool 0x0122DEDC = 0.0f, 0x0122DCB4 = 2^32 (the unsigned-to-float fix-up).
// Shape: the line height is the unsigned font height + 1 as a float local, the running height starts as a copy of it;
//   the widest height stays on the x87 stack; the glyph comes from an inline lookup (range test in the caller, the bank
//   null test and the index test fall through to the caller's null test).
// Layout: engine/CStaticFontBank.h (+0x04 is CFontBank::FontHeight, unsigned long in Ego_r.pdb; the header calls it
//   `long m_maxHeight`), MinChar +0x24, MaxChar +0x28, GlyphBanks (GlyphBank[1024], 8 bytes each) +0x30; CGlyph
//   (24 bytes) Offset +0x10, Advance +0x14.
#include <stddef.h>
struct C2DExtentsF {
    float X, Y;
    C2DExtentsF() { }
    C2DExtentsF(float x, float y) : X(x), Y(y) { }
};
struct CGlyph {
    float Left, Top, Right, Bottom;             // +0x00
    short Offset;                               // +0x10
    short Width;                                // +0x12
    short Advance;                              // +0x14
};
struct GlyphBank {
    unsigned short FirstGlyph;                  // +0x00
    unsigned short NoGlyphs;                    // +0x02
    CGlyph*        Glyphs;                      // +0x04
};
class CStaticFontBank {
public:
    virtual void v00();
    unsigned long FontHeight;                   // +0x04 (CFontBank)
    unsigned char _pad08[0x24 - 0x08];
    long          MinChar;                      // +0x24
    long          MaxChar;                      // +0x28
    long          TotalNoGlyphs;                // +0x2c
    GlyphBank     GlyphBanks[1024];             // +0x30
    const CGlyph* GetGlyph(wchar_t c) const
    {
        const GlyphBank& bank = GlyphBanks[c >> 6];
        if (bank.Glyphs) {
            unsigned long index = (c & 63) - bank.FirstGlyph;
            if (index < bank.NoGlyphs)
                return &bank.Glyphs[index];
        }
        return 0;
    }
    virtual C2DExtentsF GetStringExtents(const wchar_t* text, bool singleLine) const;
};

C2DExtentsF CStaticFontBank::GetStringExtents(const wchar_t* text, bool singleLine) const
{
    float maxHeight = 0.0f;
    float lineHeight = (float)(FontHeight + 1);
    float maxWidth = 0.0f;
    C2DExtentsF current(0.0f, lineHeight);
    while (*text) {
        wchar_t c = *text++;
        if (c == '\n') {
            if (singleLine)
                break;
            current.X = 0.0f;
            current.Y = lineHeight + current.Y;
        }
        if ((long)c >= MinChar && (long)c <= MaxChar) {
            const CGlyph* glyph = GetGlyph(c);
            if (glyph)
                current.X = glyph->Offset + current.X + glyph->Advance;
            if (current.X > maxWidth)
                maxWidth = current.X;
            if (current.Y > maxHeight)
                maxHeight = current.Y;
        }
    }
    return C2DExtentsF(maxWidth, maxHeight);
}
