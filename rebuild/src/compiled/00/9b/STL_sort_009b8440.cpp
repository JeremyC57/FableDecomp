// _STL::sort(CWideString* first, CWideString* last) -- sort a range of wide strings with operator< (the definition manager's path lists): introsort to 16-element runs, then the final insertion sort
// Leaf: ??$sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8440, 117 bytes (+ int3 padding), plain `ret` (/Gr algorithm TU:
//   first in ecx, last in edx, no stack arguments).
// Identity: STLport 4.x two-argument `sort(first, last)` over CWideString, called by CDefinitionManager::
//   CreateSymbolsFromPathList 0x009AEF60 and DoCompilePathList 0x009AF480 (both landed, declared there as
//   `SortWideStrings`). The catalog label "introsort_build_heapify" is descriptive.
// Callees: __introsort_loop<CWideString*, CWideString, int, less<CWideString> > @ 0x009B7CF0 (landed), __insertion_sort
//   @ 0x009B68D0 (landed), __unguarded_insertion_sort_aux(first + 16, last, (_Tp*)0, comp) @ 0x009B2AC0.
// Shape: STLport passes `__less(_STLP_VALUE_TYPE(first))` -- an empty less<CWideString> temporary -- to every callee:
//   VC7.1 keeps it in the `push ecx` frame word and pushes that (uninitialised) word each time. `__lg` is inline and
//   __final_insertion_sort is inline with its threshold test reusing the element count (`cmp esi,10h`).
class CWideString {
public:
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool operator<(const CWideString& other) const;     // 0x0099AF70
private:
    void* PStringData;
};

namespace _STL {
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
// less<> derives from binary_function, so it is not a POD: `less<_Tp>()` default-initialises (no zero store)
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline less<_Tp> __less(_Tp*) { return less<_Tp>(); }
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

const int __stl_threshold = 16;

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);   // 0x009B7CF0
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x009B68D0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);   // 0x009B2AC0

template <class _RandomAccessIter, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIter, class _Compare>
inline void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > __stl_threshold) {
        __insertion_sort(__first, __first + __stl_threshold, __comp);
        __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template <class _Size>
inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2,
                         __less(__value_type(__first)));
        __final_insertion_sort(__first, __last, __less(__value_type(__first)));
    }
}
template void __fastcall sort<CWideString*>(CWideString*, CWideString*);
}