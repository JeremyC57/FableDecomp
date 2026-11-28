// std::__adjust_heap(pair<C3DVector, long>* first, int holeIndex, int len, pair<C3DVector, long> val, CCompareTargetedPositionsForFirstPersonTargeting comp) -- sift a hole down to a leaf, then push_heap the value back up
// Leaf: ??$__adjust_heap@PAU?$pair@VC3DVector@@J@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008082C0, 317 bytes, __fastcall (first in ecx, holeIndex in edx;
//   len, the 16-byte pair and the 0x2C-byte comparator on the stack), `ret 40h`.
// PDB: Ego_r 0x00801CA0 and FableWin 0x024E24A0 both spell the Dinkumware
//   ??$_Adjust_heap@PAU?$pair@VC3DVector@@J@std@@HU12@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x58ed6966@@@std@@YIXPAU?$pair@VC3DVector@@J@0@HH$$QAU10@VCCompareTargetedPositionsForFirstPersonTargeting@?A0x58ed6966@@@Z
//   The bytes are the STLport 4.6 __adjust_heap body. The landed __make_heap 0x00808CF0 of this family calls it.
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint 0x007F9660 (twice per compare),
//   std::__push_heap 0x00807620.
// Shape: STLport 4.6 `__adjust_heap` (_heap.c). The by-value comparator copy into __push_heap pipelines
//   (two words preloaded before `sub esp,2Ch`) only when the scoring bodies are visible in this TU
//   (FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES), the same opt-in as the landed siblings
//   0x00807620 and 0x00808AA0. operator() calls GetCompareValueForPoint with unparenthesised named
//   values: parentheses put `add ebp,-10h` before the `fstp` (retail has the `fstp` first).
// Label: catalog "_Adjust_heap<..._anon_58ED6966...>" is the Dinkumware spelling; retail links STLport
//   4.6 (std::__adjust_heap). The anonymous-namespace hash is per TU.
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES
#include "stlport/fable_containers.h"
#include "engine/CVectorCompare_BestForTargeting.h"

namespace {
class CCompareTargetedPositionsForFirstPersonTargeting {
public:
    CVectorCompare_BestForTargeting VectorCompare;         // +0x00, 0x2C bytes
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        float aValue = VectorCompare.GetCompareValueForPoint(a.first);
        float bValue = VectorCompare.GetCompareValueForPoint(b.first);
        return aValue > bValue;
    }
};
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);              // 0x00807620

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap(pair<C3DVector, long>*, int, int, pair<C3DVector, long>,
                                       CCompareTargetedPositionsForFirstPersonTargeting);
}