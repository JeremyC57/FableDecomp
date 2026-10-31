// CStaticFontBank::GetStringExtents(const wchar_t* text, bool singleLine) const -- the width and height of a string in this font: the widest line (each glyph's offset plus advance) and the height of all lines (one font height plus one per newline; a newline ends the measure when only one line is wanted)
// Grade: DIFFER(287v287) -- wave 25: retail keeps maxHeight on the x87 stack and the line width in memory (5-word frame, maxima adjacent at +1Ch/+20h); VC7.1 here enregisters the width instead and shrink-wraps ebx/ebp/esi past the empty-text test. Tried this wave: C2DExtentsF line struct (scalarised, same), declaration order maxHeight-first (worse), sibling CStreamingFontBank 0x009A14E0 shape (extents struct declared first; same), class C2DExtentsF with copy ctor / Initialise (285v287), inline KeepMax(T&, const T&) (inert). LEVER FOUND: `maxWidth = Max(maxWidth, width)` with an STLport-style `const T& max(const T&, const T&)` flips the x87 choice to retail's (width homed, maxHeight on the stack, glyph sum order as retail) but adds an address select (297v287) -- next: a form that takes the width's address without the select.
// Leaf: ?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z
// Flags: /Zc:wchar_t
//   word with an integer 0 reset as retail; left: retail homes a second zeroed float (two zero stores at entry, 0x14
//   frame) and pushes ebx/ebp at entry, ours shrink-wraps them past the empty-text test (0x10 frame). Next: a sixth
//   float local / the height in memory
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
class C2DExtentsF {
public:
    float X, Y;
    C2DExtentsF() { }
    C2DExtentsF(float x, float y) : X(x), Y(y) { }
};
class CGlyph {
public:
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
protected:
    CGlyph* FindGlyph(unsigned short c) const             // Ego_r.pdb ?FindGlyph@CStaticFontBank@@IBEPAVCGlyph@@G@Z (inlined)
    {
        const GlyphBank& bank = GlyphBanks[c >> 6];
        if (bank.Glyphs) {
            unsigned long index = (c & 63) - bank.FirstGlyph;
            if (index < bank.NoGlyphs)
                return &bank.Glyphs[index];
        }
        return 0;
    }
public:
    virtual C2DExtentsF GetStringExtents(const wchar_t* text, bool singleLine) const;
};

C2DExtentsF CStaticFontBank::GetStringExtents(const wchar_t* text, bool singleLine) const
{
    float width = 0.0f;
    float maxHeight = 0.0f;
    float lineHeight = (float)(FontHeight + 1);
    float maxWidth = 0.0f;
    float height = lineHeight;
    while (*text) {
        wchar_t c = *text++;
        if (c == '\n') {
            if (singleLine)
                break;
            width = 0.0f;
            height = lineHeight + height;
        }
        if ((long)c >= MinChar && (long)c <= MaxChar) {
            const CGlyph* glyph = FindGlyph(c);
            if (glyph)
                width = glyph->Offset + width + glyph->Advance;
            if (width > maxWidth)
                maxWidth = width;
            if (height > maxHeight)
                maxHeight = height;
        }
    }
    return C2DExtentsF(maxWidth, maxHeight);
}
