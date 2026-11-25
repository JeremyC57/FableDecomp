// CTCFireRangedWeaponOnLeavingStateGroup::FinalCleanup() -- unregister this component from its creature, then run the creature-side ranged-weapon cleanup
// Leaf: FinalCleanup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E5CC0, length 22 bytes (vtable 0x0123E464 slot 2, the CTCBase
// "FinalCleanup" slot).
// Callees: CThingAICreature::SuspendAI(bool, CTCBase*) @ 0x00833A10 (inserts into / erases from the component set at
// +0x1cc of the creature and refreshes bit 1 of +0x1c8 from its emptiness; labelled "SetSuspendAnimation"),
// CleanupRangedWeapon(CThing*) @ 0x007E5B40 (the same TU's creature helper, manifest "Cleanup", `__fastcall`).
// Shape: ParentThing read once into esi (a local: it survives the first call), `push ecx` passes `this` as the
// component, `push 0` the bool; the helper is the last statement and becomes the tail `jmp` with the thing in ecx.
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
#include "engine/CTCBase.h"

#include "engine/CThing.h"


// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x007E5B40 ?Cleanup@CTCFireRangedWeaponOnLeavingStateGroup@@SIXAAVCThingAICreature@@@Z (was ?CleanupRangedWeapon@@YIXPAVCThing@@@Z)
class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase { public:
    void FinalCleanup();
 public: static void __fastcall Cleanup(CThingAICreature&); };

void CTCFireRangedWeaponOnLeavingStateGroup::FinalCleanup()
{
    CThing* creature = ParentThing;
    ((CThingAICreature*)creature)->SuspendAI(false, this);
    CTCFireRangedWeaponOnLeavingStateGroup::Cleanup(*reinterpret_cast<CThingAICreature*>(creature));
}