// std::__introsort_loop(CMeshInfo* first, CMeshInfo* last, CMeshInfo*, int depth_limit, CSortPrimaryMeshesFirst comp) -- STLport introsort core over a mesh-info run, primary meshes first: quicksort around a median-of-three pivot until runs are 16 or shorter, heap sort at the depth limit
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C59B0, 231 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, last in edx, then the value-type tag pointer, the depth limit and the comparator slot).
// Callers: itself (the right half, 0x004C5A56) and the sort front end 0x004C5CBD (landed 0x004C5C80).
// Callees: __unguarded_partition 0x004C3860 and __partial_sort 0x004C5220 (both this chunk).
// Identity: NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes) under CSortPrimaryMeshesFirst; the catalog's
//   "std::introsort_Partition_Quicksort" is descriptive.
// Shape: STLport 4.x `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last,
//   comp); return; } --depth_limit; cut = __unguarded_partition(first, last, T(__median(*first, *(first + (last -
//   first)/2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (T*)0, depth_limit, comp); last = cut; }` -- the
//   length test is the 0x2AAAAAAB divide by 12 (repeated for the midpoint), __median returns a reference (two bit tests
//   of the left operands: first and middle; `last - 1` is the fallback address) and the pivot copy is the implicit
//   12-byte POD copy built in the argument area.
namespace NGraphicAppearance {
class CMeshInfo {                                       // Ego_r.pdb NGraphicAppearance::CMeshInfo, 12 bytes of POD
public:
    long          Index;                                // +0x00
    float         SizeX;                                // +0x04
    unsigned char PrimaryMesh : 1;                      // +0x08 bit 0
};
}
using NGraphicAppearance::CMeshInfo;

class CSortPrimaryMeshesFirst {                         // Ego_r.pdb: empty functor, passed by value (4-byte slot)
public:
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};

namespace std {
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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x004C3860
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x004C5220

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {                     // __stl_threshold
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
template void __fastcall __introsort_loop<CMeshInfo*, CMeshInfo, int, CSortPrimaryMeshesFirst>(CMeshInfo*, CMeshInfo*, CMeshInfo*, int, CSortPrimaryMeshesFirst);
}