// std::__introsort_loop(ENavigatorType* first, ENavigatorType* last, ENavigatorType*, int depth_limit, less<ENavigatorType> comp) -- STLport introsort recursion over the creature def's navigator types: median-of-three partition, recurse on the right part until ranges reach 16 entries, partial_sort at depth zero
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00672240, length 227 bytes (+ int3 padding), `ret 0Ch`.
// Callers: sort 0x006720B0 (landed; depth 2*lg(n)) and itself (0x006722EF).
// Callees: __partial_sort(first, middle, last, T*, comp) @ 0x00672330 (landed; behind the inline partial_sort), itself.
// Label: catalog "std::IntroSort" is descriptive; STLport spelling __introsort_loop.
// Shape: STLport 4.6 `_algo.c` verbatim (`while (__last - __first > __stl_threshold) { if (__depth_limit == 0) {
//   partial_sort(__first, __last, __last, __comp); return; } --__depth_limit; __cut = __unguarded_partition(__first,
//   __last, _Tp(__median(*__first, *(__first + (__last - __first)/2), *(__last - 1), __comp)), __comp);
//   __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp); __last = __cut; }`) with __median (returning a
//   const reference: the pick is an address, `mov ecx,[eax]`) and __unguarded_partition inline (`__forceinline`: as a
//   plain template VC7.1 calls it out of line, `__cdecl`, `add esp,10h`); the threshold test is
//   the byte form `and eax,-4 ; cmp eax,40h`. /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, the
//   value-type tag, the depth limit and the empty 4-byte comparator on the stack).
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum (signed compares)

namespace std {
template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00672330

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

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    ENavigatorType __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline _RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
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

template void __fastcall std::__introsort_loop<ENavigatorType*, ENavigatorType, int, std::less<ENavigatorType> >(ENavigatorType*, ENavigatorType*, ENavigatorType*, int, std::less<ENavigatorType>);