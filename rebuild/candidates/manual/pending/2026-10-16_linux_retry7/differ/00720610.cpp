// std::__pop_heap(pair<EHeroMorphType, CTextureMorphEntry>* first, ..., pair value, CKeyPairCompareLess comp, int*) -- SGI pop_heap core for the hero texture-morph heap: *result = *first, then sift `value` down from the root
// Grade: DIFFER(116v116) -- scheduling only: retail loads the value's second word (`mov ecx,[esp+18h]`) before `sub esp,18h`, ours after it (`mov ecx,[esp+30h]`). Tried: implicit copies, STLport pair memberwise copy ctor, CTextureMorphEntry memberwise copy ctor. | retry 7 (Fable): not retried (register swap / LTCG custom convention / FP chain reassociation / pre-frame load -- known lane gaps, no untried source shape).
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
// Shape: the implicit assignment copies the six words through edx; the by-value copy for __adjust_heap is built in the argument
//   area by the implicit pair copy ctor (two words inline, the blend description through its out-of-line copy ctor); the count is
//   the reciprocal multiply by 0x2AAAAAAB with `sar edx,2` (divide by 24).
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
struct CRGBColour {
    unsigned char B, G, R, A;
};
struct CCompositeBlendDesc {
    unsigned long       TextureIndex;              // +0x00
    ECompositeBlendType BlendType;                 // +0x04
    CRGBColour          DiffuseColour;             // +0x08
    CRGBColour          SpecularColour;            // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};
struct CTextureMorphEntry {
    long                TextureLayer;              // +0x00
    CCompositeBlendDesc Blend;                     // +0x04
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V> struct CKeyPairCompareLess {
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
