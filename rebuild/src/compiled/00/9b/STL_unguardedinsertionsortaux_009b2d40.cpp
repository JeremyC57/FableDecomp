// std::__unguarded_insertion_sort_aux(pair<unsigned long, long>* first, pair<unsigned long, long>* last, pair<unsigned long, long>*, CKeyPairCompareLess<unsigned long, long> comp) -- STLport final unguarded insertion sort over 8-byte map entries (every element is linearly inserted without a lower bound check)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B2D40, 73 bytes, `ret 8`: `__fastcall` (/Gr algorithm TU: first in ecx, last
// in edx, the value-type tag pointer and the empty comparator on the stack). Catalog name "_Copy_impl<CNavigationPosition*,...>"
// is a propagated Dinkumware label and wrong: the body is the SGI/STLport insertion sort tail used by __final_insertion_sort.
// Callers: 0x009B38F7, 0x009B74FC, 0x009B8579 (the __final_insertion_sort front ends). 0x009B2D40 and 0x009B2DB0 are byte-identical instantiations for two 8-byte entry types.
// Element: pair<unsigned long, long> (a CVectorMap entry; CKeyPairCompareLess compares the keys only, unsigned).
// Shape: STLport 4.6 verbatim: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with
// the inner insert (landed out of line elsewhere, e.g. 0x009B1B00) expanded here: the value's two words ride in edi/ebx.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x00
    _T2 second;                                 // +0x04
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
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

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, long> SEntry8;
template void __fastcall std::__unguarded_insertion_sort_aux<SEntry8*, SEntry8, CKeyPairCompareLess<unsigned long, long> >(
    SEntry8* __first, SEntry8* __last, SEntry8*, CKeyPairCompareLess<unsigned long, long> __comp);