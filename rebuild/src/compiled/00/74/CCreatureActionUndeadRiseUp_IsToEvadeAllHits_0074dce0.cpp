// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `StillUnderground`.
// Leaf: ?IsToEvadeAllHits@CCreatureAction_UndeadRiseUp_Methods@@QAEEXZ
#include "engine/CCreatureAction_UndeadRiseUp.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_UndeadRiseUp_Methods : CCreatureAction_UndeadRiseUp {
    unsigned char IsToEvadeAllHits();
};
unsigned char CCreatureAction_UndeadRiseUp_Methods::IsToEvadeAllHits(){ return this->StillUnderground; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsToEvadeAllHits@CCreatureAction_UndeadRiseUp@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
