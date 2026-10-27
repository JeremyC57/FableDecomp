// std::__introsort_loop(float* first, float* last, float*, int depth_limit, less<float> comp) -- STLport introsort recursion over a float range: median-of-three partition until 16 elements remain, heap sort when the depth budget runs out
// Leaf: ??$__introsort_loop@PAMMHU?$less@M@std@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5AAA0, 242 bytes, __fastcall (first ecx, last edx), `ret 0Ch` (the float*
//   tag, the depth limit, the empty less<float>).
// Label: the manifest's `Quicksort_Float` is descriptive; the body is STLport 4.6's
//   `__introsort_loop<float*, float, int, less<float> >` (the same TU as the landed `__partial_sort<float*, float,
//   less<float> >` 0x00A5A8A0, which it calls for the depth-exhausted case, and itself for the right half).
// Shape: as the landed 0x00AD6F10 / 0x00759660 loops, but with less<float> both __median (fcomp/`test ah,5`) and
//   __unguarded_partition (fcom against the float pivot held in st(0), `test ah,41h`) inline; the threshold folds to
//   `and eax,-4 ; cmp eax,40h`.
namespace std {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
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

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, (float*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
inline _RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
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

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00A5A8A0

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (float*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
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

template void __introsort_loop<float*, float, int, less<float> >(float*, float*, float*, int, less<float>);

}   // namespace std