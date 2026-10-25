// CEngineTextureMorphSet::AddTextureMorph(long texture_index, const CCompositeBlendDesc& blend) -- add a composite blend
// to the texture's morph entry, creating the entry (with an opaque white base layer) when missing; a base-type blend (3)
// replaces all earlier blends
// Leaf: ?AddTextureMorph@CEngineTextureMorphSet@@QAEXJABVCCompositeBlendDesc@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA950, length 145 bytes (`ret 8`, void).
// Identity (FINDINGS chunk 387; was the descriptive CTextureMorphs::AddBlend(unsigned long, ...), and CTextureMorphs is a
//   different PDB class -- a CVectorMap<EHeroMorphType, CTextureMorphEntry>): Ego_r.pdb / FableWin.pdb name
//   ?AddTextureMorph@CEngineTextureMorphSet@@QAEXJABVCCompositeBlendDesc@@@Z (long, const CCompositeBlendDesc&), and
//   struct_layouts CEngineTextureMorphSet = {CArray<CEngineTextureMorph> Morphs +0} = the begin/end vector this body
//   searches (20-byte retail records {OriginalTextureIndex, MorphID, vector<CCompositeBlendDesc> Blends}). Its callee
//   0x006CA8C0 (push_back of an empty record, returns &back()) is the protected AddEngineMorph(long) (PDB
//   ?AddEngineMorph@CEngineTextureMorphSet@@IAEPAVCEngineTextureMorph@@J@Z); the linear search is the inline protected
//   FindPEngineMorph(long). Callers are the hero appearance morph updaters (0x0070759D
//   CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw, 0x0071DF79 CTCHeroMorph::UpdateTextureMorphs, 0x00727685 /
//   0x007281F0 CTCRandomAppearanceMorph::UpdateMorphSet).
// Callees: AddEngineMorph @ 0x006CA8C0, vector<CCompositeBlendDesc>::push_back @ 0x006CB010 (x2),
//   vector<CCompositeBlendDesc>::erase(first, last) @ 0x006CAD70 (STLport clear()).
// Layout (Ego_r.pdb): CCompositeBlendDesc {TextureIndex, ECompositeBlendType BlendType, CRGBColour DiffuseColour,
// CRGBColour SpecularColour} (16 bytes); CRGBColour stores B, G, R, A (its constructor takes R, G, B, A).
// Shape: the linear search jumps straight to the shared tail when found; the default layer is a named local built by
// an inline (texture, type) constructor whose colour members are initialised by CRGBColour(r, g, b, a), a constructor
// that assigns R, G, B, A in its body (stores +2, +1, +0, +3; an init list would follow the B, G, R, A layout).
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CCompositeBlendDesc {
public:
    unsigned long TextureIndex;                     // +0x00
    long          BlendType;                        // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                    // +0x08
    CRGBColour    SpecularColour;                   // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& o)
        : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) {}
    CCompositeBlendDesc(unsigned long texture, long type)
        : TextureIndex(texture), BlendType(type), DiffuseColour(255, 255, 255, 255), SpecularColour(0, 0, 0, 0) {}
};

namespace std {
template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                                      // +0x00
    _Tp* _M_finish;                                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;             // +0x08
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    void push_back(const _Tp& __x);                                     // 0x006CB010 for CCompositeBlendDesc
    iterator erase(iterator __first, iterator __last);                  // 0x006CAD70 for CCompositeBlendDesc
    void clear() { erase(begin(), end()); }
};
}

template <class T> class CArray : public std::vector<T> {
};

class CEngineTextureMorph {                         // Ego_r.pdb; 20 bytes in retail
public:
    long                              OriginalTextureIndex;   // +0x00
    long                              MorphID;                // +0x04
    std::vector<CCompositeBlendDesc>  Blends;                 // +0x08
};

class CEngineTextureMorphSet {
public:
    void AddTextureMorph(long texture_index, const CCompositeBlendDesc& blend);
protected:
    CEngineTextureMorph* AddEngineMorph(long texture_index);                     // 0x006CA8C0
    CArray<CEngineTextureMorph> Morphs;                                           // +0x00
};

void CEngineTextureMorphSet::AddTextureMorph(long texture_index, const CCompositeBlendDesc& blend)
{
    CEngineTextureMorph* morph = 0;
    for (CEngineTextureMorph* it = Morphs.begin(); it != Morphs.end(); ++it) {    // FindPEngineMorph
        if (it->OriginalTextureIndex == texture_index) {
            morph = it;
            break;
        }
    }
    if (!morph) {
        morph = AddEngineMorph(texture_index);
        CCompositeBlendDesc base(texture_index, 3);
        morph->Blends.push_back(base);
    }
    if (blend.BlendType == 3)
        morph->Blends.clear();
    morph->Blends.push_back(blend);
}
