// CGraphicFrameData::Sizeof() -- 0x13 bytes plus the texture's size (its masked header word plus 12) when the data owns a texture (+0x1a bit 2) and holds one (+8), else plus 8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FCBF0, length 42 bytes.
// Callees: none (the texture's inline Sizeof: `[texture+4] & 0FFFFFFFh + 0Ch`). Shape: the no-texture answer 0x1B preloaded before the flag branch, the two `+13h` adds.
// Leaf: ?Sizeof@CGraphicFrameData@@QBEKXZ
// Name fix (agentNF5 2026-11-10): Ego_r.pdb / FableWin.pdb ?Sizeof@CGraphicFrameData@@QBEKXZ (const, unsigned long; the GFSizeof<CGraphicFrameData>
//   helper both PDBs carry calls it on a const object); was ?Sizeof@CGraphicFrameData@@QAEJXZ. Row debt kept: local models (the retail
//   CGraphicFrameData of engine/CGraphicFrame.h has no CTexture::Sizeof to expand).
#include "fable_compiler.h"
struct CTexture_009fcbf0 {
    long          m_0;
    unsigned long m_header;                        // +0x04 (the size in the low 28 bits)
    unsigned long Sizeof() const { return (m_header & 0xfffffff) + 0xc; }
};

class CGraphicFrameData { public:
    unsigned char       _pad[8];
    CTexture_009fcbf0*  m_pTexture;                // +0x08
    unsigned char       _pad0c[0xe];
    unsigned char       m_flags;                   // +0x1a (bit 2 = owns a texture)
    unsigned long Sizeof() const;
};

static FABLE_FORCEINLINE unsigned long TextureSizeof_009fcbf0(const CTexture_009fcbf0* texture)   // 8 bytes for a null texture reference
{
    if (texture)
        return texture->Sizeof();
    return 8;
}

unsigned long CGraphicFrameData::Sizeof() const
{
    unsigned long size = 0x13 + 8;                          // the header and a null texture reference
    if (m_flags & 4)
        size = TextureSizeof_009fcbf0(m_pTexture) + 0x13;
    return size;
}