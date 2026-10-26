// std::__make_heap(const CAnimationEntry** first, const CAnimationEntry** last, CTurningAnimCompare comp, const CAnimationEntry**, int*) -- STLport heap build over the turning-animation candidates: sift every parent down from (len - 2) / 2 to 0
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E0D0, 77 bytes (+ int3 padding to 80), `__fastcall` (/Gr algorithm TU: first in
//   ecx, last in edx; the 4-byte comparator, the _Tp* and _Distance* tags on the stack), `ret 0Ch`.
// Identity (proven): _RandomAccessIterator = const CAnimationEntry**, _Compare = CTurningAnimCompare, _Tp = const CAnimationEntry*, _Distance = int. its callers are the partial_sort 0x0087E450 of std::sort(const CAnimationEntry**, const CAnimationEntry**, CTurningAnimCompare) 0x0087E610 (H008's sort_heap 0x0087E3B0 is the same instantiation) and 0x0087E2D0, its callee the __adjust_heap 0x0087DE50 (catalog ??$__adjust_heap@PAPBVCAnimationEntry@@HPBV1@VCTurningAnimCompare@?A0x...); both PDB name sets hold the heap helpers over const CAnimationEntry** with the anonymous-namespace CTurningAnimCompare (?A0x37766ae9).
// Label: the manifest's "std::make_heap_CAnimationEntry" is propagated across byte-identical bodies (family J012: six rows that differ only in
//   their call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: __adjust_heap(first, parent, len, value, comp) @ 0x0087DE50 (this instantiation's own COMDAT, __fastcall `ret 0Ch`).
// Shape: STLport 4.6 _heap.c `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`; the signed
//   halving is `cdq / sub / sar`. Model: the landed 0x005176D0 (CThingBuilding*, CIsHouseLargerThan).
class CAnimationEntry;
namespace {
class CTurningAnimCompare {                         // Ego_r.pdb: 4 bytes, anonymous namespace
public:
    float RotationAngle;                            // +0x00
};
}
typedef CTurningAnimCompare TComp;
namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x0087DE50

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
template void __fastcall __make_heap<const CAnimationEntry**, TComp, const CAnimationEntry*, int>(const CAnimationEntry**, const CAnimationEntry**, TComp, const CAnimationEntry**, int*);
}   // namespace std