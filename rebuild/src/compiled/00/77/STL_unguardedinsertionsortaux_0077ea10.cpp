// std::__unguarded_insertion_sort_aux(pair<CCharString, C3DVector>* first, pair<CCharString, C3DVector>* last, pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector> comp) -- unguarded insertion-sort pass over the 16-byte (name, position) entries: insert a copy of every entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077EA10, 67 bytes (+13 int3), `ret 8` (link-time __fastcall /Gr algorithm TU,
//   written as explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte
//   slot on the stack).
// Identity: the `__final_insertion_sort` tail of sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString,
//   C3DVector> > (callers: the landed sort 0x0077F440 and the sibling COMDATs 0x0077ECB0 / 0x0077EF30); the same
//   instantiation as the landed __unguarded_linear_insert 0x0077E6A0, __adjust_heap 0x0077EA60 and _Sort_heap 0x0077F190.
// Callees: CCharString copy constructor 0x0099EC30, __unguarded_linear_insert 0x0077E6A0 (landed; it destroys the
//   by-value pair's string).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: the 16-byte by-value
//   pair is built in its outgoing slot (`sub esp,10h ; mov edi,esp`) by STLport pair's memberwise copy constructor -- the
//   name's copy constructor, then the position's memberwise copy constructor as three dword moves through edi (an implicit
//   struct copy forms the source address with `lea` and re-bases edi).
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

class C3DVector {                                          // 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // memberwise: no `lea` of the source
};

namespace std {
template <class _T1, class _T2> struct pair {             // STLport pair: user constructors, implicit operator=
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, C3DVector> TNamedPosition;                    // 16 bytes
typedef CKeyPairCompareLess<CCharString, C3DVector> TNamedPositionLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0077E6A0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<TNamedPosition*, TNamedPosition, TNamedPositionLess>(TNamedPosition*, TNamedPosition*, TNamedPosition*, TNamedPositionLess);
}