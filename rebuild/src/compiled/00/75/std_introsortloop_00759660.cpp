// std::__introsort_loop(pair<EBodyLocation,CWoundMorphs::CEntry>* first, pair<EBodyLocation,CWoundMorphs::CEntry>* last, pair<EBodyLocation,CWoundMorphs::CEntry>*, int depth_limit, CKeyPairCompareLess<EBodyLocation,CWoundMorphs::CEntry> comp) -- STLport introsort recursion over CWoundMorphs' body-location map by key
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759660, 255 bytes, __fastcall (first ecx, last edx), `ret 0Ch`.
// Label correction: catalog "Sort_HeapsortPhase" -> STLport 4.x __introsort_loop over the
//   CVectorMap<EBodyLocation, CWoundMorphs::CEntry> vector (called by the landed sort 0x007597C0 and recursively; the
//   landed __unguarded_partition 0x00758B70 is its partition; 44-byte element: 0x2E8BA2E9 / `sar edx,3`).
// Element: pair<EBodyLocation, CWoundMorphs::CEntry (40-byte POD)> with STLport pair's user copy constructor (the key,
//   then a 10-dword `rep movsd` block).
// Callees: __unguarded_partition 0x00758B70 (pivot by value), __partial_sort 0x00759420, itself.
// Shape: as the 0x006FD1E0 loop; the pivot's block copy claims esi/edi/ecx, so depth_limit is re-read from its
//   argument slot each pass and the middle element's address is computed with `imul 2Ch`.
enum EBodyLocation { EBodyLocation_FIRST };

class CWoundMorphs {
public:
    class CEntry {                              // Ego_r.pdb, 40 bytes
    public:
        EBodyLocation Location;                 // +0x00
        long  ReplacingTextureIndex;            // +0x04
        long  ReplacingBumpMapIndex;            // +0x08
        long  WoundTextureIndex;                // +0x0c
        long  WoundBumpMapIndex;                // +0x10
        long  ScarTextureIndex;                 // +0x14
        long  ScarBumpMapIndex;                 // +0x18
        long  FadeToScarDuration;               // +0x1c
        long  TotalDuration;                    // +0x20
        float PermanentScarStrength;            // +0x24
    };
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> SWoundPair;

template<class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EBodyLocation, CWoundMorphs::CEntry> SWoundLess;

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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00758B70

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00759420

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (SWoundPair*)0, __comp);
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

template void __fastcall __introsort_loop<SWoundPair*, SWoundPair, int, SWoundLess>(SWoundPair*, SWoundPair*, SWoundPair*, int, SWoundLess);

}