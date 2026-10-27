// std::__introsort_loop(_RandomAccessIter first, _RandomAccessIter last, _Tp*, _Size depth_limit, _Compare comp) -- (const CAnimationEntry*, CTurningAnimCompare) STLport introsort: while the range exceeds 16 elements, partition around the median of three and recurse on the right part; once the depth limit runs out, partial_sort the range
// Leaf: ??$__introsort_loop@PAPBVCAnimationEntry@@PBV1@HVCTurningAnimCompare@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087e530, 136 bytes, __fastcall (first in ecx, last in edx; (_Tp*)0, depth_limit and the
//   4-byte comparator on the stack), `ret 0Ch`.
// Identity (proven): bound ??$__introsort_loop@PAPBVCAnimationEntry@@PBV1@HVCTurningAnimCompare@?A0x2d9ddda0@@@std@@...; called by the landed std::sort<const CAnimationEntry**, CTurningAnimCompare> 0x0087E610.
// Callees: __median 0x0087DA90, __unguarded_partition 0x0087DFA0, __partial_sort 0x0087E450 (all __fastcall, out of line),
//   itself (the right-part recursion).
// Shape (family L006): STLport 4.6 _algo.c `__introsort_loop` verbatim; the median's reference result is dereferenced into
//   the by-value pivot, partial_sort is the inline front end over __partial_sort(first, middle, last, value type, comp).
class CAnimationEntry;
namespace {
class CTurningAnimCompare {                                 // Ego_r.pdb, 4 bytes, anonymous namespace
public:
    float RotationAngle;                                  // +0x00
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const;   // 0x0087C660
};
}
namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);              // 0x0087DA90
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _Tp __pivot, _Compare __comp);                              // 0x0087DFA0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                               _Tp*, _Compare __comp);                                                         // 0x0087E450
template <class _Tp> inline _Tp* __value_type(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > 16) {                       // __stl_threshold
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
}
template void __fastcall std::__introsort_loop<const CAnimationEntry**, const CAnimationEntry*, int, CTurningAnimCompare >(const CAnimationEntry**, const CAnimationEntry**, const CAnimationEntry**, int, CTurningAnimCompare);