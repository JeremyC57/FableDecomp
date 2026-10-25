// std::__unguarded_partition(CMeshInfo* first, CMeshInfo* last, CMeshInfo pivot, CSortPrimaryMeshesLast comp) -- STLport quicksort partition of a mesh-info run around a by-value pivot under the "primary meshes last" order; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C38F0, 125 bytes (+ int3 padding), `ret 10h` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, last in edx, the 12-byte pivot and the empty comparator's 4-byte slot on the stack).
// Caller: the CSortPrimaryMeshesLast __introsort_loop 0x004C5AA0 (this chunk). No callees.
// Identity: NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes) sorted by CSortPrimaryMeshesLast (Ego_r.pdb, empty;
//   `!a.PrimaryMesh`) -- the sibling of 0x004C3860, as the landed __insertion_sort pair 0x004C5100 / 0x004C5190. The
//   catalog's "std::introsort_Partition_Quicksort_Inverse_Body" is descriptive.
// Shape: STLport 4.x `__unguarded_partition` verbatim. The comparator reads only its LEFT operand, so the bit is
//   inverted with `not` before every `test ...,1`; `comp(pivot, *last)` is hoisted into a dead argument byte and its
//   loop collapses to the `jmp $` self-loop (`ebfe`) retail carries. The swap temporary is the implicit 12-byte POD copy.
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
template CMeshInfo* __fastcall __unguarded_partition<CMeshInfo*, CMeshInfo, CSortPrimaryMeshesLast>(CMeshInfo*, CMeshInfo*, CMeshInfo, CSortPrimaryMeshesLast);
}