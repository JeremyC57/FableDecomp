// _STL::__unguarded_insertion_sort_aux(pair<float, CCharString>* first, pair<float, CCharString>* last, pair<float, CCharString>*, CKeyPairCompareLess<float, CCharString> comp) -- unguarded insertion-sort pass over a subtitled message's (time, animation name) tags: insert a copy of every entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0420, 56 bytes (+8 int3), `ret 8` (link-time __fastcall /Gr algorithm TU,
//   written as explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte
//   slot on the stack).
// Identity: the `__final_insertion_sort` tail of sort<pair<float, CCharString>*, CKeyPairCompareLess<...> > over
//   CSubtitledMessage::AnimationTags (CVectorMap<float, CCharString>): callers sort 0x004D24F0 (landed) and the sibling
//   COMDATs 0x004D0980 / 0x004D1940; the landed callee 0x004CFCF0 names this address as its insertion-sort caller.
// Callees: CCharString copy constructor 0x0099EC30 (building the by-value pair), __unguarded_linear_insert 0x004CFCF0
//   (landed; it destroys the by-value pair).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: the by-value pair is
//   built in its outgoing slot by STLport pair's memberwise copy constructor (the float key as a dword, then the string's
//   copy constructor), the comparator word kept in ebx and pushed first.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace _STL {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<float, CCharString> TTextPair;                           // 8 bytes
typedef CKeyPairCompareLess<float, CCharString> TTextLess;

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x004CFCF0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<TTextPair*, TTextPair, TTextLess>(TTextPair*, TTextPair*, TTextPair*, TTextLess);
}