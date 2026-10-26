// std::__make_heap(const CThing** first, const CThing** last, CCompare_FurthestFromInitPos2D comp, const CThing**, int*) -- STLport heap build over the furthest-from-initial-position thing sort: sift every parent down from (len - 2) / 2 to 0
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749050, 77 bytes (+ int3 padding to 80), `__fastcall` (/Gr algorithm TU: first in
//   ecx, last in edx; the 4-byte comparator, the _Tp* and _Distance* tags on the stack), `ret 0Ch`.
// Identity (proven): _RandomAccessIterator = const CThing**, _Compare = CCompare_FurthestFromInitPos2D, _Tp = const CThing*, _Distance = int. its callers are the partial_sort 0x00749530 (whose sort_heap 0x00749390 is H008's const CThing* / CCompare_FurthestFromInitPos2D row) and 0x007491F0, and its callee 0x00748C90 the __adjust_heap of the same instantiation; both PDB name sets spell the heap helpers of this sort with const CThing* and the anonymous-namespace CCompare_FurthestFromInitPos2D (?A0xae0dd725); the catalog's non-template `?__make_heap@std@@YIXPAPAVCThing@@0UCCompare_FurthestFromInitPos2D@@0PAH@Z` is a landed caller's stand-in.
// Label: the manifest's "Heap_MakeHeap" is propagated across byte-identical bodies (family J012: six rows that differ only in
//   their call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: __adjust_heap(first, parent, len, value, comp) @ 0x00748C90 (this instantiation's own COMDAT, __fastcall `ret 0Ch`).
// Shape: STLport 4.6 _heap.c `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`; the signed
//   halving is `cdq / sub / sar`. Model: the landed 0x005176D0 (CThingBuilding*, CIsHouseLargerThan).
class CThing;
namespace {
class CCompare_FurthestFromInitPos2D {              // Ego_r.pdb: empty, anonymous namespace
public:
    bool operator()(const CThing* a, const CThing* b) const;   // 0x007483E0
};
}
typedef CCompare_FurthestFromInitPos2D TComp;
namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x00748C90

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