// _global::__unguarded_partition(pair<EHeroMorphType,CTextureMorphEntry>* first, pair<EHeroMorphType,CTextureMorphEntry>* last, pair<EHeroMorphType,CTextureMorphEntry> pivot, CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry> comp) -- STLport quicksort partition of the hero texture-morph table around a by-value pivot key; returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00720C00, 76 bytes, `ret 1Ch`.
// Label correction: the catalog "Partition_CTextureLayer_24byte" is a descriptive auto label; the body is STLport
//   `__unguarded_partition(first, last, pivot, comp)` (caller 0x00721F12, the __introsort_loop of the same
//   instantiation).
// Callee: iter_swap / swap of two entries @ 0x0071FDF0 (__fastcall a in ecx, b in edx; its catalog label is
//   swap<CTCHeroMorph::CTextureLayer>).
// Convention: /Gr (link-time __fastcall): first in ecx (edi), last in edx (esi), the 24-byte pivot and the empty
//   comparator on the stack (0x18 + 4 = 0x1C).  The pivot key is loaded once into ebx.
// Element: the landed __adjust_heap 0x0071FF00 model (pair<EHeroMorphType, CTextureMorphEntry>, signed key).
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
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
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef pair<EHeroMorphType, CTextureMorphEntry> CMorphPair;
typedef CKeyPairCompareLess<EHeroMorphType, CTextureMorphEntry> CMorphLess;

void __fastcall iter_swap(CMorphPair* a, CMorphPair* b);   // 0x0071FDF0

CMorphPair* __fastcall __unguarded_partition(CMorphPair* first, CMorphPair* last, CMorphPair pivot, CMorphLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}