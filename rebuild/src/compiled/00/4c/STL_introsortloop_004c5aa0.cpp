// std::__introsort_loop(CMeshInfo* first, CMeshInfo* last, CMeshInfo*, int depth_limit, CSortPrimaryMeshesLast comp) -- STLport introsort core over a mesh-info run, primary meshes last: quicksort around a median-of-three pivot until runs are 16 or shorter, heap sort at the depth limit
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C5AA0, 239 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, then the value-type tag pointer, the depth limit and the comparator slot).
// Callers: itself (0x004C5B4E) and the sort front end 0x004C5D1D (landed 0x004C5CE0).
// Callees: __unguarded_partition 0x004C38F0 and __partial_sort 0x004C52D0 (both this chunk).
// Identity: NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes) under CSortPrimaryMeshesLast (`!a.PrimaryMesh`, the
//   `not dl ; test dl,1` pairs in __median); the CSortPrimaryMeshesFirst sibling is 0x004C59B0.
// Shape: STLport 4.x `__introsort_loop` verbatim -- see 0x004C59B0; the comparator's `not` costs the extra bytes.
namespace NGraphicAppearance {
class CMeshInfo {                                       // Ego_r.pdb NGraphicAppearance::CMeshInfo, 12 bytes of POD
public:
    long          Index;                                // +0x00
    float         SizeX;                                // +0x04
    unsigned char PrimaryMesh : 1;                      // +0x08 bit 0
};
}
using NGraphicAppearance::CMeshInfo;

class CSortPrimaryMeshesLast {                         // Ego_r.pdb: empty functor, passed by value (4-byte slot)
public:
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return !a.PrimaryMesh; }
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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x004C38F0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x004C52D0

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
template void __fastcall __introsort_loop<CMeshInfo*, CMeshInfo, int, CSortPrimaryMeshesLast>(CMeshInfo*, CMeshInfo*, CMeshInfo*, int, CSortPrimaryMeshesLast);
}