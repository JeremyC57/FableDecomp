// std::fill(CCountedPointer<CEngineSimulationBase>* first, CCountedPointer<CEngineSimulationBase>* last, const CCountedPointer<CEngineSimulationBase>& value) -- assign value to every element
// Leaf: ??$fill@PAV?$CCountedPointer@VCEngineSimulationBase@@@@V1@@std@@YIXPAV?$CCountedPointer@VCEngineSimulationBase@@@@0ABV1@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b82e30, 94 bytes, /Gr `__fastcall` (first ecx, last edx spilled, value on the stack), `ret 4`.
// Identity (proven): called by vector<CCountedPointer<CEngineSimulationBase> >::_M_fill_insert 0x00B83E70 (bound).
// Callee: operator delete 0x00BFE9BC (a released count block); CCountedPointer's operator= inline (the source's fields read first,
//   release the old block, adopt, add-ref).
class CEngineSimulationBase;
#include "engine/CCountedPointer.h"
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(CCountedPointer<CEngineSimulationBase>*, CCountedPointer<CEngineSimulationBase>*, const CCountedPointer<CEngineSimulationBase>&);
}