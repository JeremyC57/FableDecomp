// std::__partial_sort(pair<unsigned long,CDefString>* first, pair<unsigned long,CDefString>* middle, pair<unsigned long,CDefString>* last, pair<unsigned long,CDefString>*, CKeyPairCompareLess<unsigned long,CDefString> comp) -- STLport heap-select partial sort of the sound map's 8-byte criteria entries (the introsort depth-exhausted fallback): heapify [first, middle), push every smaller later entry through the heap top, then sort the heap
// Instantiation: std::__partial_sort<pair<unsigned long,CDefString>*,pair<unsigned long,CDefString>,CKeyPairCompareLess<unsigned long,CDefString> >
// Leaf: ??$__partial_sort@PAU?$pair@KVCDefString@@@std@@U12@V?$CKeyPairCompareLess@KVCDefString@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047142B, 131 bytes (size-optimised def TU, no padding), __fastcall
//   (first ecx, middle edx; last, the value-type pointer and the 4-byte comparator slot on the stack), `ret 0Ch`.
// Identity: called by the landed __introsort_loop @ 0x00471338 (CSoundMap::CriteriaMap's pair<unsigned long,
//   CDefString>; partial_sort(first, last, last, comp) when the depth budget runs out). The 12-byte CSoundPair twin is
//   0x00470FA5 (same source). The catalog name `std::sort_heapify_8byte` is descriptive.
// Callees: __make_heap @ 0x00471530, __adjust_heap @ 0x004714AE (landed), __pop_heap_aux @ 0x00471576 (landed), all out
//   of line.
// Shape: STLport 4.6 `__partial_sort` with make_heap, __pop_heap (`*result = *first` as movsd x3, then
//   `__adjust_heap(first, 0, middle - first, val, comp)`), sort_heap and pop_heap inlined (`__forceinline` under "s");
//   8-byte distances are `sar 3` / `and ~7` against 8 (no idiv); sort_heap keeps `middle` in the dead `last` argument
//   slot. Frame pointer: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }     // _STLP_VALUE_TYPE for pointers
template <class _Tp> __forceinline int* __distance_type(const _Tp*) { return (int*)0; }  // _STLP_DISTANCE_TYPE for pointers

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x004714AE
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);                                                             // 0x00471530
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00471576

template <class _RandomAccessIterator, class _Compare>
__forceinline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Compare>
__forceinline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIterator, class _Compare>
__forceinline void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
template void __fastcall __partial_sort(_CriteriaEntry*, _CriteriaEntry*, _CriteriaEntry*, _CriteriaEntry*,
                                        CKeyPairCompareLess<unsigned long, CDefString>);
}