// std::__adjust_heap(CThing** first, int holeIndex, int len, CThing* val, ThingCompare comp) -- STLport heap sift-down: walk the hole to a leaf along the larger child, then push val back up from there
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00932420, 108 bytes (112 with padding), `ret 0Ch` (first in ecx, holeIndex in edx; len, val and comp on the stack).
// Name: STLport `__adjust_heap<CThing**, int, CThing*, bool (__fastcall*)(CThing*, CThing*)>`; seven callers (__make_heap,
//   __partial_sort 0x009327AE, the inlined pop_heaps of sort_heap). Callee: __push_heap 0x00932370 (landed). The catalog's
//   "_Adjust_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan>" is the propagated Dinkumware spelling.
// Element: CThing* sorted under a plain `bool (__fastcall*)(CThing*, CThing*)` function pointer -- the sort of
//   CAIStateGroup_MinionReactToCriminal::Update (0x0093150A: `sort(first, last, &0x00930D90)`, a file-local compare whose
//   body is `a->GetPosition().Z < b->GetPosition().Z` through CThing::GetPosition 0x004C73D0). The pointee is only ever
//   handed to GetPosition, so CThing (or a class with CThing at offset 0) is all the bytes show. The catalog's
//   CThingBuilding / CIsHouseLargerThan labels are propagated from the Ego_r.pdb Dinkumware instantiation of another TU.
// /Gr algorithm TU: the first two pointer/int arguments in ecx/edx (the element value and the comparator pointer are
//   register candidates), the rest on the stack; the comparator is called with both elements in ecx/edx.
// Landed siblings in the TU: __median 0x009322D0, __push_heap 0x00932370, sort 0x009328D0.
// Shape: STLport `top = hole; second = 2 * hole + 2; while (second < len) { if (comp(*(first + second), *(first + (second
//   - 1)))) second--; *(first + hole) = *(first + second); hole = second; second = 2 * (second + 1); } if (second == len)
//   { *(first + hole) = *(first + (second - 1)); hole = second - 1; } __push_heap(first, hole, top, val, comp);` -- top
//   parked in the `push ecx` slot, comp called from its stack slot.
#include <stddef.h>
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);   // 0x00BFEAE6

class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // e.g. the height compare 0x00930D90

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);   // 0x00932370

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

template void __fastcall __adjust_heap(CThing**, int, int, CThing*, ThingCompare);
}