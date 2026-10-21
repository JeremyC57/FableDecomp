// CTexture::GetWidth(unsigned long) const -- the width of a mip level, from GetDimensions
// Leaf: GetWidth@CTexture
// Retail: Fable.exe @ 0x004372A0, 26 bytes, `ret 4`. Catalog label CSubtitleRenderer::GetTextWidthFromText is wrong: the body is
// CTexture::GetDimensions(level) (0x009F9DB0, ?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z, C2DExtentsI returned through a
// hidden pointer to an 8-byte local) and returns its X (+0x00). Ego_r.pdb ?GetWidth@CTexture@@QBEKK@Z. Callers are in
// CEnginePrimitiveManager2DSprites::RenderClockPrim (0x00BAFA00), with ecx = the texture.
class C2DExtentsI {                                     // Ego_r.pdb: long X, Y; non-POD (returned through a hidden pointer)
public:
    long X;                                              // +0x00
    long Y;                                              // +0x04
    C2DExtentsI() {}
};
class CTexture {
public:
    C2DExtentsI GetDimensions(unsigned long level) const;   // 0x009F9DB0
    unsigned long GetWidth(unsigned long level) const;
};
unsigned long CTexture::GetWidth(unsigned long level) const
{
    return GetDimensions(level).X;
}