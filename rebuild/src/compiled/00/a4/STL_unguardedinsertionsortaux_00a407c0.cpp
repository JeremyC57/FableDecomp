// std::__unguarded_insertion_sort_aux(pair<CCharString, CSoundBankLH*>* first, pair<CCharString, CSoundBankLH*>* last, pair<CCharString, CSoundBankLH*>*, CKeyPairCompareLess<CCharString, CSoundBankLH*> comp) -- unguarded insertion-sort pass over the sound manager's 8-byte (name, bank) entries: insert a copy of every entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A407C0, 57 bytes, `ret 8` (link-time __fastcall /Gr algorithm TU, written as
//   explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte slot on the
//   stack).
// Identity: the `__final_insertion_sort` tail of the sort over CSoundManagerLH's NameMap entries (callers: the landed
//   sort 0x00A41E20 and the sibling COMDATs 0x00A40EF0 / 0x00A41480); same instantiation as the landed
//   __unguarded_linear_insert 0x00A40370 and __linear_insert 0x00A40730. Byte twin of 0x009B5A20 (another callee).
// Callees: CCharString copy constructor 0x0099EC30, __unguarded_linear_insert 0x00A40370 (landed; it destroys the
//   by-value pair's name).
// Element: pair<CCharString, CSoundBankLH*> (the landed 0x00A40370 model; CSoundBankLH is descriptive).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: the by-value pair is
//   built in its outgoing slot (`sub esp,8 ; mov edi,esp`) by STLport pair's memberwise copy constructor.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

class CSoundBankLH;                                         // descriptive: the mapped object of CSoundManagerLH's maps

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, CSoundBankLH*> SNameEntry;  // NameMap entry, 8 bytes
typedef CKeyPairCompareLess<CCharString, CSoundBankLH*> CNameLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A40370

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<SNameEntry*, SNameEntry, CNameLess>(SNameEntry*, SNameEntry*, SNameEntry*, CNameLess);
}