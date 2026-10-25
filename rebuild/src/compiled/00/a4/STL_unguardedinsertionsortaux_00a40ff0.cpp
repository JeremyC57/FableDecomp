// std::__unguarded_insertion_sort_aux(pair<pair<CCharString, CCharString>, CSoundBankLH*>* first, pair<pair<CCharString, CCharString>, CSoundBankLH*>* last, pair<pair<CCharString, CCharString>, CSoundBankLH*>*, CKeyPairCompareLess<pair<CCharString, CCharString>, CSoundBankLH*> comp) -- unguarded insertion-sort pass over the sound manager's 12-byte ((name, name), bank) entries: insert a copy of every entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A40FF0, 69 bytes, `ret 8` (link-time __fastcall /Gr algorithm TU, written as
//   explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte slot on the
//   stack).
// Identity: the `__final_insertion_sort` tail of the sort over CSoundManagerLH's NamePairMap entries (callers: the landed
//   __final_insertion_sort 0x00A418F0 and the sibling COMDAT 0x00A415D0); same instantiation as the landed
//   __unguarded_linear_insert 0x00A40820 and __linear_insert 0x00A40F50.
// Callees: CCharString copy constructor 0x0099EC30 (twice: the key pair's two names), __unguarded_linear_insert
//   0x00A40820 (landed; it destroys the by-value entry's strings).
// Element: pair<pair<CCharString, CCharString>, CSoundBankLH*> (the landed 0x00A40820 model; CSoundBankLH descriptive).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: the 12-byte by-value
//   entry is built in its outgoing slot (`sub esp,0Ch ; mov edi,esp`): the inner pair's memberwise copy (two string
//   copy constructors, the second at `lea ecx,[edi+4]`), then the bank pointer.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

typedef std::pair<CCharString, CCharString>      SNamePair;      // the (name, name) key
typedef std::pair<SNamePair, CSoundBankLH*>       SNamePairEntry; // NamePairMap entry, 12 bytes
typedef CKeyPairCompareLess<SNamePair, CSoundBankLH*> CNamePairLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A40820

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<SNamePairEntry*, SNamePairEntry, CNamePairLess>(SNamePairEntry*, SNamePairEntry*, SNamePairEntry*, CNamePairLess);
}