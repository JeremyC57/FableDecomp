// std::__introsort_loop(pair<long,CTCHeroMorph::CTextureLayer>* first, pair<long,CTCHeroMorph::CTextureLayer>* last, pair<long,CTCHeroMorph::CTextureLayer>*, int depth_limit, CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer> comp) -- STLport introsort recursion over the hero-morph texture-layer map by key
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00721E20, 335 bytes, __fastcall (first ecx, last edx), `ret 0Ch`.
// Label correction: catalog "MorphData_TimSort" -> STLport 4.x __introsort_loop over the CTCHeroMorph TU's
//   CVectorMap<long, CTCHeroMorph::CTextureLayer> vector (the landed __unguarded_partition 0x00720C00 names this
//   caller; 24-byte element: 0x2AAAAAAB / `sar edx,2`).
// Element: pair<long, CTCHeroMorph::CTextureLayer {long TextureID; CCompositeBlendDesc Blend;}>; the blend descriptor's
//   inline copy constructor (out-of-line twin 0x005BA48A) copies two dwords and the two CRGBColour channels through
//   their four-argument Set (all four bytes loaded before any store, spilled to the dead argument slots).
// Callees: __unguarded_partition 0x00720C00 (pivot by value), __partial_sort 0x00721960, itself.
// Shape: as the 0x006FD1E0 loop: `while (last - first > 16)`, the depth-limit partial_sort fallback, `--depth_limit` in
//   its own slot, __median inlined over the signed keys, the pivot copy-constructed in the argument area, `last = cut`.
class CRGBColour {                              // D3DCOLOR byte order
public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
class CCompositeBlendDesc {                     // Ego_r.pdb, 16 bytes
public:
    unsigned long       TextureIndex;           // +0x00
    ECompositeBlendType BlendType;              // +0x04
    CRGBColour          DiffuseColour;          // +0x08
    CRGBColour          SpecularColour;         // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other)
        : TextureIndex(other.TextureIndex), BlendType(other.BlendType),
          DiffuseColour(other.DiffuseColour), SpecularColour(other.SpecularColour) {}
};
class CTCHeroMorph {
public:
    class CTextureLayer {                       // Ego_r.pdb, 20 bytes
    public:
        long                TextureID;          // +0x00
        CCompositeBlendDesc Blend;              // +0x04
    };
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef std::pair<long, CTCHeroMorph::CTextureLayer> SLayerPair;

template<class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> SLayerLess;

namespace std {

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00720C00

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00721960

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (SLayerPair*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop<SLayerPair*, SLayerPair, int, SLayerLess>(SLayerPair*, SLayerPair*, SLayerPair*, int, SLayerLess);

}