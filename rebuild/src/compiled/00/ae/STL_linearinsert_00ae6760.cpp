// _STL::__linear_insert(SPair* first, SPair* last, SPair val, SLess comp) -- STLport guarded insertion step over 8-byte pair<long, unsigned long> records: a new minimum shifts [first, last) up one slot and lands at first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE6760, 109 bytes (+3 int3 padding), `ret 0Ch`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, last in edx, the 8-byte val by value and the 4-byte empty comparator slot on the stack.
// Callers: __insertion_sort 0x00AE7110 (landed). Callee: __unguarded_linear_insert 0x00AE5AB0 (val rebuilt in the argument
//   area by the pair's user copy constructor: `sub esp,8 ; mov eax,esp`).
// Element: pair<long, unsigned long> -- the instantiation of the landed __adjust_heap 0x00AE5FB0 (STLport pair operator<).
// Label: catalog "InsertionSort_InsertBefore" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);`); for this element copy_backward is the inline random-access
//   `for (n = last - first; n > 0; --n) *--result = *--last;` loop (the pair's assignment is implicit).
namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

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

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00AE5AB0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
}

typedef _STL::pair<long, unsigned long> SPair;
typedef _STL::less<SPair> SLess;

template void __fastcall _STL::__linear_insert<SPair*, SPair, SLess>(SPair*, SPair*, SPair, SLess);