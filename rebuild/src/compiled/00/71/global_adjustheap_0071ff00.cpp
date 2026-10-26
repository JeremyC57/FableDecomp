// _global::__adjust_heap(pair<EHeroMorphType,CTextureMorphEntry>*, int, int, pair<EHeroMorphType,CTextureMorphEntry>, CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>) -- SGI heap sift-down of the hero texture-morph table by morph type, then __push_heap @ 0x0071F710
// Flags: /Gr
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071FF00, 199 bytes.  Called by the landed SGI
// __pop_heap 0x00720610, whose element model this source reuses.
// Callees: CCompositeBlendDesc copy ctor @ 0x005BA48A, __push_heap @ 0x0071F710 (same instantiation).
// The SGI algorithm TUs are /Gr: first in ecx, holeIndex in edx, then len, the 24-byte pair by
// value and the empty comparator (4-byte slot); `ret 0x20`.
// Element (retail, 24 bytes): pair { EHeroMorphType first; CTextureMorphEntry second = { long
// TextureLayer; CCompositeBlendDesc Blend (16 bytes) } }.  Assignment is the implicit 6-dword copy;
// the by-value copy for __push_heap is built in the argument area by the implicit pair copy ctor
// (two words inline, the blend description through its out-of-line copy ctor).
// Shape: SGI `__adjust_heap` (`secondChild = 2 * (secondChild + 1)`; the bottom case uses
// `secondChild - 1`); the compare is the signed key `jge`.
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
// Same-TU body of the copy constructor (as in the landed 0x00720610): VC7.1 then sees that it only
// reads `other` and writes `this`, so the argument-area build keeps retail's load order (value.first
// and TextureLayer both loaded before `sub esp,18h`).  Without it TextureLayer is loaded late.
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

void __fastcall __push_heap(CMorphPair* first, int holeIndex, int topIndex, CMorphPair value, CMorphLess comp);   // 0x0071F710

void __fastcall __adjust_heap(CMorphPair* first, int holeIndex, int len, CMorphPair value, CMorphLess comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value, comp);
}