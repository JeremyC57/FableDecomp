// std::__partial_sort(CWideString* first, CWideString* middle, CWideString* last, CWideString*, less<CWideString> comp) -- STLport partial sort: heapify [first, middle), pop every smaller element of [middle, last) into the heap, then sort the heap
// Flags: /Gr
// Leaf: ??$__partial_sort@PAVCWideString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B6A40, 130 bytes (+ int3 padding), `ret 0Ch` (/Gr TU: first in ecx,
//   middle in edx; last, the unused T* and the empty comparator on the stack).
// Identity: called from the CWideString introsort loop (depth exhausted, 0x009B7DBD) and from partial_sort 0x009B757C;
//   catalogued as `Heapsort_Top1Element_CWideString`.
// Callees: __make_heap(first, middle, comp, T*, int*) @ 0x009B3BB0, CWideString::operator< @ 0x0099AF70 (less<> inlined:
//   `*i < *first`), CWideString(const CWideString&) @ 0x0099B720 (x2), CWideString::operator= @ 0x0099B7D0,
//   __adjust_heap(first, 0, middle - first, value, comp) @ 0x009B2EA0, ~CWideString @ 0x0099B510,
//   sort_heap(first, middle, comp) @ 0x009B5A80.
// Shape: STLport _algo.c `make_heap; for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first, middle, i,
//   T(*i), comp, DISTANCE_TYPE); sort_heap;` with make_heap and __pop_heap inlined (the T(*i) value is the frame local,
//   its by-value copy for __adjust_heap is built in the argument area).
class CWideString {
public:
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator<(const CWideString& other) const;     // 0x0099AF70
private:
    void* PStringData;
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x009B3BB0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CWideString*)0, (int*)0);
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x009B2EA0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x009B5A80

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __partial_sort(CWideString*, CWideString*, CWideString*, CWideString*, less<CWideString>);
}