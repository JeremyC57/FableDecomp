// _STL::__unguarded_insertion_sort_aux(pair<unsigned long, SValue8>* first, pair<unsigned long, SValue8>* last, pair<unsigned long, SValue8>*, CKeyPairCompareLess<unsigned long, SValue8> comp) -- STLport final unguarded insertion sort over 12-byte map entries
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B2CB0, 103 bytes, `ret 8`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the value-type tag pointer and the empty comparator on the stack). Catalog name "_Copy_impl<CNavigationPosition*,...>"
// is a propagated Dinkumware label and wrong. Callers: 0x009B3887, 0x009B6865 (__final_insertion_sort front ends).
// Element: pair<unsigned long, 8-byte value> (CVectorMap entry; the 12-byte __unguarded_linear_insert is landed at 0x009B1A30).
// Shape: STLport 4.6 verbatim; with the inner insert expanded, the value's third word and `last` spill to the frame
// (sub esp,10h) because only ebx/edi stay free for the key and the second word.
namespace _STL {
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

struct SValue8 {                                // 8-byte mapped value (two words)
    long a;                                     // +0x00
    long b;                                     // +0x04
};

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, SValue8> SEntry12;
template void __fastcall _STL::__unguarded_insertion_sort_aux<SEntry12*, SEntry12, CKeyPairCompareLess<unsigned long, SValue8> >(
    SEntry12* __first, SEntry12* __last, SEntry12*, CKeyPairCompareLess<unsigned long, SValue8> __comp);