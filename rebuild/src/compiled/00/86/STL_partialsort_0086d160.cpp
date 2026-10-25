// std::__partial_sort(pair<long, CRandomAppearanceMorph::CSkeletalMorph>* first, pair<long, CSkeletalMorph>* middle, pair<long, CSkeletalMorph>* last, pair<long, CSkeletalMorph>*, CKeyPairCompareLess<long, CSkeletalMorph> comp) -- STLport partial_sort core over the skeletal-morph vector map: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D160, length 171 bytes, `ret 0Ch`.
// Callers: the map's __introsort_loop 0x0086DB30 (this chunk; 0x0086DBFA, the depth-limit fallback) and 0x0086D66C.
// Callees: __make_heap(first, last, comp, T*, int*) @ 0x0086B5C0 (this chunk), __adjust_heap(first, holeIndex, len,
//   T value, comp) @ 0x0086B070 (landed), sort_heap(first, last, comp) @ 0x0086C950 (landed); all __fastcall.
// Label: catalog "std::partial_sort_top_k_CSkeletalMorph" is descriptive; STLport 4.x `__partial_sort` verbatim.
// Element: pair<long, CRandomAppearanceMorph::CSkeletalMorph> (16 bytes; Ego_r.pdb CSkeletalMorph {GroupID, CDefString Name,
//   bool AllowVariation}), signed key compare; the same model as this chunk's 0x0086B3B0 / 0x0086B5C0.
// Shape: as this chunk's 0x006810E0 (voice pairs): /Gr algorithm TU (`__fastcall` template: first in ecx, middle in edx;
//   last, the value-type tag and the empty comparator on the stack), make_heap the inline forwarder (two null tags pushed
//   before the comparator), __pop_heap `__forceinline` with the value read before the `*__result = *__first` store and
//   rebuilt in the argument area for __adjust_heap.
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

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x0086B5C0
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x0086B070
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0086C950

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;
typedef CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphLess;
template void __fastcall std::__partial_sort<TSkeletalMorphPair*, TSkeletalMorphPair, TSkeletalMorphLess>(TSkeletalMorphPair*, TSkeletalMorphPair*, TSkeletalMorphPair*, TSkeletalMorphPair*, TSkeletalMorphLess);