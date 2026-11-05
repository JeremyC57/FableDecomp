// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `FaceTarget`.
// Leaf: ?IsToFaceTarget@CCreatureAction_DragonFlight_Methods@@QAEEXZ
#include "engine/CCreatureAction_DragonFlight.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_DragonFlight_Methods : CCreatureAction_DragonFlight {
    unsigned char IsToFaceTarget();
};
unsigned char CCreatureAction_DragonFlight_Methods::IsToFaceTarget(){ return this->FaceTarget; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsToFaceTarget@CCreatureAction_DragonFlight@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
