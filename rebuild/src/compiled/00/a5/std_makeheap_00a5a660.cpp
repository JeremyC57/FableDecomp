// std::__make_heap(float* first, float* last, less<float> comp, float*, int*) -- STLport heap build over a range of floats (STLport's less<float>): sift every parent down from (len - 2) / 2 to 0
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A660, 77 bytes (+ int3 padding to 80), `__fastcall` (/Gr algorithm TU: first in
//   ecx, last in edx; the 4-byte comparator, the _Tp* and _Distance* tags on the stack), `ret 0Ch`.
// Identity (proven): _RandomAccessIterator = float*, _Compare = less<float>, _Tp = float, _Distance = int. its callers are the __partial_sort 0x00A5A8A0 of the float introsort 0x00A5AAA0 / sort 0x00A5ABA0 that 0x00A59FD0 (reached from CPhysicalPrimitiveCylinder::GetFirstIntersectionWithFiniteLine) runs on its intersection distances, and 0x00A5A700; its callee 0x00A5A580 compares the values inline with `fld / fcomp` (less<float>); the catalog binds ??$__make_heap@PAMU?$less@M@std@@MH@std@@... here and FableWin.pdb holds _Make_heap<float*, int, float, less<float> >.
// Label: the manifest's "MakeHeap_CAStarState_Ptr" is propagated across byte-identical bodies (family J012: six rows that differ only in
//   their call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: __adjust_heap(first, parent, len, value, comp) @ 0x00A5A580 (this instantiation's own COMDAT, __fastcall `ret 0Ch`).
// Shape: STLport 4.6 _heap.c `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`; the signed
//   halving is `cdq / sub / sar`. Model: the landed 0x005176D0 (CThingBuilding*, CIsHouseLargerThan).

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}
typedef std::less<float> TComp;
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x00A5A580

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
template void __fastcall __make_heap<float*, TComp, float, int>(float*, float*, TComp, float*, int*);
}   // namespace std