// std::make_heap(ENavigatorType* first, ENavigatorType* last, less<ENavigatorType> comp) -- STLport heap build over an ENavigatorType range: sift every parent down from (len - 2) / 2 to 0
// Leaf: ??$make_heap@PAW4ENavigatorType@@
//
// Retail 0x00672410, 77 bytes (+ int3 padding to 80), `ret 4` (/Gr: first in ecx, last in edx, the empty 4-byte
//   comparator on the stack).
// Callee: 0x00672390 __adjust_heap(first, holeIndex, len, value, comp) (__fastcall, `ret 0Ch`).
// Caller: 0x00672330 __partial_sort (the vector<ENavigatorType> sort of CThingCreatureDef::Transfer 0x00671600).
// Shape: STLport _heap.c -- make_heap forwards to `__make_heap(first, last, comp, _Tp*, _Distance*)`, inlined here:
//   `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
//   parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` (signed halving = cdq/sub/sar).
//   The retail TU is /Gr; the convention is written as an explicit __fastcall.
// Label: catalog "Vector_MakeHeap" (descriptive); STLport spelling make_heap.
enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& x, const _Tp& y) const { return x < y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value,
                              _Compare comp);                                 // 0x00672390

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
__forceinline void __make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*)
{
    if (last - first < 2) return;
    _Distance len = last - first;
    _Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp)
{
    __make_heap(first, last, comp, __value_type(first), __distance_type(first));
}
}

template void __fastcall std::make_heap<ENavigatorType*, std::less<ENavigatorType> >(
    ENavigatorType*, ENavigatorType*, std::less<ENavigatorType>);