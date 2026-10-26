// Behaviour test for 00720610: src/00720610.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. std::__pop_heap(pair<EHeroMorphType, CTextureMorphEntry>* first, ..., pair value, CKeyPairCompareLess comp, int*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----
static CMorphPair* g_first; static int g_hole, g_len, g_calls = 0; static unsigned char g_value[24];
void __fastcall __adjust_heap(CMorphPair* first, int holeIndex, int len, CMorphPair value, CMorphLess comp)
{ ++g_calls; g_first = first; g_hole = holeIndex; g_len = len; memcpy(g_value, &value, sizeof value); }
static void Fill(CMorphPair& p, int k)
{
    p.first = (EHeroMorphType)(k); p.second.TextureLayer = 100 + k; p.second.Blend.TextureIndex = 200 + k;
    p.second.Blend.BlendType = (ECompositeBlendType)(300 + k);
    p.second.Blend.DiffuseColour.B = (unsigned char)(1 + k); p.second.Blend.DiffuseColour.G = (unsigned char)(2 + k);
    p.second.Blend.DiffuseColour.R = (unsigned char)(3 + k); p.second.Blend.DiffuseColour.A = (unsigned char)(4 + k);
    p.second.Blend.SpecularColour.B = (unsigned char)(5 + k); p.second.Blend.SpecularColour.G = (unsigned char)(6 + k);
    p.second.Blend.SpecularColour.R = (unsigned char)(7 + k); p.second.Blend.SpecularColour.A = (unsigned char)(8 + k);
}
int main()
{
    CHECK(sizeof(CMorphPair) == 24);
    static unsigned char heapStore[5 * 24], resultStore[24], valueStore[24];
    CMorphPair* heap = (CMorphPair*)heapStore; CMorphPair* result = (CMorphPair*)resultStore; CMorphPair& value = *(CMorphPair*)valueStore;
    for (int i = 0; i < 5; ++i) Fill(heap[i], 10 * i);
    Fill(value, 7);
    CMorphLess comp;
    __pop_heap(heap, heap + 5, result, value, comp, 0);
    CHECK(memcmp(result, &heap[0], 24) == 0);
    CHECK(g_calls == 1); CHECK(g_first == heap); CHECK(g_hole == 0); CHECK(g_len == 5);
    CHECK(memcmp(g_value, &value, 24) == 0);
    __pop_heap(heap + 1, heap + 4, result, value, comp, 0);
    CHECK(memcmp(result, &heap[1], 24) == 0); CHECK(g_len == 3); CHECK(g_first == heap + 1); CHECK(g_calls == 2);
    if (fails == 0) printf("00720610_TEST PASS\n"); else printf("00720610_TEST FAIL\n");
    return fails;
}