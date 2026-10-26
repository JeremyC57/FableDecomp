// std::__unguarded_linear_insert(pair<unsigned long, long>* last, pair<unsigned long, long> val, CKeyPairCompareLess<unsigned long, long> comp) -- STLport insertion-sort inner step: shift larger entries up one slot until val's key fits, then store val
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B1B00, 51 bytes (+ int3 padding), `ret 0Ch`: `__fastcall` (the STLport
// algorithm TUs are /Gr: `last` in ecx, the 8-byte value and the 4-byte empty comparator on the stack). Catalog name
// "SortedArray_InsertElement_Uint" (descriptive); called only by the matching __linear_insert @ 0x009B46B0.
// Element: an 8-byte CVectorMap entry pair<unsigned long, 4-byte value> (the CDefinitionManager module's
// CVectorMap<unsigned long, long> instantiations sort through here); CKeyPairCompareLess compares the keys only, unsigned.
// Shape: STLport 4.6 verbatim: `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; }
// *last = val;` -- the key is held in edx, the next key is pre-loaded at the bottom of the loop.
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
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, long> SEntry8;
template void __fastcall std::__unguarded_linear_insert<SEntry8*, SEntry8, CKeyPairCompareLess<unsigned long, long> >(
    SEntry8* __last, SEntry8 __val, CKeyPairCompareLess<unsigned long, long> __comp);