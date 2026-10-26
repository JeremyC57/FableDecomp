// std::__make_heap(const CThing** first, const CThing** last, bool (__fastcall*)(const CThing*, const CThing*) comp, const CThing**, int*) -- STLport heap build over the minion's criminal candidates (a plain function-pointer comparator): sift every parent down from (len - 2) / 2 to 0
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00932540, 77 bytes (+ int3 padding to 80), `__fastcall` (/Gr algorithm TU: first in
//   ecx, last in edx; the 4-byte comparator, the _Tp* and _Distance* tags on the stack), `ret 0Ch`.
// Identity (proven): _RandomAccessIterator = const CThing**, _Compare = bool (__fastcall*)(const CThing*, const CThing*), _Tp = const CThing*, _Distance = int. its callers are the partial_sort 0x00932770 of the introsort 0x009327F0 / sort 0x009328D0 that CAIStateGroup_MinionReactToCriminal::Update runs with the file-local compare 0x00930D90, and 0x00932610; its callee is the __adjust_heap 0x00932420; both PDB name sets hold this sort only over const CThing** with bool (__fastcall*)(const CThing*, const CThing*) (?_Make_heap / ?_Adjust_heap@PAPBVCThing@@HPBV1@P6I_NPBV1@0@Z@...), so the catalog's non-const `??$__make_heap@PAPAVCThing@@P6I_NPAV1@0@Z...` is corrected here.
// Label: the manifest's "Sort_BuildHeap_Buildings" is propagated across byte-identical bodies (family J012: six rows that differ only in
//   their call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: __adjust_heap(first, parent, len, value, comp) @ 0x00932420 (this instantiation's own COMDAT, __fastcall `ret 0Ch`).
// Shape: STLport 4.6 _heap.c `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`; the signed
//   halving is `cdq / sub / sar`. Model: the landed 0x005176D0 (CThingBuilding*, CIsHouseLargerThan).
class CThing;
typedef bool (__fastcall* TComp)(const CThing* a, const CThing* b);   // the file-local height compare 0x00930D90
namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x00932420

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template void __fastcall __make_heap<const CThing**, TComp, const CThing*, int>(const CThing**, const CThing**, TComp, const CThing**, int*);
}   // namespace std