// std::__introsort_loop(_RandomAccessIter first, _RandomAccessIter last, _Tp*, _Size depth_limit, _Compare comp) -- (const CThing*, CCompare_FurthestFromInitPos2D) STLport introsort: while the range exceeds 16 elements, partition around the median of three and recurse on the right part; once the depth limit runs out, partial_sort the range
// Leaf: ??$__introsort_loop@PAPBVCThing@@PBV1@HVCCompare_FurthestFromInitPos2D@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x007496b0, 136 bytes, __fastcall (first in ecx, last in edx; (_Tp*)0, depth_limit and the
//   4-byte comparator on the stack), `ret 0Ch`.
// Identity (proven): its __median 0x00748760 compares through 0x007483E0, bound ??RCCompare_FurthestFromInitPos2D@?A0x30a63056@@QBE_NPBVCThing@@0@Z; the sort front end 0x00749740 that calls it is called by CTCStealth::UpdateDangerInfo 0x00747C70 with an empty comparator byte; Ego_r.pdb holds the const CThing** / ?A0x..::CCompare_FurthestFromInitPos2D sort helpers.
// Callees: __median 0x00748760, __unguarded_partition 0x00748B60, __partial_sort 0x00749530 (all __fastcall, out of line),
//   itself (the right-part recursion).
// Shape (family L006): STLport 4.6 _algo.c `__introsort_loop` verbatim; the median's reference result is dereferenced into
//   the by-value pivot, partial_sort is the inline front end over __partial_sort(first, middle, last, value type, comp).
class CThing;
namespace {
class CCompare_FurthestFromInitPos2D {                      // Ego_r.pdb, anonymous namespace of the CTCStealth TU (empty)
public:
    bool operator()(const CThing* a, const CThing* b) const;   // 0x007483E0
};
}
namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);              // 0x00748760
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _Tp __pivot, _Compare __comp);                              // 0x00748B60
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                               _Tp*, _Compare __comp);                                                         // 0x00749530
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
template void __fastcall std::__introsort_loop<const CThing**, const CThing*, int, CCompare_FurthestFromInitPos2D >(const CThing**, const CThing**, const CThing**, int, CCompare_FurthestFromInitPos2D);