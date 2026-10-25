// std::__adjust_heap<std::pair<long,unsigned long>*,int,std::pair<long,unsigned long>,std::less<std::pair<long,unsigned long> > >(first, holeIndex, len, value, comp) -- STLport heap sift-down of (long, unsigned long) pairs under the lexicographic pair operator<, then __push_heap @ 0x00AE5AF0
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@JK@std@@HU12@U?$less@U?$pair@JK@std@@@2@@std@@YIXPAU?$pair@JK@0@HHU10@U?$less@U?$pair@JK@std@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE5FB0, 122 bytes, __fastcall (first ecx, holeIndex edx); len, the pair
//   by value (built in its slot by the pair's user copy constructor: `sub esp,8 ; mov eax,esp`) and the empty
//   less<> (4-byte slot) follow (`ret 10h`).
// Identity: STLport `__adjust_heap` -- the library is STLport, not Dinkumware: the pair passed by value is built by a
//   USER copy constructor, which STLport's pair declares (`pair(const pair<_T1,_T2>& __o)`) and VC7.1 Dinkumware's does
//   not; its callers are the landed STLport `_STL::__partial_sort` 0x00AE7760, sort_heap 0x00AE71A0 and the make_heap
//   0x00AE62F0 of the same instantiation. Renamed from _global::_Adjust_heap (Dinkumware name and comments).
// Callee: 0x00AE5AF0 __push_heap (same instantiation; catalog `std::_Push_heap`).
// Shape: the child test is the pair operator< `x.first < y.first || (!(y.first < x.first) && x.second < y.second)`:
//   signed `cmp left,right ; jg dec ; jl skip` on the long first, then an unsigned `jae` on the second -- the element is
//   pair<long, unsigned long> exactly.
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

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00AE5AF0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

typedef pair<long, unsigned long> _LongULongPair;
template void __adjust_heap<_LongULongPair*, int, _LongULongPair, less<_LongULongPair> >(_LongULongPair*, int, int, _LongULongPair, less<_LongULongPair>);

}
