// __make_heap(CThingBuilding** first, CThingBuilding** last, CIsHouseLargerThan comp, CThingBuilding**, int*) -- STLport heap build over a CThingBuilding* range ordered by CIsHouseLargerThan: sift every parent down from (len - 2) / 2 to 0
// Leaf: ??$__make_heap@PAPAVCThingBuilding@@
//
// Retail 0x005176D0, 77 bytes (+ int3 padding to 80), `ret 0Ch` (first in ecx, last in edx; comp, the _Tp* and _Distance* tags on the stack).
// Callee: 0x00515570 __adjust_heap(first, holeIndex, len, value, comp) (__fastcall, `ret 0Ch`).
// Caller: __partial_sort 0x0051ACF0 (make_heap inline there: `push 0 ; push 0 ; push comp`).
// Shape: STLport _heap.c -- `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`.  The signed
//   halving is `cdq / sub / sar`.  The retail TU is /Gr; the convention is spelled as an explicit __fastcall here.
// Label: catalog "std_make_heap" (propagated); STLport spelling __make_heap.
class CThingBuilding;

class CIsHouseLargerThan {
public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;   // 0x00512410
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value,
                              _Compare comp);                                 // 0x00515570

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*)
{
    if (last - first < 2) return;
    _Distance len = last - first;
    _Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
}

template void __fastcall std::__make_heap<CThingBuilding**, CIsHouseLargerThan, CThingBuilding*, int>(
    CThingBuilding**, CThingBuilding**, CIsHouseLargerThan, CThingBuilding**, int*);