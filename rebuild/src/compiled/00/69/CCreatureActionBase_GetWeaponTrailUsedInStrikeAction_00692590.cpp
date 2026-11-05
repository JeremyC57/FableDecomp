// Leaf: ?GetWeaponTrailUsedInStrikeAction@CCreatureActionBase_Methods@@QAEHH@Z
#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureActionBase_Methods : CCreatureActionBase {
    int GetWeaponTrailUsedInStrikeAction(int a0);
};
int CCreatureActionBase_Methods::GetWeaponTrailUsedInStrikeAction(int a0) { return 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetWeaponTrailUsedInStrikeAction@CCreatureActionBase@@UBEPAVCTCWeaponTrail@@AAJ@Z); evidence: RTTI vtable slot + PDB name set.
