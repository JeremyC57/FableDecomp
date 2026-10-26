// std::__pop_heap(pair<EHeroMorphType, CTextureMorphEntry>* first, ..., pair value, CKeyPairCompareLess comp, int*) -- SGI pop_heap core for the hero texture-morph heap: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00720610, 116 bytes (`ret 24h`: result, the 24-byte value, the empty comparator and the
//   distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr `__fastcall`).
// Label correction: the catalog name CObjectAugmentationParticleSet::CObjectAugmentationParticleSet (a copy ctor) is propagated;
//   the callee 0x0071FF00 carries the manifest name `_Adjust_heap<pair<EHeroMorphType,CTextureMorphEntry>*,int,...,
//   CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>>`, so this is the matching __pop_heap (the same family as the landed
//   pop-heap rows of chunk 139, e.g. 0x00A462A0).
// Callees: CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) @ 0x005BA48A (label "CCompositeBlendDesc"),
//   __adjust_heap @ 0x0071FF00.
// Element (retail, 24 bytes): pair { EHeroMorphType first; CTextureMorphEntry second = { long TextureLayer; CCompositeBlendDesc
//   Blend (16 bytes: TextureIndex, BlendType, two CRGBColour) } }.
// Shape: the implicit assignment copies the six words through edx; the CCompositeBlendDesc copy constructor's body is in the
//   TU (the landed 0x005BA48A text: CRGBColour copies through the inline four-channel Set) -- without it VC7.1 loads
//   TextureLayer after the first argument-area store; the by-value copy for __adjust_heap is built in the argument
//   area by the implicit pair copy ctor (two words inline, the blend description through its out-of-line copy ctor); the count is
//   the reciprocal multiply by 0x2AAAAAAB with `sar edx,2` (divide by 24).
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
class CRGBColour { public:                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CCompositeBlendDesc { public:
    unsigned long       TextureIndex;              // +0x00
    ECompositeBlendType BlendType;                 // +0x04
    CRGBColour          DiffuseColour;             // +0x08
    CRGBColour          SpecularColour;            // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};
#pragma auto_inline(off)
// Same-TU body of the copy constructor (inline in the header, emitted out of line; the linker kept the COMDAT copy at
// 0x005BA48A): VC7.1 sees that it only reads `other` and writes `this`, so the caller's argument-area build keeps retail's
// load order (value.first and TextureLayer both loaded before `sub esp,18h`).
__declspec(noinline) CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
    : TextureIndex(other.TextureIndex)
    , BlendType(other.BlendType)
    , DiffuseColour(other.DiffuseColour)
    , SpecularColour(other.SpecularColour)
{
}
#pragma auto_inline(on)

class CTextureMorphEntry { public:
    long                TextureLayer;              // +0x00
    CCompositeBlendDesc Blend;                     // +0x04
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<EHeroMorphType, CTextureMorphEntry> CMorphPair;
typedef CKeyPairCompareLess<EHeroMorphType, CTextureMorphEntry> CMorphLess;

void __fastcall __adjust_heap(CMorphPair* first, int holeIndex, int len, CMorphPair value, CMorphLess comp);   // 0x0071FF00

void __fastcall __pop_heap(CMorphPair* first, CMorphPair* last, CMorphPair* result, CMorphPair value, CMorphLess comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), value, comp);
}