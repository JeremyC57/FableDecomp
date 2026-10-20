// CWound::UpdateDecal(CTCTextureDecal&, long&, long, long, float) -- keep one wound/scar decal in step with its strength: delete it once the strength falls to 0.05, else add it (first time) or change it with a white diffuse colour whose alpha is the strength
// Leaf: UpdateDecal
// Grade: DIFFER(ours v retail) -- 179v179, scheduling: retail calls _ftol2 for the alpha before the TextureIndex/BlendType/
// colour stores (alpha kept in al, stores through ecx); ours stores first, then converts (stores through eax), and the
// AddDecal argument loads rotate eax/ecx/edx
// Retail: Fable.exe @ 0x00757440, 179 bytes, `ret 14h`. PDB ?UpdateDecal@CWound@CTCWound@@IAEXAAVCTCTextureDecal@@AAJJJM@Z
// (CTCWound::CWound, protected).
// Callees: CTCTextureDecal::DeleteDecal(long) 0x0070A430, CTCTextureDecal::AddDecal(long, CCompositeBlendDesc&)
// 0x0070A490 (manifest "CRenderTarget_EnqueueDecalUpdate"), CTCTextureDecal::ChangeDecal(long, CCompositeBlendDesc&)
// 0x0070A3E0, _ftol2 0x00BFEA70. Float pool 0x0125A15C = 0.05f, 0x01230014 = 255.0f.
// CCompositeBlendDesc (16 bytes, PDB): TextureIndex +0, BlendType +4, DiffuseColour +8, SpecularColour +0xC;
// CRGBColour (4 bytes, PDB): B +0, G +1, R +2, A +3.
// Shape: `this` is unused; the alpha is a named local converted before the desc is filled; the blend desc is
// filled member by member and the colours through an inline Set(r, g, b, a)
// storing R, G, B, A (the retail +2, +1, +0, +3 byte order); the
// entry id is read once and kept in esi for both tests.
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_2 = 2 };
struct CCompositeBlendDesc {
    unsigned long       TextureIndex;                        // +0x00
    ECompositeBlendType BlendType;                           // +0x04
    CRGBColour          DiffuseColour;                       // +0x08
    CRGBColour          SpecularColour;                      // +0x0C
};
struct CTCTextureDecal {
    void DeleteDecal(long id);                                           // 0x0070A430
    long AddDecal(long decal, CCompositeBlendDesc& desc);                // 0x0070A490
    void ChangeDecal(long id, CCompositeBlendDesc& desc);                // 0x0070A3E0
};

struct CTCWound {
    struct CWound {
        void UpdateDecal(CTCTextureDecal& decals, long& entryID, long decal, long textureIndex, float strength);
    };
};

void CTCWound::CWound::UpdateDecal(CTCTextureDecal& decals, long& entryID, long decal, long textureIndex, float strength)
{
    long id = entryID;
    if (id && strength <= 0.05f)
    {
        decals.DeleteDecal(id);
        entryID = 0;
        return;
    }
    unsigned char alpha = (unsigned char)(strength * 255.0f);
    CCompositeBlendDesc desc;
    desc.TextureIndex = textureIndex;
    desc.BlendType = COMPOSITE_BLEND_TYPE_2;
    desc.DiffuseColour.Set(255, 255, 255, alpha);
    desc.SpecularColour.Set(0, 0, 0, 0);
    if (!id)
        entryID = decals.AddDecal(decal, desc);
    else
        decals.ChangeDecal(id, desc);
}
