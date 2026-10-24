// _STL::__unguarded_linear_insert(pair<unsigned long, SValue8>* last, pair<unsigned long, SValue8> val, CKeyPairCompareLess<unsigned long, SValue8> comp) -- STLport insertion-sort inner step over 12-byte map entries: shift larger entries up one slot until val's key fits, then store val
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B1A30, 67 bytes, `ret 10h`: `__fastcall` (the STLport algorithm TUs are /Gr:
// `last` in ecx, the 12-byte value and the 4-byte empty comparator on the stack). Catalog name "Insertion_Sort_Shift"
// (descriptive); called by the __linear_insert @ 0x009B4650 and 0x009B56D0.
// Element: a 12-byte CVectorMap entry pair<unsigned long, 8-byte value> (value type not recoverable from the body:
// modelled as two longs); CKeyPairCompareLess compares the keys only, unsigned.
// Shape: STLport 4.6 verbatim: `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; }
// *last = val;` -- the key is held in edx, the three-word copy runs through esi/edi, the next key is pre-loaded.
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
}

struct SValue8 {                                // 8-byte mapped value (two words)
    long a;                                     // +0x00
    long b;                                     // +0x04
};

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, SValue8> SEntry12;
template void __fastcall _STL::__unguarded_linear_insert<SEntry12*, SEntry12, CKeyPairCompareLess<unsigned long, SValue8> >(
    SEntry12* __last, SEntry12 __val, CKeyPairCompareLess<unsigned long, SValue8> __comp);