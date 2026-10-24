// _STL::__adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CCompare_FurthestFromInitPos2D comp) -- sift a hole down through the children the predicate orders last, then push the value back up
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748C90, 115 bytes, `ret 0Ch` (link-time __fastcall: first in ecx, holeIndex in
//   edx, then len, the value and the comparator on the stack).
// Callees: CCompare_FurthestFromInitPos2D::operator()(CThing*, CThing*) const @ 0x007483E0 (landed),
//   __push_heap(first, holeIndex, topIndex, val, comp) @ 0x00748880 (__fastcall, `ret 0Ch`; the manifest's
//   `_Push_heap<float*,...>` is a propagation).
// Callers: the thing-pointer heap/partial-sort family (0x00749050 make_heap, 0x00749530 partial_sort, 0x00749390 sort_heap).
// Label: the manifest's `_Adjust_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan>` is a propagated
//   Dinkumware spelling; the comparator is the one landed at 0x007483E0 (CCompare_FurthestFromInitPos2D).
// Shape: STLport `__adjust_heap`: topIndex is homed in the `push ecx` word; the predicate runs on the comparator
//   argument slot (`lea ecx,[esp+28h]`) with the two child pointers pushed right to left; `dec esi` picks the left child;
//   the tail passes the value and the comparator dword straight from their argument slots to __push_heap.
struct CThing;

struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const;              // 0x007483E0
};

namespace _STL {
void __fastcall __push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CCompare_FurthestFromInitPos2D comp);   // 0x00748880

void __fastcall __adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CCompare_FurthestFromInitPos2D comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}
}