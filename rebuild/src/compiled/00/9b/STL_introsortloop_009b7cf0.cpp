// std::__introsort_loop(CWideString* first, CWideString* last, CWideString*, int depth_limit, less<CWideString> comp) -- STLport introsort over wide strings: median-of-three partition while the range exceeds 16 elements, recursing on the upper part and falling back to partial_sort when the depth runs out
// Flags: /Gr
// Leaf: ??$__introsort_loop@PAVCWideString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B7CF0, 217 bytes (+ int3 padding), `ret 0Ch` (/Gr TU: first in ecx,
//   last in edx; the unused T*, depth and the empty comparator on the stack).
// Identity: sort<CWideString*, less<CWideString>> (caller 0x009B8476, plus its own recursion 0x009B7D97); its
//   depth-exhausted path is the __partial_sort row 0x009B6A40.  Catalogued as `Quicksort_Partition_String`.
// Callees: CWideString::operator< @ 0x0099AF70 (less<> inlined in __median), CWideString(const CWideString&) @ 0x0099B720
//   (the pivot built in the argument area), __unguarded_partition(first, last, pivot, comp) @ 0x009B3580,
//   __partial_sort @ 0x009B6A40.
// Shape: STLport _algo.c __introsort_loop with partial_sort and __median inlined (__median's nested if/else returns pick
//   first / middle / last - 1); `last - first > 16` compiles to `and eax,-4 ; cmp eax,40h`.
class CWideString {
public:
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool operator<(const CWideString& other) const;     // 0x0099AF70
private:
    void* PStringData;
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _Tp, class _Compare>
__forceinline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x009B6A40

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x009B3580

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __introsort_loop(CWideString*, CWideString*, CWideString*, int, less<CWideString>);
}