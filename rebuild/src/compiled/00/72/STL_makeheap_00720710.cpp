// std::__make_heap(pair<long, CTCHeroMorph::CTextureLayer>* first, pair<long, CTCHeroMorph::CTextureLayer>* last, CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> comp, pair<long, CTCHeroMorph::CTextureLayer>*, int*) -- SGI make_heap of the hero texture-layer table by key: sift every parent down from (len-2)/2 to the root with __adjust_heap, each fed a by-value copy of the parent
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00720710, 184 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx, the
// empty comparator's slot, the value-type pointer and the Distance* on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: std::__adjust_heap(first, holeIndex, len, T value, comp) @ 0x0071FF00 (landed; same instantiation -- its source
// names the pair `pair<EHeroMorphType, CTextureMorphEntry>`, the same 24-byte layout).
// Types (Ego_r.pdb): CTCHeroMorph::CTextureLayer (20 bytes) = long TextureID +0, CCompositeBlendDesc Blend +4;
// CCompositeBlendDesc (16) = ulong TextureIndex, ECompositeBlendType BlendType, CRGBColour DiffuseColour, SpecularColour;
// CRGBColour = B, G, R, A bytes. The pair is 24 bytes, key at +0.
// Shape: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
// parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the count divides by 24 (0x2AAAAAAB,
// `sar edx,2`). Here the pair's implicit copy runs CCompositeBlendDesc's implicit copy constructor INLINE (four words,
// then each CRGBColour through the landed four-channel Set: R, G, B, A stores from the B/G/R/A loads), built straight
// into the argument area from a strength-reduced `first + parent*24 + 11h` pointer (the 0x00720610 pop_heap of the same
// TU calls the out-of-line 0x005BA48A instead -- that call site copies a by-value argument).
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };

class CRGBColour { public:                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

class CCompositeBlendDesc { public:                       // 16 bytes (implicit memberwise copy)
    unsigned long       TextureIndex;              // +0x00
    ECompositeBlendType BlendType;                 // +0x04
    CRGBColour          DiffuseColour;             // +0x08
    CRGBColour          SpecularColour;            // +0x0c
};

struct CTextureLayer {                             // CTCHeroMorph::CTextureLayer, 20 bytes
    long                TextureID;                 // +0x00
    CCompositeBlendDesc Blend;                     // +0x04
};

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};

template <class K, class V>
class CKeyPairCompareLess { public:                       // empty functor, passed in a 4-byte slot
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};

typedef pair<long, CTextureLayer> CTextureLayerPair;
typedef CKeyPairCompareLess<long, CTextureLayer> CTextureLayerLess;

void __fastcall __adjust_heap(CTextureLayerPair* first, int holeIndex, int len, CTextureLayerPair value, CTextureLayerLess comp);   // 0x0071FF00

void __fastcall __make_heap(CTextureLayerPair* first, CTextureLayerPair* last, CTextureLayerLess comp, CTextureLayerPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, CTextureLayerPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}