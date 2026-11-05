// Leaf: ?GetCombatAbilityDataType@CCombatAbilityBlockingData_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CCombatAbilityBlockingData.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatAbilityBlockingData_Methods : CCombatAbilityBlockingData {
    int GetCombatAbilityDataType();
};
int CCombatAbilityBlockingData_Methods::GetCombatAbilityDataType() { return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetCombatAbilityDataType@CCombatAbilityBlockingData@@UBE?AW4ECombatAbilityDataType@@XZ); evidence: RTTI vtable slot + PDB name set.
