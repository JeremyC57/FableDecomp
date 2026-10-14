// CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) -- memberwise copy: TextureIndex, BlendType, then the two CRGBColour members through their four-channel Set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BA48A, length 91 bytes.
// No callees. Layout = Ego_r.pdb CCompositeBlendDesc (16 bytes): TextureIndex +0, BlendType +4,
// DiffuseColour +8, SpecularColour +0xc. The two dwords are copied as plain moves; each colour's copy
// constructor goes through the inline Set(r, g, b, a) (landed shape 0x0043F490): all four channel
// bytes are loaded before any is stored and the stores land in R, G, B, A order, which with the
// D3DCOLOR byte layout (B, G, R, A) gives the retail permutation. The body keeps a frame pointer and
// spills two of the channel bytes (one to its single local slot, one into the dead high byte of the
// argument slot at [ebp+0Bh]) while still using ebx freely: an `optimize("y", off)` + `optimize("s", on)`
// translation unit, as the other NUISystem/engine data-description files.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRGBColour {                                             // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }   // forced: the size optimiser would call it out of line
};

struct CCompositeBlendDesc {
    unsigned long TextureIndex;                                 // +0x00
    long          BlendType;                                    // +0x04 (ECompositeBlendType)
    CRGBColour    DiffuseColour;                                // +0x08
    CRGBColour    SpecularColour;                               // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);
};

CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
    : TextureIndex(other.TextureIndex)
    , BlendType(other.BlendType)
    , DiffuseColour(other.DiffuseColour)
    , SpecularColour(other.SpecularColour)
{
}