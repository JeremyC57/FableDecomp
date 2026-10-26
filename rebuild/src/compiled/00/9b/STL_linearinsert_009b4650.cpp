// std::__linear_insert(pair<unsigned long, SValue8>* first, pair<unsigned long, SValue8>* last, pair<unsigned long, SValue8> val, CKeyPairCompareLess<unsigned long, SValue8> comp) -- STLport insertion-sort step over 12-byte map entries: a new minimum shifts the prefix up one slot, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B4650, 91 bytes, `ret 10h`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the 12-byte value and the empty comparator on the stack). Catalog name "StdVector_InsertElement_Sort"
// (descriptive). No static xrefs (reached from an __insertion_sort through a computed call or folded away).
// Callees: __copy_backward<pair*, pair*, int> @ 0x009B18E0 (out of line here, unlike the 8-byte instantiations: its
// 12-byte element division costs too much for /Ob2), __unguarded_linear_insert @ 0x009B1A30 (landed here).
// Element: pair<unsigned long, 8-byte value>; CKeyPairCompareLess compares the keys only (unsigned: jae).
// Shape: STLport 4.6 verbatim; the random_access_iterator_tag temporary for the __copy_backward call is homed in the
// dead comparator argument slot (`lea eax,[esp+1Ch]`), the else arm copy-constructs val into the callee's argument area.
namespace std {
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
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x009B18E0

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B1A30

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

struct SValue8 {                                // 8-byte mapped value (two words)
    long a;                                     // +0x00
    long b;                                     // +0x04
};

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, SValue8> SEntry12;
template void __fastcall std::__linear_insert<SEntry12*, SEntry12, CKeyPairCompareLess<unsigned long, SValue8> >(
    SEntry12* __first, SEntry12* __last, SEntry12 __val, CKeyPairCompareLess<unsigned long, SValue8> __comp);