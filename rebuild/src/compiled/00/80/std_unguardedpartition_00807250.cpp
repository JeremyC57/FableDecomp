// std::__unguarded_partition(pair<C3DVector, long>* first, pair<C3DVector, long>* last, pair<C3DVector, long> pivot, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport introsort partition step of the first-person targeting sort: move first up past entries that score better than the pivot and last down past entries that score worse, swap the two and repeat until they cross; returns the split point
// Leaf: ??$__unguarded_partition@PAU?$pair@VC3DVector@@J@std@@U12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807250, 199 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx, last in
//   edx), the 16-byte pivot and the 0x2c-byte comparator by value on the stack, `ret 3Ch`.
// Name: STLport `__unguarded_partition` (stlport/fable_containers.h FABLE_RETAIL_STLP_SORT_INTERNALS); Ego_r.pdb's Dinkumware
//   counterpart is `_Unguarded_partition<pair<C3DVector,long>*, `anonymous namespace'::CCompareTargetedPositionsForFirstPersonTargeting>`
//   (donor 0x008041C0).
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (twice per compare).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (anonymous namespace, 0x2c: the CVectorCompare_BestForTargeting
//   it wraps); pair<C3DVector, long> 16 bytes.
// Shape: comp(x, y) is CVectorCompare_BestForTargeting::operator()(x.first, y.first) (FableWin 0x024DF1C0, inline in retail):
//   two named values with parenthesised initialisers -- the parentheses keep c2's no-code PAREN tuple on the first call's
//   result, so the pivot `lea eax,[esp+24h]` (0x0080726A) issues between the call and the `fstp` as in retail; iter_swap is
//   STLport's swap through a pair temporary (C3DVector's user copy constructor), the assignments the implicit four-dword copies.
//   The TU sees the scoring bodies as inline definitions, as its sibling 0x00808AA0 needs (no change to this row).
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "engine/CVectorCompare_BestForTargeting.h"

namespace {
class CCompareTargetedPositionsForFirstPersonTargeting {   // Ego_r.pdb, 0x2c
public:
    CVectorCompare_BestForTargeting VectorCompare;          // +0x00
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        return VectorCompare(a.first, b.first);
    }
};
}

template std::pair<C3DVector, long>* FABLE_STLP_FASTCALL std::__unguarded_partition(std::pair<C3DVector, long>*, std::pair<C3DVector, long>*,
                                                                                    std::pair<C3DVector, long>,
                                                                                    CCompareTargetedPositionsForFirstPersonTargeting);