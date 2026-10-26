// _global::__partial_sort(pair<EHeroMorphType,CTextureMorphEntry>* first, pair<EHeroMorphType,CTextureMorphEntry>* middle, pair<EHeroMorphType,CTextureMorphEntry>* last, pair<EHeroMorphType,CTextureMorphEntry>*, CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry> comp) -- STLport partial_sort core for the hero texture-morph table: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721960, 227 bytes (240 with padding), `ret 0Ch`.
// Label correction: the catalog "TimSort_TextureLayer_24byte" is a descriptive auto label; the body is STLport
//   `__partial_sort(first, middle, last, _Tp*, comp)` (call sites 0x00721D1C / 0x00721F60 of the introsort loops).
// Callees: __make_heap @ 0x00720710, __pop_heap @ 0x00720610 (landed; first in ecx, last in edx, then result, the
//   24-byte value, the comparator and the distance-type pointer), sort_heap @ 0x007214A0.
// Convention: /Gr (link-time __fastcall): first in ecx (spilled to the frame), middle in edx (edi), then last, the
//   value-type tag pointer and the empty comparator on the stack.
// Element: the landed __adjust_heap 0x0071FF00 model, except that the blend description's copy constructor is an
//   INLINE memberwise copy here (two dwords, then each CRGBColour through the inline Set(r, g, b, a): the bytes
//   are stored R, G, B, A after loading all four) -- the by-value `_Tp(*i)` is built in the argument area of the
//   out-of-line __pop_heap.  VC7.1 turns the `i < last` loop into a down-counted trip count
//   ((last - middle - 1) / 24 + 1) with `i` strength-reduced to `&i->second.Blend.DiffuseColour.G`.
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
class CRGBColour { public:                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CCompositeBlendDesc { public:
    unsigned long       TextureIndex;              // +0x00
    ECompositeBlendType BlendType;                 // +0x04
    CRGBColour          DiffuseColour;             // +0x08
    CRGBColour          SpecularColour;            // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other)
        : TextureIndex(other.TextureIndex)
        , BlendType(other.BlendType)
        , DiffuseColour(other.DiffuseColour)
        , SpecularColour(other.SpecularColour)
    {
    }
};
class CTextureMorphEntry { public:
    long                TextureLayer;              // +0x00
    CCompositeBlendDesc Blend;                     // +0x04
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef pair<EHeroMorphType, CTextureMorphEntry> CMorphPair;
typedef CKeyPairCompareLess<EHeroMorphType, CTextureMorphEntry> CMorphLess;

void __fastcall __make_heap(CMorphPair* first, CMorphPair* last, CMorphLess comp, CMorphPair*, int*);   // 0x00720710
void __fastcall __pop_heap(CMorphPair* first, CMorphPair* last, CMorphPair* result, CMorphPair value,
                           CMorphLess comp, int*);   // 0x00720610
void __fastcall sort_heap(CMorphPair* first, CMorphPair* last, CMorphLess comp);   // 0x007214A0

inline void make_heap(CMorphPair* first, CMorphPair* last, CMorphLess comp)
{
    __make_heap(first, last, comp, (CMorphPair*)0, (int*)0);
}

void __fastcall __partial_sort(CMorphPair* first, CMorphPair* middle, CMorphPair* last, CMorphPair*, CMorphLess comp)
{
    make_heap(first, middle, comp);
    for (CMorphPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, CMorphPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}