// CCountedPointer<CCombatSequenceBase>::CCountedPointer(CCombatSequenceBase* data) -- adopt the object: store it and, when non-null, a new 12-byte count block {1, DeleteData, object}
// Leaf: ??0?$CCountedPointer@VCCombatSequenceBase@@@@QAE@PAVCCombatSequenceBase@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EF6F0, 78 bytes, `ret 4` (operator new 0x00BFEA1A, deleter 0x006EE3D0).
// Name-fix 2026-11-12 (was the stand-in CCountedPointer_006ee3d0::CCountedPointer_006ee3d0(void*) with a local SCountBlock):
//   the landed CCombatSequenceGeneric::OnCreateSpecial 0x008B0B60 calls it at 0x008B0BBC on the result of the PDB-typed creator
//   CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&) and binds this address under this symbol;
//   FableWin.pdb carries the CCountedPointer<CCombatSequenceBase> instantiation (?ShareData@?$CCountedPointer@VCCombatSequenceBase@@@@).
//   Body: the shared engine/CCountedPointer.h constructor, explicitly instantiated as the landed twin 0x00695880
//   (CCountedPointer<CCreatureActionBase>) does.
#include "engine/CCountedPointer.h"
class CCombatSequenceBase;
template CCountedPointer<CCombatSequenceBase>::CCountedPointer(CCombatSequenceBase*);
