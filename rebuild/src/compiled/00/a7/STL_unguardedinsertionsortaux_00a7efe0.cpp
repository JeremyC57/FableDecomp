// std::__unguarded_insertion_sort_aux(SUniqueIDOpen* first, SUniqueIDOpen* last, SUniqueIDOpen*, CUniqueIDOpenLess comp) -- unguarded insertion sort of the 16-byte (unsigned __int64 unique ID, bool open) entries: insert each entry of [first, last) into the sorted run before it
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7EFE0, 58 bytes, `ret 8` (/Gr: first in ecx, last in edx; the null value-type
//   pointer and the empty comparator's slot on the stack).
// Callers: the final insertion sorts 0x00A7FC37, 0x00A832CC, 0x00A841B9 (CNavSwitchableLeafNode's UniqueIDsOpen map,
//   CVectorMap<unsigned __int64, bool>). Callee: __unguarded_linear_insert 0x00A7D570 (this chunk, `ret 14h`).
// Label: catalog "_Copy_impl<CComplexNumber*,CComplexNumber*>" is propagated; the body is STLport _algo.c
//   `__unguarded_insertion_sort_aux`.
// Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` -- the comparator slot is kept
//   in ebx and pushed first; the value is pair's explicit copy ctor built in the argument area (key as two dwords, the
//   bool as one byte).
namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

typedef std::pair<unsigned __int64, bool>          SUniqueIDOpen;      // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, bool> CUniqueIDOpenLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A7D570

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux(SUniqueIDOpen*, SUniqueIDOpen*, SUniqueIDOpen*, CUniqueIDOpenLess);
}