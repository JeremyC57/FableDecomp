// std::__unguarded_insertion_sort_aux(CCharString* first, CCharString* last, CCharString*, less<CCharString> comp) -- unguarded insertion-sort pass over a CCharString range: insert a copy of every string of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E0F60, 46 bytes, `ret 8` (link-time __fastcall /Gr algorithm TU, written as
//   explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte slot on the
//   stack).
// Identity: the `__final_insertion_sort` tail of sort<CCharString*> (instantiation 0x005E3A50; same TU as the landed
//   __unguarded_linear_insert 0x005E0710, __linear_insert 0x005E1A40 and __unguarded_insertion_sort 0x005E19A0); xref:
//   0 calls / 0 references (an emitted COMDAT with no surviving caller).
// Callees: CCharString copy constructor 0x0099EC30 (the by-value string built in its outgoing slot),
//   __unguarded_linear_insert 0x005E0710 (landed; it destroys the by-value string).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: `push ebx` (the
//   comparator word), `push ecx ; mov ecx,esp` opens the string's slot, `push esi ; call` copy-constructs it.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    bool operator<(const CCharString& other) const;
};

namespace std {
template <class _Tp> struct less {                         // STLport less<T>: empty, passed by value
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x005E0710

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<CCharString*, CCharString, less<CCharString> >(CCharString*, CCharString*, CCharString*, less<CCharString>);
}