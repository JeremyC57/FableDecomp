// std::__insertion_sort(long* first, long* last, less<long> comp) -- STLport insertion sort over 4-byte signed integers: an element smaller than the front shifts the whole prefix up one slot with memmove, any other sinks leftwards past larger values
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00777920, 115 bytes, /Gr `__fastcall` `ret 4` (first in ecx -> ebx, last in
//   edx, spilled to the `push ecx` frame word; the empty comparator on the stack).
// Callee: memmove @ 0x00BFEAE6 (MSVCR71; STLport's trivial copy_backward).
// Callers: sort<long*, less<long> > 0x00778540 and __final_insertion_sort 0x00777E90 (the first 16 elements, or all of a
//   short range) -- the sort<long*, less<long> > family of the landed sort_heap 0x00777EE0 (SIGNED compares).
// Label correction: catalogued as `_Insertion_sort1<ENavigatorType*,ENavigatorType>` (propagated); the memmove path
//   proves a scalar element (an enum would take STLport's __false_type element loop) and the body is STLport
//   `__insertion_sort`.
// Shape: STLport `for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);` with __linear_insert inline:
//   `if (comp(val, *first)) { copy_backward(first, i, i + 1); *first = val; } else __unguarded_linear_insert(i, val,
//   comp);` -- copy_backward of a scalar is `__copy_trivial_backward` (`n = (char*)last - (char*)first; n > 0 ?
//   memmove(result - n, first, n) : result`).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

inline long* copy_backward(long* __first, long* __last, long* __result)
{
    return (long*)__copy_trivial_backward(__first, __last, __result);
}

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
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template void __fastcall __insertion_sort<long*, less<long> >(long*, long*, less<long>);

}