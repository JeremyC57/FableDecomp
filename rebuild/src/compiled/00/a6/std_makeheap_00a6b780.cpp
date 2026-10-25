// std::__make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*) -- (std::pair<unsigned long, unsigned long>, std::less<std::pair<unsigned long, unsigned long> >) STLport heapify: sift every parent down from (len - 2) / 2 to the root through the out-of-line __adjust_heap
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B780, 74 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx, last
//   in edx, then the 4-byte empty comparator slot and the value-type / distance-type tag pointers on the stack).
// Identity: std::pair<unsigned long, unsigned long> under std::less<std::pair<unsigned long, unsigned long> > -- proven; evidence: its callers are the landed __partial_sort 0x00A6BBB0 and __linear_insert 0x00A6B820 (default less<pair>: unsigned `ja/jb` on first, `jae` on second in __adjust_heap 0x00A6B4F0); FableWin.pdb names make_heap<pair<unsigned long, unsigned long>*> (the default comparator).
// Label: the manifest's "HeapSort_MakeHeap_u64Pair" is propagated across byte-identical bodies (the eight rows differ only in the
//   __adjust_heap call displacement); this is STLport 4.6 `__make_heap` over an 8-byte pair.
// Callee: std::__adjust_heap(first, holeIndex, len, _Tp value, comp) @ 0x00A6B4F0 (__fastcall: first in ecx, holeIndex in
//   edx; len, the by-value 8-byte pair and the comparator on the stack).
// Body: STLport 4.6 _heap.c verbatim: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2;
//   for (;;) { __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`
//   -- the element is rebuilt in the argument area by pair's user copy constructor (`sub esp,8 ; mov eax,esp`).

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                          // +0
    _T2 second;                                         // +4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>                   // 0x00A6B4F0
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
typedef std::pair<unsigned long, unsigned long> _Elem;
template void __fastcall __make_heap(_Elem*, _Elem*, std::less<std::pair<unsigned long, unsigned long> >, _Elem*, int*);
}