// std::__introsort_loop(long* first, long* last, long*, int depth_limit, less<long> comp) -- STLport introsort loop over 4-byte signed integers: median-of-three partition and recurse on the right part while more than 16 elements remain, falling back to partial_sort when the depth limit runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778490, 173 bytes (+ int3 padding), /Gr `__fastcall` `ret 0Ch` (first in
//   ecx -> edi, last in edx -> ebx; the value-type tag pointer, the depth limit and the empty comparator on the stack).
// Callees: __unguarded_partition<long*> @ 0x00776D40 (this chunk; the pivot value and the comparator stacked), itself,
//   __partial_sort<long*> @ 0x007781B0 (this chunk).
// Caller: sort<long*, less<long> > 0x00778540 (`push 0 ; call` with depth 2 * lg(n)), the family of the landed sort_heap
//   0x00777EE0 (SIGNED compares).
// Label correction: catalogued as `Sort_IntrosortPhase_IntElem`; the body is STLport `__introsort_loop`.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last, comp);
//   return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last - first) / 2),
//   *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` with `__median`
//   inline (`(last - first) > 16` as `and eax,-4 ; cmp eax,40h`) and the decremented depth stored back to its slot.
namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                                   // 0x00776D40
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                                     // 0x007781B0

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __partial_sort_front(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                 _Tp*, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (_Tp*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            __partial_sort_front(__first, __last, __last, (_Tp*)0, __comp);
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

}

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

template void __fastcall std::__introsort_loop<long*, long, int, std::less<long> >(long*, long*, long*, int, std::less<long>);