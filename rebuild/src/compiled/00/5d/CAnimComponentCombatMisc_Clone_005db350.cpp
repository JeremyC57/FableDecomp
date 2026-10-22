#include "engine/CAnimComponentCombatMisc.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentCombatMisc* (__fastcall* CloneCombatMiscFn)(CAnimComponentCombatMisc*);
void* __fastcall Clone_CombatMisc(CAnimComponentCombatMisc* component)
{
    CloneCombatMiscFn* vtable = (CloneCombatMiscFn*)component->__vftable;
    CAnimComponentCombatMisc* copy = vtable[0x10 / 4](component);
    copy->MeleeFlourish = component->MeleeFlourish;
    copy->MeleeKnockdown = component->MeleeKnockdown;
    return copy;
}
