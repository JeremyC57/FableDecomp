// CCompositeBlendDesc::CCompositeBlendDesc(ECompositeBlendType, unsigned long, const CRGBColour&, const CRGBColour&) -- a composite-texture blend layer: blend type, texture index and the diffuse/specular colours copied from the references
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CABA0, length 90 bytes. No callees.
// The manifest label "CQuestInfoElementBar::CQuestInfoElementBar" is propagated and rejected: the object has no
// vtable and is 16 bytes -- PDB CCompositeBlendDesc {TextureIndex +0, BlendType +4, DiffuseColour +8,
// SpecularColour +0xc}; the only caller is CTCHeroMorph::AddCompositeTexture (0x0071DC95), which builds the
// descriptor on its frame. The first argument lands at +4 (BlendType), the second at +0 (TextureIndex).
// Shape: each colour is copied with all four bytes loaded before any store (A first, then B -- spilled into the
// dead argument slot [esp+0Ch] -- then G and R): the copy goes through an inline helper taking the four channel
// values (r, g, b, a), evaluated right to left; the channels sit B, G, R, A in memory (D3DCOLOR order).
typedef long ECompositeBlendType;

struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CCompositeBlendDesc {
    unsigned long       TextureIndex;       // +0x00
    ECompositeBlendType BlendType;          // +0x04
    CRGBColour          DiffuseColour;      // +0x08
    CRGBColour          SpecularColour;     // +0x0c
    CCompositeBlendDesc(ECompositeBlendType blendType, unsigned long textureIndex, const CRGBColour& diffuse, const CRGBColour& specular);
};

CCompositeBlendDesc::CCompositeBlendDesc(ECompositeBlendType blendType, unsigned long textureIndex, const CRGBColour& diffuse, const CRGBColour& specular)
    : TextureIndex(textureIndex)
    , BlendType(blendType)
{
    DiffuseColour.Set(diffuse.R, diffuse.G, diffuse.B, diffuse.A);
    SpecularColour.Set(specular.R, specular.G, specular.B, specular.A);
}