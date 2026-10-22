#include "engine/CAnimComponentCombatHandedness.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentCombatHandedness* (__fastcall* CloneHandednessFn)(CAnimComponentCombatHandedness*);
CAnimComponentCombatHandedness* __fastcall Clone_CombatHandedness(CAnimComponentCombatHandedness* component)
{
    CloneHandednessFn* vtable = (CloneHandednessFn*)component->__vftable;
    CAnimComponentCombatHandedness* copy = vtable[0x10 / 4](component);
    *(long*)copy->StartHandedness = *(long*)component->StartHandedness;
    *(long*)copy->EndHandedness = *(long*)component->EndHandedness;
    return copy;
}
