// CVectorMap<long,CTCHeroMorph::CTextureLayer,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer> >::Add(const long& key, const CTCHeroMorph::CTextureLayer& value) -- append a {key, texture layer} pair to the map's vector (in place, or through the out-of-line growth path) and mark the map dirty (re-sorted on the next lookup)
// Grade: DIFFER(125v125) -- scheduling only: in the inlined placement copy retail loads pair.first and second.TextureID (ecx, edx) before the first store; ours stores [eax] between the two loads. Tried: STLport memberwise pair copy ctor (this), implicit pair copy, /Oa, a CTextureLayer memberwise copy ctor (with/without the pair one), a same-TU auto_inline(off) CCompositeBlendDesc copy-ctor body.
// Leaf: ?Add@CTextureLayerMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721A50, 125 bytes, `ret 8`.
// Label correction: the catalog "Vector_InsertRange_CCompositeBlendDesc" is descriptive; the body is the CVectorMap::Add
//   shape of the landed 0x006FCF40 (`Data.push_back(pair(key, value)); Dirty = true;`) for the CTCHeroMorph texture
//   layer map (Ego_r.pdb CTCHeroMorph::AddCompositeTexture / UpdateTattoos take CVectorMap<long, CTextureLayer,
//   CKeyPairCompareLess<long, CTextureLayer> >).  No direct callers (inlined elsewhere).
// Callees: CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) @ 0x005BA48A (x2: the pair temporary,
//   then the in-place element), vector::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend) @ 0x00721060.
// Layout: CVectorMap = STLport vector (12 bytes retail) + empty Compare (+0xc) + bool Dirty (+0xd); the element is
//   pair<long, CTextureLayer { long TextureID; CCompositeBlendDesc Blend; }> (24 bytes).
// Shape: the pair temporary is built by STLport's `pair(const T1& a, const T2& b)` (key and TextureID words, then the
//   blend copy constructor) in a 24-byte frame; push_back is inlined (placement copy through pair's memberwise copy
//   constructor behind the null test, `add [esi+4],18h`; the growth call's empty tag on the dead key slot); the
//   Dirty store is duplicated into both arms.
inline void* operator new(unsigned int, void* p) { return p; }
struct __false_type { __false_type() {} };

enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
};
struct CCompositeBlendDesc {                       // 16 bytes
    unsigned long       TextureIndex;              // +0x00
    ECompositeBlendType BlendType;                 // +0x04
    CRGBColour          DiffuseColour;             // +0x08
    CRGBColour          SpecularColour;            // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};
struct CTextureLayer {                             // CTCHeroMorph::CTextureLayer (20 bytes)
    long                TextureID;                 // +0x00
    CCompositeBlendDesc Blend;                     // +0x04
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair& o) : first(o.first), second(o.second) {}
};
typedef pair<long, CTextureLayer> TLayerPair;

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

struct CTextureLayerVector {                       // STLport vector<pair<long, CTextureLayer> > (12 bytes)
    TLayerPair* _M_start;                          // +0x00
    TLayerPair* _M_finish;                         // +0x04
    TLayerPair* _M_end_of_storage;                 // +0x08
    void _M_insert_overflow(TLayerPair* pos, const TLayerPair& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00721060
    void push_back(const TLayerPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

struct CTextureLayerMap {                          // CVectorMap<long, CTCHeroMorph::CTextureLayer, CKeyPairCompareLess<..> >
    CTextureLayerVector Data;                      // +0x00
    unsigned char       Compare;                   // +0x0c (empty)
    bool                Dirty;                     // +0x0d
    void Add(const long& key, const CTextureLayer& value);
};

void CTextureLayerMap::Add(const long& key, const CTextureLayer& value)
{
    Data.push_back(TLayerPair(key, value));
    Dirty = true;
}
