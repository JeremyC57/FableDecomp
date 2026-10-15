// CStreamingFontBank::FindGlyph(unsigned short character) const -- the glyph record for a character (a non-breaking space maps to a space): find its 64-glyph bank, LZO-unpack a compressed bank into the cache, and return the entry when it lies in the bank's range and is present (low 24 bits set), else 0
// Grade: DIFFER(143v142) -- retail tests the compressed bit as `test byte ptr [eax+1],80h ; je`, ours `mov cl,[eax+1] ; test cl,cl ; jns`; the 0x104 size store then sinks below the pushes
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A4160, 142 bytes; `ret 4`. Callees: lzo1x_decompress_safe @ 0x00C08170 (__cdecl).
// Layout: engine/CStreamingFontBank.h: glyphBankCache +0x14, glyphBankCacheIndex +0x18, GlyphBanks[1024] +0x2c.
//         A packed bank starts with a 16-bit size word (bit 15 = LZO-compressed); an unpacked bank is {first, count, glyphs[]}.
// Shape: the compressed flag is tested on the size word's high byte (`test byte ptr [p+1],80h`), the size read as a word
//        afterwards (`and edx,7FFFh`); the unpacked-size local (0x104) is homed in the dead character-argument slot; the 16-bit compare happens on the
//        argument register before the character is widened.
// Leaf: FindGlyph
struct CStreamingGlyph {
    unsigned long Data;                            // low 24 bits: present / offset data
};
struct GlyphBank {
    unsigned short  First;                         // first character index in the bank (low 6 bits)
    unsigned short  Count;
    CStreamingGlyph Glyphs[1];
};
struct CPackedGlyphBank {
    union {
        unsigned short PackedWord;                 // bits 0..14: the packed length
        struct { unsigned char Low, High; } Bytes; // High bit 7: LZO-compressed
    };
    bool IsCompressed() const { return (Bytes.High & 0x80) != 0; }
    unsigned long GetPackedSize() const { return PackedWord & 0x7fff; }
    unsigned char   Data[1];
};
extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char* src, unsigned long srcLength, unsigned char* dst,
                                             unsigned long* dstLength, void* workMemory);   // 0x00C08170
class CStreamingFontDataBank;
class CTexture;
class CStreamingFontBank {
public:
    void*                   __vftable;             // +0x00
    long                    m_maxHeight;           // +0x04
    unsigned char           _pad_0x08[0x8];        // +0x08
    CStreamingFontDataBank* data_bank;             // +0x10
    GlyphBank*              glyphBankCache;        // +0x14
    unsigned long           glyphBankCacheIndex;   // +0x18
    unsigned long           MaxHeight;             // +0x1c
    long                    Weight;                // +0x20
    bool                    Italics;               // +0x24
    void*                   GlyphBankMemory;       // +0x28
    CPackedGlyphBank*       GlyphBanks[1024];      // +0x2c
protected:
    const CStreamingGlyph* FindGlyph(unsigned short character) const;
};

const CStreamingGlyph* CStreamingFontBank::FindGlyph(unsigned short character) const
{
    if (character == 0xa0)
        character = 0x20;
    GlyphBank* bank = glyphBankCache;
    unsigned long index = character;
    unsigned long bankIndex = index >> 6;
    if (glyphBankCacheIndex != bankIndex) {
        CPackedGlyphBank* packed = GlyphBanks[bankIndex];
        if (!packed)
            return 0;
        if (packed->IsCompressed()) {
            unsigned long size = 0x104;
            lzo1x_decompress_safe(packed->Data, packed->GetPackedSize(), (unsigned char*)bank, &size, 0);
            const_cast<CStreamingFontBank*>(this)->glyphBankCacheIndex = bankIndex;
        }
        else
            bank = (GlyphBank*)packed->Data;
    }
    unsigned long slot = (index & 0x3f) - bank->First;
    if (slot < bank->Count) {
        const CStreamingGlyph* glyph = &bank->Glyphs[slot];
        if (glyph->Data & 0xffffff)
            return glyph;
    }
    return 0;
}
