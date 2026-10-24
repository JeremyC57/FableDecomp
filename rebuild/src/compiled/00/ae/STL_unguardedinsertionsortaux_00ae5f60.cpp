// _STL::__unguarded_insertion_sort_aux(SPair* first, SPair* last, SPair*, SLess comp) -- STLport final insertion pass over 8-byte pair<long, unsigned long> records: every element in [first, last) goes through the unguarded linear insert (the first 16 are already sorted, so no lower guard)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE5F60, 67 bytes (+13 int3 padding), `ret 8`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, last in edx, the value-type pointer tag and the 4-byte empty comparator slot on the stack.
// Element: pair<long, unsigned long> -- the instantiation of the landed __adjust_heap 0x00AE5FB0 (STLport pair operator<).
// Label: catalog "InsertionSort_64Bit" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`) with
//   the out-of-line __unguarded_linear_insert (0x00AE5AB0) inlined here: val's two words in edi/ebp, the hole in esi.
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

typedef _STL::pair<long, unsigned long> SPair;
typedef _STL::less<SPair> SLess;

template void __fastcall _STL::__unguarded_insertion_sort_aux<SPair*, SPair, SLess>(SPair*, SPair*, SPair*, SLess);