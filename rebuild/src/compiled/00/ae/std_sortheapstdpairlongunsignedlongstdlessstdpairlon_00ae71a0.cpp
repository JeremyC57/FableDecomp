// std::sort_heap<std::pair<long,unsigned long>*,std::less<std::pair<long,unsigned long> > >(first, last, comp) -- STLport sort_heap: pop_heap until one element remains (the last pair set aside, the front copied to the back, __adjust_heap @ 0x00AE5FB0 from hole 0)
// Leaf: ??$sort_heap@PAU?$pair@JK@std@@U?$less@U?$pair@JK@std@@@2@@std@@YIXPAU?$pair@JK@0@0U?$less@U?$pair@JK@std@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE71A0, 96 bytes, __fastcall (first ecx, last edx), the empty less<> in a
//   4-byte stack slot (`ret 4`).
// Identity: the sort_heap step of the landed STLport `std::__partial_sort` 0x00AE7760 (its only caller, which declares
//   `sort_heap(_RandomAccessIterator, _RandomAccessIterator, _Compare)` for this instantiation); STLport's
//   `while (__last - __first > 1) pop_heap(__first, __last--, __comp);` with pop_heap -> __pop_heap_aux -> __pop_heap
//   inlined. Renamed from the stand-in _global::SortHeap_00ae71a0 (Dinkumware comments).
// Shape: `__pop_heap(first, last - 1, last - 1, _Tp(*(last - 1)), comp, (int*)0)`: the last pair is copied into the
//   reserved argument slot by the pair's user copy constructor (`sub esp,8 ; mov eax,esp`, STLport's pair declares one),
//   the front is copied to the back, then __adjust_heap(first, 0, (last - 1) - first, value, comp). The byte-count
//   form (`and -8 ; cmp 8`) is VC's lowering of `last - first > 1`.
namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00AE5FB0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

typedef pair<long, unsigned long> _LongULongPair;
template void __fastcall sort_heap<_LongULongPair*, less<_LongULongPair> >(_LongULongPair*, _LongULongPair*, less<_LongULongPair>);

}
