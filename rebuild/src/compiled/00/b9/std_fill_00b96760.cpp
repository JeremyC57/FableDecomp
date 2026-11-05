// std::fill(CCountedPointer<CUnprovenPointee_00b97cb0>* first, CCountedPointer<CUnprovenPointee_00b97cb0>* last, const CCountedPointer<CUnprovenPointee_00b97cb0>& value) -- assign value to every element
// Leaf: ??$fill@PAV?$CCountedPointer@VCUnprovenPointee_00b97cb0@@@@V1@@std@@YIXPAV?$CCountedPointer@VCUnprovenPointee_00b97cb0@@@@0ABV1@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b96760, 94 bytes, /Gr `__fastcall` (first ecx, last edx spilled, value on the stack), `ret 4`.
// Identity (descriptive): called by the fill insert 0x00B97E90, whose __uninitialized_copy 0x00B95F60 is bound only with the descriptive pointee CUnprovenPointee_00b97cb0 (reused here so the vector's helpers share one element spelling).
// Callee: operator delete 0x00BFE9BC (a released count block); CCountedPointer's operator= inline (the source's fields read first,
//   release the old block, adopt, add-ref).
class CUnprovenPointee_00b97cb0;
#include "engine/CCountedPointer.h"
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(CCountedPointer<CUnprovenPointee_00b97cb0>*, CCountedPointer<CUnprovenPointee_00b97cb0>*, const CCountedPointer<CUnprovenPointee_00b97cb0>&);
}