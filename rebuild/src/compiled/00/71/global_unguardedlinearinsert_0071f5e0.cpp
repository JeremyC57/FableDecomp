// _global::__unguarded_linear_insert(pair<EHeroMorphType,CTextureMorphEntry>* last, pair<EHeroMorphType,CTextureMorphEntry> val, CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry> comp) -- STLport insertion-sort step over the hero texture-morph table: shift every larger morph type up one slot, drop `val` into the hole
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F5E0, 106 bytes (112 with padding), `ret 1Ch`.  No callees.
// Label correction: the catalog "RBTree_Insert_ShiftUp" is a descriptive auto label; the body is STLport
//   `__unguarded_linear_insert(last, val, comp)` of the texture-morph sort (callers 0x0071FD75 / 0x00720D1E are
//   the __linear_insert / __unguarded_insertion_sort_aux of the same instantiation).
// Convention: /Gr (link-time __fastcall): `last` in ecx, the 24-byte pair and the empty comparator on the stack
//   (0x18 + 4 = 0x1C).
// Element: the landed __adjust_heap 0x0071FF00 / __push_heap 0x0071F710 model: pair { EHeroMorphType first;
//   CTextureMorphEntry { long TextureLayer; CCompositeBlendDesc Blend (16 bytes) } }; the signed key compare
//   (`cmp edx,[next] ; jl`, the key loaded once into edx); assignment is the trivial 6-dword copy through edi,
//   and the final drop stores the six value words from the argument slots.
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

void __fastcall __unguarded_linear_insert(CMorphPair* last, CMorphPair val, CMorphLess comp)
{
    CMorphPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}