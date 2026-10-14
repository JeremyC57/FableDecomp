// CGraphicFrameData::Sizeof() -- 0x13 bytes plus the texture's size (its masked header word plus 12) when the data owns a texture (+0x1a bit 2) and holds one (+8), else plus 8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FCBF0, length 42 bytes.
// Callees: none (the texture's inline Sizeof: `[texture+4] & 0FFFFFFFh + 0Ch`). Shape: the no-texture answer 0x1B preloaded before the flag branch, the two `+13h` adds.
struct CTexture_009fcbf0 {
    long          m_0;
    unsigned long m_header;                        // +0x04 (the size in the low 28 bits)
    long Sizeof() { return (m_header & 0xfffffff) + 0xc; }
};

struct CGraphicFrameData {
    unsigned char       _pad[8];
    CTexture_009fcbf0*  m_pTexture;                // +0x08
    unsigned char       _pad0c[0xe];
    unsigned char       m_flags;                   // +0x1a (bit 2 = owns a texture)
    long Sizeof();
};

static __forceinline long TextureSizeof_009fcbf0(CTexture_009fcbf0* texture)   // 8 bytes for a null texture reference
{
    if (texture)
        return texture->Sizeof();
    return 8;
}

long CGraphicFrameData::Sizeof()
{
    long size = 0x13 + 8;                          // the header and a null texture reference
    if (m_flags & 4)
        size = TextureSizeof_009fcbf0(m_pTexture) + 0x13;
    return size;
}
