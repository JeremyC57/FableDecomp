// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `Blockable`.
// Leaf: ?IsBlockable@CCreatureAction_VillagerAttackKnockdown_Methods@@QAEEXZ
#include "engine/CCreatureAction_VillagerAttackKnockdown.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_VillagerAttackKnockdown_Methods : CCreatureAction_VillagerAttackKnockdown {
    unsigned char IsBlockable();
};
unsigned char CCreatureAction_VillagerAttackKnockdown_Methods::IsBlockable(){ return this->Blockable; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsBlockable@CCreatureAction_VillagerAttackKnockdown@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
