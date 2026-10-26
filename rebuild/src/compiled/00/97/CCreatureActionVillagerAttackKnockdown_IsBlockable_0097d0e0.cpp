// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `Blockable`.
#include "engine/CCreatureAction_VillagerAttackKnockdown.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_VillagerAttackKnockdown_Methods : CCreatureAction_VillagerAttackKnockdown {
    unsigned char M();
};
unsigned char CCreatureAction_VillagerAttackKnockdown_Methods::M(){ return this->Blockable; }