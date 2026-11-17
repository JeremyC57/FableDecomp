// CCountedPointer<CAnimFilterBase>::Reset(CAnimFilterBase* data) -- drop the shared count block (destroying the filter on last reference), then adopt the new one (CThingDeadCreature::PAnimFilter)
// Leaf: ?Reset@?$CCountedPointer@VCAnimFilterBase@@@@QAEXPAVCAnimFilterBase@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836580, 103 bytes, thiscall `ret 4`.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; ...
// Identity: Ego_r.pdb CThingDeadCreature::PAnimFilter is CCountedPointer<CAnimFilterBase> (+0xD0); its two retail callers are
//   CThingDeadCreature::SetAnimationFilter 0x00835AF0 (`PAnimFilter.Reset(filter.Clone())`, Reset called on this+0xD0) and the copy of
//   that body Create 0x00835C80 expands (NF80: engine/CThingDeadCreature.h calls it as engine/CCountedPointer.h Reset(T*)). The catalog's
//   CCountedPointer_008364b0 (named after the per-type deleter 0x008364B0) was made up.
// NF82 (q11004): re-authored as the explicit instantiation of engine/CCountedPointer.h Reset(T*) (the landed 0x004BB200 recipe: the same
//   103-byte body) in place of the row-local count-block / counted-pointer stand-ins.
#include "engine/CAnimFilterBase.h"
#include "engine/CCountedPointer.h"
template void CCountedPointer<CAnimFilterBase>::Reset(CAnimFilterBase*);
