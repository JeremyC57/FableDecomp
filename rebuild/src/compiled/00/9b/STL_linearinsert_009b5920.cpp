// _STL::__linear_insert(pair<unsigned long, unsigned long>* first, pair<unsigned long, unsigned long>* last, pair<unsigned long, unsigned long> val, CKeyPairCompareLess<unsigned long, unsigned long> comp) -- STLport insertion-sort step over 8-byte map entries: a new minimum shifts the prefix up one slot, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5920, 99 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the 8-byte value and the empty comparator on the stack). Catalog name "Insertion_Sort_InsertElement_Pair8byte"
// (descriptive). Caller 0x009B69CB (the __insertion_sort); callee __unguarded_linear_insert @ 0x009B1AA0 (a sibling instantiation).
// Element: pair<unsigned long, unsigned long> (CVectorMap entry); CKeyPairCompareLess compares the keys only (unsigned: jae).
// Shape: STLport 4.6 verbatim. copy_backward of a non-trivial-assign pair (user copy ctor => __false_type) goes through
// __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0) = the counted `for (n = last - first;
// n > 0; --n) *--result = *--last;` loop, inlined; the else arm copy-constructs val into the callee's argument area.
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x00
    _T2 second;                                 // +0x04
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

struct random_access_iterator_tag {
    random_access_iterator_tag() {}
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
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B1AA0

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

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, unsigned long> SEntry8;
template void __fastcall _STL::__linear_insert<SEntry8*, SEntry8, CKeyPairCompareLess<unsigned long, unsigned long> >(
    SEntry8* __first, SEntry8* __last, SEntry8 __val, CKeyPairCompareLess<unsigned long, unsigned long> __comp);