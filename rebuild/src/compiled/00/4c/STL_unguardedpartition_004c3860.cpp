// _STL::__unguarded_partition(CMeshInfo* first, CMeshInfo* last, CMeshInfo pivot, CSortPrimaryMeshesFirst comp) -- STLport quicksort partition of a mesh-info run around a by-value pivot under the "primary meshes first" order; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C3860, 124 bytes (+ int3 padding), `ret 10h` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, last in edx, the 12-byte pivot and the empty comparator's 4-byte slot on the stack).
// Caller: the CSortPrimaryMeshesFirst __introsort_loop 0x004C59B0 (this chunk). No callees.
// Identity: NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes: Index, SizeX, PrimaryMesh:1) sorted by
//   CSortPrimaryMeshesFirst (Ego_r.pdb, empty) -- the same element/comparator pair as the landed __adjust_heap 0x004C2C70,
//   __insertion_sort 0x004C5100 and __unguarded_insertion_sort_aux 0x004C2B90. The catalog's
//   "std::Introsort_Partition_Barrier" is descriptive.
// Shape: STLport 4.x `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`. The comparator reads only its LEFT operand
//   (`a.PrimaryMesh`), so `comp(pivot, *last)` is loop-invariant: VC7.1 hoists the pivot bit into a dead argument byte
//   (`and cl,bl ; mov [esp+20h],cl`) and the inner `--last` loop collapses to the `jmp $` self-loop retail carries
//   (`ebfe`, reached only with a primary pivot). The inlined __iter_swap temporary is the implicit 12-byte POD copy.
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

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
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
template CMeshInfo* __fastcall __unguarded_partition<CMeshInfo*, CMeshInfo, CSortPrimaryMeshesFirst>(CMeshInfo*, CMeshInfo*, CMeshInfo, CSortPrimaryMeshesFirst);
}