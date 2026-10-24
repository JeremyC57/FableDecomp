// std::__unguarded_partition(long* first, long* last, long pivot, less<long> comp) -- STLport quicksort partition step over 4-byte signed integers: advance from the left past smaller values, retreat from the right past larger ones, swap, until the cursors cross; returns the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00776D40, 68 bytes, /Gr `__fastcall` `ret 8` (first in ecx -> eax, last in
//   edx; the pivot value and the empty comparator on the stack).
// Caller: __introsort_loop<long*> 0x00778490 (this chunk) -- the sort<long*, less<long> > family whose landed members are
//   __push_heap 0x00776750 and sort_heap 0x00777EE0 (both compare as SIGNED integers: `jge`/`jl`).
// Label correction: catalogued as `Quicksort_Partition_IntElem`; the body is STLport `__unguarded_partition`.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pointer test is unsigned (`jae`), the
//   swap goes through two registers.
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __a);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
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

template long* __fastcall __unguarded_partition<long*, long, less<long> >(long*, long*, long, less<long>);

}