// std::__introsort_loop(pair<unsigned long,unsigned long>* first, pair<unsigned long,unsigned long>* last, pair<unsigned long,unsigned long>*, int depth_limit, less<pair<unsigned long,unsigned long> > comp) -- STLport introsort recursion over (unsigned, unsigned) pairs: median-of-three partition until 16 elements remain, heap sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6BEC0, 248 bytes, __fastcall (first ecx, last edx), `ret 0Ch` (the _Tp* tag,
//   the depth limit, the empty comparator).
// Label: the manifest's `QuickSort_Median3_Partition` is descriptive; the body is STLport 4.6's
//   `__introsort_loop<pair<unsigned long, unsigned long>*, pair<..>, int, less<pair<..> > >`: it calls the landed
//   `__unguarded_partition` 0x00A6B420 and `__partial_sort` 0x00A6BBB0 of the same pair/less instance (this TU's
//   _Push_heap/_Adjust_heap/_Make_heap/sort_heap 0x00A6B1E0..0x00A6B990), and itself.
// Element: std::pair<unsigned long, unsigned long> (8 bytes; the unsigned `ja/jb` key and second compares are
//   pair's operator<), with STLport pair's user copy constructor -- the by-value pivot is built in its argument slot
//   (`sub esp,8 ; mov ecx,esp ; mov [ecx],edx ; mov [ecx+4],eax`).
// Shape: the landed 0x00AD6F10 / 0x00759660 loop: `and eax,-8 ; cmp eax,80h` threshold, `--depth_limit` kept in its
//   argument slot, __median inline (returns a reference), `last = cut` at the end.
namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00A6B420

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00A6BBB0

typedef pair<unsigned long, unsigned long> TPair;

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (TPair*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
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

template void __fastcall __introsort_loop<TPair*, TPair, int, less<TPair> >(TPair*, TPair*, TPair*, int, less<TPair>);

}   // namespace std