// _STL::__introsort_loop(pair<long, CRandomAppearanceMorph::CSkeletalMorph>* first, pair<long, CSkeletalMorph>* last, pair<long, CSkeletalMorph>*, int depth_limit, CKeyPairCompareLess<long, CSkeletalMorph> comp) -- STLport introsort recursion over the skeletal-morph vector map: median-of-three partition, recurse on the right part until ranges reach 16 entries, partial_sort at depth zero
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086DB30, length 214 bytes (+ int3 padding), `ret 0Ch`.
// Callers: itself (0x0086DBCF) and the map's sort 0x0086DD80 (landed; 0x0086DDB8).
// Callees: __unguarded_partition(first, last, T pivot, comp) @ 0x0086B3B0 and __partial_sort(first, middle, last, T*,
//   comp) @ 0x0086D160 (both this chunk; the latter behind the inline partial_sort), itself.
// Label: catalog "std::introsort_4way_partition" is descriptive; STLport spelling __introsort_loop.
// Element: pair<long, CRandomAppearanceMorph::CSkeletalMorph> (16 bytes; Ego_r.pdb CSkeletalMorph {GroupID, CDefString Name,
//   bool AllowVariation}), signed key compare; the same model as this chunk's 0x0086B3B0 / 0x0086D160.
// Shape: STLport 4.6 `_algo.c` verbatim, as this chunk's 0x00681270 (voice pairs): __median inline (a const reference: the
//   pick is an address whose pair is copied into the argument area), the threshold test in bytes (`and eax,-10h ; cmp
//   eax,100h`); /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, the value-type tag, the depth limit
//   and the empty comparator on the stack).
class CDefString {                                          // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                          // +0x00
};
class CRandomAppearanceMorph {
public:
    class CSkeletalMorph {                                  // Ego_r.pdb, 12 bytes (implicit, trivial copy)
    public:
        long       GroupID;                                 // +0x00
        CDefString Name;                                    // +0x04
        bool       AllowVariation;                          // +0x08
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;
typedef CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphLess;

namespace _STL {
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x0086B3B0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x0086D160

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

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

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
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
}


template void __fastcall _STL::__introsort_loop<TSkeletalMorphPair*, TSkeletalMorphPair, int, TSkeletalMorphLess>(TSkeletalMorphPair*, TSkeletalMorphPair*, TSkeletalMorphPair*, int, TSkeletalMorphLess);