// std::__insertion_sort(CWideString* first, CWideString* last, less<CWideString> comp) -- STLport insertion sort of wide strings: each element that sorts before *first shifts the prefix up one slot and lands at the front, the rest take the unguarded insert
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B68D0, 200 bytes, `ret 4`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the empty comparator on the stack; no value-type tag argument). Catalog name "Vector_InsertRange_String" is wrong.
// Callers: the __final_insertion_sort front ends 0x009B749B / 0x009B74B9 / 0x009B848D / 0x009B84AB.
// Callees: CWideString(const CWideString&) @ 0x0099B720, operator< @ 0x0099AF70, operator= @ 0x0099B7D0,
// ~CWideString @ 0x0099B510, __unguarded_linear_insert @ 0x009B1870 (this chunk).
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, _Tp(*i), comp);`
// with __linear_insert inlined here (`__forceinline`: /Ob2 keeps the plain inline out of line, as its copy
// 0x009B58B0): val is the frame temporary, copy_backward of
// a class range is __copy_backward's counted `*--result = *--last` loop of operator= calls (n spilled to the frame), the
// else arm rebuilds val in __unguarded_linear_insert's argument area.
class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator<(const CWideString& other) const;     // 0x0099AF70
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct random_access_iterator_tag {
    random_access_iterator_tag() {}
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B1870

template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, CWideString(*__i), __comp);
}
}

template void __fastcall std::__insertion_sort<CWideString*, std::less<CWideString> >(
    CWideString* __first, CWideString* __last, std::less<CWideString> __comp);