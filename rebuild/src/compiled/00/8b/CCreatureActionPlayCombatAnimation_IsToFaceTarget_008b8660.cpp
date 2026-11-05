// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `FaceTarget`.
// Leaf: ?IsToFaceTarget@CCreatureAction_PlayCombatAnimation_Methods@@QAEEXZ
#include "engine/CCreatureAction_PlayCombatAnimation.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_PlayCombatAnimation_Methods : CCreatureAction_PlayCombatAnimation {
    unsigned char IsToFaceTarget();
};
unsigned char CCreatureAction_PlayCombatAnimation_Methods::IsToFaceTarget(){ return this->FaceTarget; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsToFaceTarget@CCreatureAction_PlayCombatAnimation@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
