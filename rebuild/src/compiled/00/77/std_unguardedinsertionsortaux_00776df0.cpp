// std::__unguarded_insertion_sort_aux(long* first, long* last, long*, less<long> comp) -- STLport unguarded insertion pass over 4-byte signed integers: sink every element of [first, last) leftwards past larger values (the caller guarantees a smaller sentinel in front)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00776DF0, 50 bytes, /Gr `__fastcall` `ret 8` (first in ecx, last in edx;
//   the value-type tag pointer and the empty comparator on the stack).
// Callers: sort<long*, less<long> > 0x00778540 and __final_insertion_sort 0x00777E90 (the `first + 16 .. last` part),
//   0x007771E0 -- the sort family of the landed sort_heap 0x00777EE0 (signed integer compares).
// Label correction: catalogued as `_Copy_impl<CComplexNumber*,CComplexNumber*>` (propagated); the `ret 8` + tag pointer
//   + the `while (comp(val, *next))` inner loop name STLport `__unguarded_insertion_sort_aux`.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the linear insert
//   inline: `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`.
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<long*, long, less<long> >(long*, long*, long*, less<long>);

}