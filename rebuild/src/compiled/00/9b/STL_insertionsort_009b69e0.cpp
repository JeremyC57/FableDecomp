// std::__insertion_sort(pair<CCharString,unsigned long>* first, pair<CCharString,unsigned long>* last, CKeyPairCompareLess<CCharString,unsigned long> comp) -- STLport insertion sort: __linear_insert each element after the first, passing a copy of it by value
// Flags: /Gr
// Leaf: ??$__insertion_sort@PAU?$pair@VCCharString@@K@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B69E0, 66 bytes (+ int3 padding), `ret 4` (/Gr TU: first in ecx, last in edx,
//   the empty comparator in one stack dword).
// Identity: the sort of CVectorMap<CCharString, unsigned long> (FableWin.pdb names pair<CCharString,unsigned long> /
//   CKeyPairCompareLess<CCharString,unsigned long> for the heap helpers); called from __final_insertion_sort 0x009B7520 and
//   the inlined one in sort 0x009B8EF0 (CDefinitionManager::CheckDependencies' file-checksum map).  Catalogued as
//   `Swap_CCharString_Pair`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the by-value element built in the argument area,
//   `second` copied after it), __linear_insert(first, i, value, comp) @ 0x009B5990 (__fastcall, `ret 0Ch`).
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`.
class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __p) : first(__p.first), second(__p.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B5990

template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template void __insertion_sort(pair<CCharString, unsigned long>*, pair<CCharString, unsigned long>*,
                               CKeyPairCompareLess<CCharString, unsigned long>);
}