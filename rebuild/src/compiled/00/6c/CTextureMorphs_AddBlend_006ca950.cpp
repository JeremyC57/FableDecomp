// CTextureMorphs::AddBlend(unsigned long textureIndex, const CCompositeBlendDesc& blend) -- add a composite blend to the texture's morph entry, creating the entry (with an opaque white base layer) when missing; a base-type blend (3) replaces all earlier blends
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA950, length 145 bytes (`ret 8`, void).
// Label correction: the catalog's "CRegion::IsMapIncluded" is propagated. `this` is a vector<CEngineTextureMorph>
// (begin +0, end +4; 0x14-byte retail records {OriginalTextureIndex, MorphID, vector<CCompositeBlendDesc> Blends});
// the callers are the hero appearance morph updaters (0x0070759D CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw,
// 0x0071DF79 CTCHeroMorph::UpdateTextureMorphs, 0x00727685 / 0x007281F0 CTCRandomAppearanceMorph::UpdateMorphSet),
// which build the CCompositeBlendDesc argument the same way. The method and class names are descriptive.
// Callees: AddMorph(textureIndex) @ 0x006CA8C0 (push_back of an empty entry, returns &back(); catalog
// "Map_InsertEntry"), vector<CCompositeBlendDesc>::push_back @ 0x006CB010 (x2), ::erase(first, last) @ 0x006CAD70.
// Layout (Ego_r.pdb): CCompositeBlendDesc {TextureIndex, ECompositeBlendType BlendType, CRGBColour DiffuseColour,
// CRGBColour SpecularColour} (16 bytes); CRGBColour stores B, G, R, A (its constructor takes R, G, B, A).
// Shape: the linear search jumps straight to the shared tail when found; the default layer is a named local built by
// an inline (texture, type) constructor whose colour members are initialised by CRGBColour(r, g, b, a), a constructor
// that assigns R, G, B, A in its body (stores +2, +1, +0, +3; an init list would follow the B, G, R, A layout).
// Leaf: AddBlend
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CCompositeBlendDesc {
    unsigned long TextureIndex;                     // +0x00
    long          BlendType;                        // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                    // +0x08
    CRGBColour    SpecularColour;                   // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& o)
        : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) {}
    CCompositeBlendDesc(unsigned long texture, long type)
        : TextureIndex(texture), BlendType(type), DiffuseColour(255, 255, 255, 255), SpecularColour(0, 0, 0, 0) {}
};

struct CBlendVector {                               // vector<CCompositeBlendDesc>
    CCompositeBlendDesc* m_first;
    CCompositeBlendDesc* m_last;
    CCompositeBlendDesc* m_end;
    void push_back(const CCompositeBlendDesc& blend);                              // 0x006CB010
    CCompositeBlendDesc* erase(CCompositeBlendDesc* first, CCompositeBlendDesc* last);   // 0x006CAD70
    void clear() { erase(m_first, m_last); }
};

struct CEngineTextureMorph {
    long         OriginalTextureIndex;              // +0x00
    long         MorphID;                           // +0x04
    CBlendVector Blends;                            // +0x08
};

struct CTextureMorphs {                             // vector<CEngineTextureMorph>
    CEngineTextureMorph* m_first;
    CEngineTextureMorph* m_last;
    CEngineTextureMorph* m_end;
    CEngineTextureMorph* AddMorph(unsigned long textureIndex);                    // 0x006CA8C0
    void AddBlend(unsigned long textureIndex, const CCompositeBlendDesc& blend);
};

void CTextureMorphs::AddBlend(unsigned long textureIndex, const CCompositeBlendDesc& blend)
{
    CEngineTextureMorph* morph = 0;
    for (CEngineTextureMorph* it = m_first; it != m_last; ++it) {
        if (it->OriginalTextureIndex == (long)textureIndex) {
            morph = it;
            break;
        }
    }
    if (!morph) {
        morph = AddMorph(textureIndex);
        CCompositeBlendDesc base(textureIndex, 3);
        morph->Blends.push_back(base);
    }
    if (blend.BlendType == 3)
        morph->Blends.clear();
    morph->Blends.push_back(blend);
}