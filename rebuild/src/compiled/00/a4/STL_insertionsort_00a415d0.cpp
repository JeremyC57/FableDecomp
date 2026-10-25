// _STL::__insertion_sort(pair<pair<CCharString, CCharString>, long>* first, pair<pair<CCharString, CCharString>, long>* last, CKeyPairCompareLess<pair<CCharString, CCharString>, long> comp) -- insertion sort of [first, last): every element after the first, copied by value, goes through the guarded linear insert
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A415D0, 78 bytes, __fastcall (first ecx, last edx; the empty comparator on the stack,
//   `ret 4`).
// Callers: the sort of the 12-byte CVectorMap<pair<CCharString, CCharString>, long> entries (the map whose landed Add is
//   0x00A41A70 and whose grow path's __uninitialized_fill_n is 0x00A3FC00) -- its __final_insertion_sort's short-range arm.
// Callees: __linear_insert(first, last, val, comp) 0x00A40F50, CCharString copy constructor 0x0099EC30.
// Element: STLport pair<pair<CCharString, CCharString>, long> (12 bytes; key strings +0x0 / +0x4, value +0x8), copied
//   memberwise into the by-value argument slot (two CCharString copies, then the long).
// Label: catalog "vector<std::pair<long ...>::_Destroy" is propagated; the body is STLport's __insertion_sort.
// Shape: STLport 4.6 `if (__first == __last) return; for (__i = __first + 1; __i != __last; ++__i)
//   __linear_insert(__first, __i, *__i, __comp);` with the element handed BY VALUE to the out-of-line linear insert
//   (as the landed __insertion_sort 0x00AD5B50); the comparator's operator() stays declaration-only.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const;
};

typedef _STL::pair<CCharString, CCharString> CStringPairKey;
typedef _STL::pair<CStringPairKey, long> CStringPairEntry;                      // 12 bytes
typedef CKeyPairCompareLess<CStringPairKey, long> CStringPairEntryLess;

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A40F50

template <class _Tp, class _Compare>
void __fastcall __insertion_sort(_Tp* __first, _Tp* __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_Tp* __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template void __fastcall __insertion_sort(CStringPairEntry*, CStringPairEntry*, CStringPairEntryLess);
}