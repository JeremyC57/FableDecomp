// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `Knockdown`.
// Leaf: ?IsKnockdownStrike@CCreatureAction_BanditKingUnstuck_Methods@@QAEEXZ
#include "engine/CCreatureAction_BanditKingUnstuck.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_BanditKingUnstuck_Methods : CCreatureAction_BanditKingUnstuck {
    unsigned char IsKnockdownStrike();
};
unsigned char CCreatureAction_BanditKingUnstuck_Methods::IsKnockdownStrike(){ return this->Knockdown; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsKnockdownStrike@CCreatureAction_BanditKingUnstuck@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
