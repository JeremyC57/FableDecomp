// std::__push_heap(CThing** first, int hole, int top, CThing* value, `anonymous namespace'::CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- heap sift-up of thing pointers under the in-direction-and-nearest comparator
// Leaf: ??$__push_heap@PAPAVCThing@@HPAV1@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097ADF0, 97 bytes, `ret 0x18`.
// Label correction: the catalog "_Push_heap<float*,int,float,std::less<float>>" is propagated; the
// predicate called here is CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(CThing*,
// CThing*) @ 0x0097ABF0 (manifest), so the heap holds CThing pointers.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the pointer value
// and the 16-byte comparator by value (4 + 4 + 16 = 0x18).  The comparator reads four floats at
// +0..+0xC (0x0097ABF0: a 2D origin and a 2D direction); its operator() is out of line (`this` = the
// comparator slot, `push value ; push parent`).
// Shape: SGI/STLport `__push_heap`; the drop is tail-duplicated into both exits (as 0x005140A0).
// Identity: the caller std::__adjust_heap 0x0097AF70 binds this address as ??$__push_heap@PAPAVCThing@@HPAV1@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x...@@@std@@ (naming wave 14; the global `_Push_heap` was a stand-in).
class CThing;

namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {   // Ego_r.pdb 16 bytes, anonymous namespace (?A0xeaff0e08)
public:
    float OriginX;                              // +0x0
    float OriginY;                              // +0x4
    float DirectionX;                           // +0x8
    float DirectionY;                           // +0xc
    bool operator()(const CThing* a, const CThing* b) const;   // 0x0097ABF0
};
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator first, _Distance hole, _Distance top, _Tp value, _Compare comp)
{
    _Distance parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
template void __fastcall __push_heap<CThing**, int, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch>(CThing**, int, int, CThing*,
    CThingCompare_InDirectionOfAndNearestToHeuristicBitch);
}