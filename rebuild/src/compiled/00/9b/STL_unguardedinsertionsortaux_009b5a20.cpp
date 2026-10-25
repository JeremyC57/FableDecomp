// _STL::__unguarded_insertion_sort_aux(pair<CCharString, long>* first, pair<CCharString, long>* last, pair<CCharString, long>*, CKeyPairCompareLess<CCharString, long> comp) -- unguarded insertion-sort pass over the 8-byte (name, value) entries: insert a copy of every entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5A20, 57 bytes, `ret 8` (link-time __fastcall /Gr algorithm TU, written as
//   explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte slot on the
//   stack).
// Identity: the `__final_insertion_sort` tail of sort<pair<CCharString, long>*, CKeyPairCompareLess<CCharString, long> >:
//   callers the landed sort 0x009B8EF0 and the sibling COMDATs 0x009B69E0 / 0x009B73E0; the landed callee 0x009B47F0
//   names this address as its unguarded-insertion-sort caller.
// Callees: CCharString copy constructor 0x0099EC30, __unguarded_linear_insert 0x009B47F0 (landed; it destroys the
//   by-value pair's name).
// Element: a CVectorMap<CCharString, long>-style entry (the landed 0x009B47F0 model; the second word is not recoverable).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: the by-value pair is
//   built in its outgoing slot (`sub esp,8 ; mov edi,esp`) by STLport pair's memberwise copy constructor.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value (4-byte slot)
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<CCharString, long> SNameEntry;           // 8 bytes
typedef CKeyPairCompareLess<CCharString, long> SNameLess;

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B47F0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<SNameEntry*, SNameEntry, SNameLess>(SNameEntry*, SNameEntry*, SNameEntry*, SNameLess);
}