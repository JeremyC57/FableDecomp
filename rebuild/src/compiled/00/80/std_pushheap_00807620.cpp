// std::__push_heap(pair<C3DVector, long>* first, int holeIndex, int topIndex, pair<C3DVector, long> val, CCompareTargetedPositionsForFirstPersonTargeting comp) -- STLport heap sift-up for the first-person targeting position sort (best-for-targeting score, larger first)
// Leaf: ??$__push_heap@PAU?$pair@VC3DVector@@J@std@@HU12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x
//   (full: ??$__push_heap@PAU?$pair@VC3DVector@@J@std@@HU12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x<tu-hash>@@@std@@YIXPAU?$pair@VC3DVector@@J@0@HHU10@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x<tu-hash>@@@Z;
//   the anonymous-namespace hash is per TU -- retail's is ?A0x58ed6966 -- so the Leaf stops before it, as the landed TU sibling 0x00807170)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807620, 164 bytes, /Gr `__fastcall` (first in ecx, holeIndex in edx, topIndex,
//   the 16-byte pair and the 0x2c-byte comparator by value on the stack), `ret 40h`.
// Name: STLport 4.6 `__push_heap` (<stl/_heap.c>); Ego_r.pdb / FableWin.pdb name the Dinkumware counterpart
//   std::_Push_heap<pair<C3DVector,long>*, int, pair<C3DVector,long>, `anonymous namespace'::CCompareTargetedPositionsForFirstPersonTargeting>
//   (the catalog label). Only caller 0x008083EE (this sort TU's __adjust_heap).
// Comparator: the shared CVectorCompare_BestForTargeting header with its inline operator() (parenthesised named values) and its
//   scoring bodies visible (FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES), exactly as the landed TU siblings 0x00807170 /
//   0x00807250 / 0x00808AA0: the second GetCompareValueForPoint argument (`lea ecx,[esp+1Ch] ; push ecx`, retail 0x00807652) then
//   issues before the `fstp` of the first value. The October draft's local model with a declaration-only scorer and an
//   unparenthesised compare pushed it after the store (2 lines).
// Callee: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (twice per compare).
// Shape: STLport `__push_heap`: parent = (hole - 1) / 2; sift up while hole > top and comp(first[parent], val); the pair's
//   implicit copy is four dword moves.
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES
#include "stlport/fable_containers.h"
#include "engine/CVectorCompare_BestForTargeting.h"

namespace {
class CCompareTargetedPositionsForFirstPersonTargeting {
public:
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        return VectorCompare(a.first, b.first);
    }
};
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template void __fastcall __push_heap(pair<C3DVector, long>*, int, int, pair<C3DVector, long>,
                                     CCompareTargetedPositionsForFirstPersonTargeting);
}