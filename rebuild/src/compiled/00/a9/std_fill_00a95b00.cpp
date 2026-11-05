// std::fill(CCountedPointer<C3DGroup2>* first, CCountedPointer<C3DGroup2>* last, const CCountedPointer<C3DGroup2>& value) -- assign value to every element
// Leaf: ??$fill@PAV?$CCountedPointer@VC3DGroup2@@@@V1@@std@@YIXPAV?$CCountedPointer@VC3DGroup2@@@@0ABV1@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a95b00, 94 bytes, /Gr `__fastcall` (first ecx, last edx spilled, value on the stack), `ret 4`.
// Identity (probable): called by the fill insert 0x00A97F70 (landed under the stand-in N00a97f70::vector<Element>), whose copies go through __uninitialized_copy<CCountedPointer<C3DGroup2>*> 0x00A95820 (bound); reloc_bindings names this address only as the stand-in ??$fill@UElement@N00a97f70@@.
// Callee: operator delete 0x00BFE9BC (a released count block); CCountedPointer's operator= inline (the source's fields read first,
//   release the old block, adopt, add-ref).
class C3DGroup2;
#include "engine/CCountedPointer.h"
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(CCountedPointer<C3DGroup2>*, CCountedPointer<C3DGroup2>*, const CCountedPointer<C3DGroup2>&);
}