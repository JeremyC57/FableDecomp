// _STL::__unguarded_insertion_sort_aux(pair<long, CTCHeroMorph::CTextureLayer>* first, pair<long, CTCHeroMorph::CTextureLayer>* last, pair<long, CTCHeroMorph::CTextureLayer>*, CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> comp) -- unguarded insertion sort of the hero's texture-layer map entries: each element, copied by value, goes through the unguarded linear insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071FCF0, 156 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the comparator on the stack, `ret 8`).
// Callers: one, the landed __final_insertion_sort 0x007218E0 (`first + 16 .. last` arm), itself called from the presort
//   0x00721FE0 that the landed CTCHeroMorph::UpdateTextureMorphs 0x0071DE80 runs over its composite texture layers.
// Callees: __unguarded_linear_insert(last, val, comp) 0x0071F5E0.
// Element: Ego_r.pdb pair<long, CTCHeroMorph::CTextureLayer> (24 bytes), the entry of CTCHeroMorph's
//   CVectorMap<long, CTCHeroMorph::CTextureLayer, CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> >:
//   first +0x0, CTextureLayer {long TextureID +0x4; CCompositeBlendDesc Blend +0x8 {ulong TextureIndex, ECompositeBlendType
//   BlendType, CRGBColour DiffuseColour +0x10, CRGBColour SpecularColour +0x14}}. CRGBColour's user copy constructor
//   (`Set(o.R, o.G, o.B, o.A)`, as the landed CRGBColour models) makes the copy memberwise: four dword moves, then each
//   colour byte by byte in R, G, B, A store order.
// Label: catalog "vector<std::pair<unsigned long ...>::_Destroy" is propagated; the body is STLport's sort helper.
// Shape: STLport 4.6 `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the
//   by-value element is copy-constructed in its outgoing argument slot; the comparator's operator() stays
//   declaration-only.
class CRGBColour {                              // 4 bytes, D3DCOLOR byte order; user copy constructor
public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

enum ECompositeBlendType { COMPOSITE_BLEND_NONE };

class CCompositeBlendDesc {                     // Ego_r.pdb, 16 bytes
public:
    unsigned long       TextureIndex;           // +0x0
    ECompositeBlendType BlendType;              // +0x4
    CRGBColour          DiffuseColour;          // +0x8
    CRGBColour          SpecularColour;         // +0xC
};

class CTCHeroMorph {
public:
    class CTextureLayer {                       // Ego_r.pdb, 20 bytes
    public:
        long                TextureID;          // +0x0
        CCompositeBlendDesc Blend;              // +0x4
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const;
};

typedef _STL::pair<long, CTCHeroMorph::CTextureLayer> CTextureLayerEntry;           // 24 bytes
typedef CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> CTextureLayerLess;

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0071F5E0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(CTextureLayerEntry*, CTextureLayerEntry*, CTextureLayerEntry*, CTextureLayerLess);
}