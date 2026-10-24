// _global::__push_heap(pair<EHeroMorphType,CTextureMorphEntry>*, int, int, pair<EHeroMorphType,CTextureMorphEntry>, CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>) -- SGI heap sift-up of the hero texture-morph table by morph type
// Leaf: __push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F710, 145 bytes, `ret 0x20`.  No callees.
// Called by the landed __adjust_heap @ 0x0071FF00 (same instantiation; its element model is reused
// here, without the same-TU copy-constructor body it needs for the argument-area build).
// Element (retail, 24 bytes): pair { EHeroMorphType first; CTextureMorphEntry second = { long
// TextureLayer; CCompositeBlendDesc Blend (16 bytes) } }.  The key is a signed enum
// (`cmp [parent],ebx ; jge`, value.first loaded once into ebx); every move is the implicit 6-dword
// assignment through ebp, and the final drop stores the six value words.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 24-byte pair by
// value and the empty comparator (4 + 0x18 + 4 = 0x20).  Byte twin of 0x00819B10.
// Shape: SGI `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(*(first + parent), value))`).
namespace N0071f710 {
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

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<EHeroMorphType, CTextureMorphEntry> TEntry;

template<class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EHeroMorphType, CTextureMorphEntry> TCompare;

void __fastcall __push_heap(TEntry* first, int hole, int top, TEntry value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}