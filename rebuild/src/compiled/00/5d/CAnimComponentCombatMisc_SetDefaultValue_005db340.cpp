// Clear the two byte flags at this+0x8 / this+0x9. __fastcall this=ecx.
// Leaf: ?SetDefaultValue@CAnimComponentCombatMisc_Methods@@QAEXXZ
#include "engine/CAnimComponentCombatMisc.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CAnimComponentCombatMisc_Methods : CAnimComponentCombatMisc {
    void SetDefaultValue();
};
#pragma pack(pop)
void CAnimComponentCombatMisc_Methods::SetDefaultValue() { this->MeleeFlourish = false; this->MeleeKnockdown = false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?SetDefaultValue@CAnimComponentCombatMisc@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
