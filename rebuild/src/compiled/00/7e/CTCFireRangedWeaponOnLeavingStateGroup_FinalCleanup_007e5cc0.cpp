// CTCFireRangedWeaponOnLeavingStateGroup::FinalCleanup() -- unregister this component from its creature, then run the creature-side ranged-weapon cleanup
// Leaf: FinalCleanup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E5CC0, length 22 bytes (vtable 0x0123E464 slot 2, the CTCBase
// "FinalCleanup" slot).
// Callees: CThing::SetComponentRegistered(bool, CTCBase*) @ 0x00833A10 (inserts into / erases from the component set at
// +0x1cc of the creature and refreshes bit 1 of +0x1c8 from its emptiness; labelled "SetSuspendAnimation"),
// CleanupRangedWeapon(CThing*) @ 0x007E5B40 (the same TU's creature helper, manifest "Cleanup", `__fastcall`).
// Shape: ParentThing read once into esi (a local: it survives the first call), `push ecx` passes `this` as the
// component, `push 0` the bool; the helper is the last statement and becomes the tail `jmp` with the thing in ecx.
#include "engine/CTCBase.h"

class CThing { public:
    void SetComponentRegistered(bool registered, CTCBase* component);   // 0x00833A10
};

void __fastcall CleanupRangedWeapon(CThing* creature);                  // 0x007E5B40

class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase { public:
    void FinalCleanup();
};

void CTCFireRangedWeaponOnLeavingStateGroup::FinalCleanup()
{
    CThing* creature = ParentThing;
    creature->SetComponentRegistered(false, this);
    CleanupRangedWeapon(creature);
}