// _STL::__unguarded_partition(SPair* first, SPair* last, SPair pivot, SLess comp) -- STLport quicksort partition step over 8-byte pair<long, unsigned long> records: walk first up past records below the pivot, last down past records above it, swap, repeat until the cursors cross
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE5F00, 87 bytes (+9 int3 padding), `ret 0Ch`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, last in edx, the 8-byte pivot by value and the 4-byte empty comparator slot on the stack; the crossing cursor
//   comes back in eax.
// Element: pair<long, unsigned long> -- the instantiation of the landed __adjust_heap 0x00AE5FB0 (STLport pair operator<:
//   signed `jg/jl` on the first word, unsigned compare on the second).
// Label: catalog "QuickSort_Partition64Bit_Swap" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last))
//   --last; if (!(first < last)) return first; iter_swap(first, last); ++first; }`); iter_swap is the inline
//   `__iter_swap(a, b, value_type)` -- `T tmp = *a; *a = *b; *b = tmp;`.
namespace _STL {
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

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
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
}

typedef _STL::pair<long, unsigned long> SPair;
typedef _STL::less<SPair> SLess;

template SPair* __fastcall _STL::__unguarded_partition<SPair*, SPair, SLess>(SPair*, SPair*, SPair, SLess);