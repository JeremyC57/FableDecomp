// std::__unguarded_insertion_sort_aux(NGraphicAppearance::CMeshInfo* first, NGraphicAppearance::CMeshInfo* last, NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesLast comp) -- STLport: unguarded linear insert of every element of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@PAVCMeshInfo@NGraphicAppearance@@V12@VCSortPrimaryMeshesLast@@@std@@YIXPAVCMeshInfo@NGraphicAppearance@@00VCSortPrimaryMeshesLast@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C2C00, 105 bytes, `ret 8` (__fastcall /Gr algorithm TU: first in ecx,
//   last in edx, the value-type tag pointer and the 4-byte comparator slot on the stack).
// Identity: the CSortPrimaryMeshesLast twin of the landed 0x004C2B90 (CSortPrimaryMeshesFirst): its caller is the
//   landed __final_insertion_sort 0x004C5770 (0x004C57A5), whose __insertion_sort 0x004C5190 is the landed
//   CMeshInfo / CSortPrimaryMeshesLast instance; the other caller 0x004C3977 sits in the same mesh-sort code. Element
//   (Ego_r.pdb) NGraphicAppearance::CMeshInfo {long Index; float SizeX; uchar PrimaryMesh:1} (12-byte POD); comparator
//   CSortPrimaryMeshesLast (Ego_r.pdb, empty, by value) = `!a.PrimaryMesh`. The catalog label
//   "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is a propagated Dinkumware name.
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the insert
//   inlined. The comparator reads only its left operand, so `comp(val, *next)` is loop-invariant: a non-primary value
//   enters the exit-less backward copy loop (retail's `jmp` back without a test -- the sort hangs on such input, as the
//   landed twin notes), a primary one is stored back into its own slot. `last` is spilled to the frame and the hoisted
//   predicate bit (`not dl ; and dl,1`) to a dead frame byte.
namespace NGraphicAppearance {
class CMeshInfo {                               // Ego_r.pdb, 12 bytes of POD
public:
    long          Index;                        // +0x00
    float         SizeX;                        // +0x04
    unsigned char PrimaryMesh : 1;              // +0x08 bit 0
};
}

class CSortPrimaryMeshesLast {                  // Ego_r.pdb: empty functor, passed by value (4-byte slot)
public:
    bool operator()(const NGraphicAppearance::CMeshInfo& a, const NGraphicAppearance::CMeshInfo& b) const { return !a.PrimaryMesh; }
};

namespace std {

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<NGraphicAppearance::CMeshInfo*, NGraphicAppearance::CMeshInfo, CSortPrimaryMeshesLast>(
    NGraphicAppearance::CMeshInfo*, NGraphicAppearance::CMeshInfo*, NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesLast);

} // namespace std