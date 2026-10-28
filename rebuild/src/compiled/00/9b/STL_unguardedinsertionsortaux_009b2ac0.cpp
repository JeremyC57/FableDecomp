// std::__unguarded_insertion_sort_aux(CWideString* first, CWideString* last, CWideString*, less<CWideString> comp) -- unguarded insertion-sort pass over a wide-string range: sift a copy of every string of [first, last) down past the greater strings, then assign it into the hole
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B2AC0, 111 bytes, `ret 8` (link-time __fastcall /Gr algorithm TU, written as
//   explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty less<> slot on the stack).
// Identity: the `__final_insertion_sort` tail of sort<CWideString*> -- the landed sort 0x009B8440 names this address as
//   its `__unguarded_insertion_sort_aux(first + 16, last, (_Tp*)0, comp)`; the sibling COMDATs 0x009B3580 / 0x009B73E0
//   of the definition manager's path-list sort call it too.
// Callees: CWideString copy constructor 0x0099B720, CWideString::operator< 0x0099AF70 (landed), CWideString::operator=
//   0x0099B7D0, CWideString::~CWideString 0x0099B510.
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` with the linear
//   insert INLINE here (the wide-string instantiation is small enough): the by-value string lands in the `push ecx`
//   frame word, `next` rides in esi as `i - 1` (`lea esi,[ebx-4]`) and `last` in edi, less<> folds to the member
//   operator< on the value, `*last = *next` / `*last = val` are the out-of-line operator=, then the copy dies.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux<CWideString*, CWideString, less<CWideString> >`.
class CWideString {
public:
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator<(const CWideString& other) const;     // 0x0099AF70
private:
    void* PStringData;
};

namespace std {
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<CWideString*, CWideString, less<CWideString> >(CWideString*, CWideString*, CWideString*, less<CWideString>);
}