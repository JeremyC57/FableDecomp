// std::__introsort_loop(pair<EHeroMorphType,CParticleMorphs::CEntry>* first, pair<EHeroMorphType,CParticleMorphs::CEntry>* last, pair<EHeroMorphType,CParticleMorphs::CEntry>*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort recursion of a hero-morph map's (key, entry) pairs: median-of-three partition until 16 pairs remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081DC80, 272 bytes, `ret 0Ch` (/Gr algorithm TU: first in ecx, last in edx; the
//   value-type tag pointer, the depth limit and the 4-byte comparator slot on the stack).
// Identity: called by sort 0x0081E090 (landed) and recursively by itself; the sorted map is CParticleMorphs::Morphs
//   (Ego_r.pdb CVectorMap<EHeroMorphType, CParticleMorphs::CEntry, CKeyPairCompareLess<..> >; FableWin.pdb names its Sort). The
//   catalog's "std_introsort_9ElementQuickSort" is descriptive.
// Callees: __unguarded_partition(first, last, pivot, comp) 0x0081B800 (pivot by value, built in its argument slot by
//   the pair copy constructor), __partial_sort(first, last, last, (_Tp*)0, comp) 0x0081D4D0, itself.
// Layout: pair<EHeroMorphType, CParticleMorphs::CEntry> = 40 bytes; signed key compare (`jge`/`jl`: an enum key).
// Shape: STLport 4.6 verbatim (as the landed 0x009B7ED0 family): `while (last - first > __stl_threshold)`,
//   `if (depth_limit == 0) { partial_sort(first, last, last, comp); return; }`, `--depth_limit` written back to its
//   argument slot, __median inline returning a reference, the pivot copied by STLport's memberwise pair copy,
//   `last = cut`.
enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum EMeshLocationPoint { MESH_LOCATION_POINT_NONE };
class CDefString {                                   // 4 bytes, an index into the def string table
public:
    long TablePos;
};

class CParticleMorphs {
public:
    class CEntry {                                   // Ego_r.pdb, 36 bytes, trivially copyable
    public:
        EHeroMorphType     MorphType;                // +0x00
        bool               Blending;                 // +0x04
        EMeshLocationPoint Location;                 // +0x08
        CDefString         LocationName;             // +0x0c
        long               LocationIndex;            // +0x10
        long               ParticleIndex;            // +0x14
        long               SecondParticleIndex;      // +0x18
        float              StartAtMorphStrength;     // +0x1c
        float              EndAtMorphStrength;       // +0x20
    };
};

namespace std {
template <class _T1, class _T2> struct pair {           // STLport 4.6 pair (a struct: mangled U)
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (passed in a 4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<EHeroMorphType, CParticleMorphs::CEntry> TMapEntry;
typedef CKeyPairCompareLess<EHeroMorphType, CParticleMorphs::CEntry> TMapCompare;

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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x0081B800
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x0081D4D0
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (TMapEntry*)0, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
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
template void __fastcall __introsort_loop<TMapEntry*, TMapEntry, int, TMapCompare>(TMapEntry*, TMapEntry*,
                                                                                       TMapEntry*, int, TMapCompare);
}