// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f_2` -> `UsePhysics`.
// Leaf: ?IsMovementUsingPhysics@CCreatureAction_PlayAnimation_Methods@@QAEEXZ
#include "engine/CCreatureAction_PlayAnimation.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_PlayAnimation_Methods : CCreatureAction_PlayAnimation {
    unsigned char IsMovementUsingPhysics();
};
unsigned char CCreatureAction_PlayAnimation_Methods::IsMovementUsingPhysics(){ return this->UsePhysics; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsMovementUsingPhysics@CCreatureAction_PlayAnimation@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
