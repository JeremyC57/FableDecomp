// std::__push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CThingCompare_BestForTargeting2 comp) -- sift a targeting candidate up the heap from holeIndex toward topIndex
// Leaf: ??$__push_heap@PAPAVCThing@@HPAV1@VCThingCompare_BestForTargeting2@@@std@@YIXPAPAVCThing@@HHPAV1@VCThingCompare_BestForTargeting2@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FA160, 197 bytes, /Gr `__fastcall` (first in ecx, holeIndex in edx, topIndex, the value and
//   the 52-byte comparator by value on the stack), `ret 3Ch`.
// Name: STLport 4.6 `__push_heap` (<stl/_heap.c>); the PDBs name the Dinkumware counterpart
//   std::_Push_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2> (the catalog label). Only caller 0x007FA43A (this
//   sort TU's __adjust_heap).
// Comparator: the landed operator() 0x007F9F00 body (multipliers computed and discarded, positions b first, named values) inlined;
//   CVectorCompare_BestForTargeting from the shared header with its scoring bodies visible (FABLE_RETAIL_CVECTORCOMPARE_
//   BESTFORTARGETING_BODIES, as the landed 0x00807170 / 0x00808AA0 sort rows): c2 then knows the comparator is not captured and
//   reloads the posB reference and pushes it (`mov ecx,[esp+10h] ; push ecx`, retail 0x007FA1BE) before the `fstp` of valueA.
//   The October drafts' local model with a declaration-only scorer stored first (2 lines). No parentheses needed here.
// Callees: CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing*) 0x007F9E70, CThing::GetPos() const 0x004C73D0,
//   CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) const 0x007F9660 (reloc_bindings.tsv).
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 { CThing* Hero; CVectorCompare_BestForTargeting VectorCompare; float
//   EnemyPreferenceFactor; } (52 bytes; no shared header yet, the landed siblings' model).
#define FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES
#include "engine/CThing.h"
#include "engine/CVectorCompare_BestForTargeting.h"

class CThingCompare_BestForTargeting2 {
public:
    CThing* Hero;                                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;              // +0x04
    float EnemyPreferenceFactor;                                // +0x30
    float GetThingCompareMultiplier(CThing* thing);             // 0x007F9E70
    bool operator()(CThing* a, CThing* b)
    {
        float multiplierA = GetThingCompareMultiplier(a);
        float multiplierB = GetThingCompareMultiplier(b);
        const C3DVector& posB = b->GetPos();
        const C3DVector& posA = a->GetPos();
        float valueA = VectorCompare.GetCompareValueForPoint(posA);
        float valueB = VectorCompare.GetCompareValueForPoint(posB);
        if (valueA > valueB)
            return true;
        return false;
    }
};

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

template void __fastcall __push_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2>(CThing**, int, int, CThing*, CThingCompare_BestForTargeting2);
}