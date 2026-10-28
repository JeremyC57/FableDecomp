// std::__unguarded_partition(CWideString* first, CWideString* last, CWideString pivot, less<CWideString> comp) -- quicksort partition over wide strings: advance first past smaller strings, retreat last past larger ones, swap the pair, repeat until they cross
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B3580, 146 bytes, `ret 8`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the 4-byte CWideString pivot and the empty comparator on the stack), returns the cut in eax. Catalog name
// "Quicksort_Partition_String" (descriptive). Caller: the __introsort_loop @ 0x009B7D84.
// Callees: CWideString::operator< @ 0x0099AF70, CWideString(const CWideString&) @ 0x0099B720 / operator= @ 0x0099B7D0 /
// ~CWideString @ 0x0099B510 (iter_swap's temporary in the `push ecx` slot, then the by-value pivot).
// Shape: STLport 4.6 verbatim, with iter_swap -> __iter_swap(a, b, value_type) = `_Tp tmp = *a; *a = *b; *b = tmp;` inlined.
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

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template CWideString* __fastcall std::__unguarded_partition<CWideString*, CWideString, std::less<CWideString> >(
    CWideString* __first, CWideString* __last, CWideString __pivot, std::less<CWideString> __comp);