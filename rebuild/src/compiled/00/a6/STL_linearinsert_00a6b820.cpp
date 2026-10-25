// std::__linear_insert(pair<unsigned long,unsigned long>* first, pair<unsigned long,unsigned long>* last, pair<unsigned long,unsigned long> val, less<pair<unsigned long,unsigned long> > comp) -- insertion-sort step: a value below *first shifts [first, last) up one slot and goes first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B820, 109 bytes, `ret 0Ch` (link-time __fastcall: first in ecx, last
//   in edx, the 8-byte pair value and the empty comparator on the stack).
// Callee: 0x00A6B1A0 std::__unguarded_linear_insert (same instantiation, out of line).
// Caller: 0x00A6B97B, the landed __insertion_sort 0x00A6B950 (`__linear_insert(first, i, T(*i), comp)`).
// Identity: the manifest's "InsertionSort_InsertElement_u64Pair" is descriptive. Element = std::pair<unsigned long,
//   unsigned long> under the default less<pair> (unsigned `ja/jb` on first, `jae` on second), as in the catalogued
//   _Push_heap 0x00A6B1E0 / _Adjust_heap 0x00A6B4F0 of the same TU.
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward is fully inlined down to SGI __copy_backward with
//   the random-access tag (`for (n = last - first; n > 0; --n) *--result = *--last;`, a counted esi loop); the else arm
//   rebuilds val in the argument area (`sub esp,8 ; mov eax,esp`: STLport's user pair copy constructor).
namespace std {

class __false_type { public: __false_type() { } };
class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> class less {
public:
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A6B1A0

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

typedef pair<unsigned long, unsigned long> TPair;
template void __fastcall __linear_insert<TPair*, TPair, less<TPair> >(TPair*, TPair*, TPair, less<TPair>);

}