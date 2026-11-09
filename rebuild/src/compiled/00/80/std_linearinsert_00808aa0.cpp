// std::__linear_insert(pair<C3DVector, long>* first, pair<C3DVector, long>* last, pair<C3DVector, long> val, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport insertion-sort step of the first-person targeting sort: if val scores better than *first, shift [first, last) up one slot and put val first, else insert it from the back
// Leaf: ??$__linear_insert@PAU?$pair@VC3DVector@@J@std@@U12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808AA0, 229 bytes, /Gr `__fastcall` (first in ecx, last in edx), the 16-byte
//   value and the 0x2c-byte comparator by value on the stack, `ret 3Ch`.
// Name: STLport `__linear_insert` (stlport/fable_containers.h FABLE_RETAIL_STLP_SORT_INTERNALS); Ego_r.pdb's Dinkumware
//   counterpart is `_Insertion_sort1<pair<C3DVector,long>*, ..>` (donor 0x00801B70).
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint 0x007F9660 (twice), STLport __copy_backward 0x00806310,
//   __unguarded_linear_insert 0x008074A0 (landed).
// Layout: Ego_r.pdb CCompareTargetedPositionsForFirstPersonTargeting (anonymous namespace, 0x2c: the CVectorCompare_BestForTargeting
//   it wraps); pair<C3DVector, long> 16 bytes (STLport's explicit pair copy constructor, C3DVector's user copy constructor).
// Shape: the TU sees the scoring bodies as inline definitions (FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES), as retail's
//   LTCG build did: the by-value comparator and value copies of the else arm then pipeline their loads one store ahead
//   (0x00808B07..0x00808B5C) because c2 knows neither object's address is captured, while `last` stays in edi across the
//   out-of-line score calls. The comparator's two values are named locals (the header's operator()).
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

template void FABLE_STLP_FASTCALL std::__linear_insert(std::pair<C3DVector, long>*, std::pair<C3DVector, long>*, std::pair<C3DVector, long>,
                                                       CCompareTargetedPositionsForFirstPersonTargeting);