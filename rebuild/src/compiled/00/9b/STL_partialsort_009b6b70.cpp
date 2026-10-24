// _STL::__partial_sort(pair<unsigned long,long>* first, pair<unsigned long,long>* middle, pair<unsigned long,long>* last, pair<unsigned long,long>*, CKeyPairCompareLess<unsigned long,long> comp) -- STLport partial sort of (key, value) pairs by unsigned key: heapify [first, middle), pop every smaller element of [middle, last) into the heap, then sort the heap
// Flags: /Gr
// Leaf: ??$__partial_sort@PAU?$pair@KJ@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B6B70, 107 bytes (+ int3 padding), `ret 0Ch` (/Gr TU: first in ecx,
//   middle in edx; last, the unused T* and the empty comparator on the stack).
// Identity: reached from the introsort loop 0x009B7ED0 (depth exhausted, 0x009B7F88) and partial_sort 0x009B75BC; that
//   loop belongs to sort 0x009B8520, whose callers include CCRCSymbolMap (Ego_r.pdb SLONGMap = CVectorMap<unsigned long,long,
//   CKeyPairCompareLess<unsigned long,long>>) and CDefinitionManager::LoadBinaryDefinitions (NameCRCToIndexMap, the same
//   CVectorMap<unsigned long,long>).  The element type is a
//   best-evidence pick: the bytes only fix an 8-byte {unsigned key, dword} pair.  Catalogued as `PartialSort_Heap_Pair8byte`.
// Callees: __make_heap @ 0x009B3C70, __adjust_heap @ 0x009B2FF0 (landed), sort_heap @ 0x009B5BB0 (landed).
// Shape: as 0x009B6A40 (CWideString): make_heap and __pop_heap inlined; the T(*i) value stays in edx/ecx and is stored
//   into the argument area for __adjust_heap after `*i = *first`.
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

namespace _STL {
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

template void __partial_sort(pair<unsigned long, long>*, pair<unsigned long, long>*, pair<unsigned long, long>*,
                             pair<unsigned long, long>*, CKeyPairCompareLess<unsigned long, long>);
}