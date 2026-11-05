// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `FaceTargetAtStart`.
// Leaf: ?IsToFaceTargetAtStart@CCreatureAction_BlockRespond_Methods@@QAEEXZ
#include "engine/CCreatureAction_BlockRespond.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_BlockRespond_Methods : CCreatureAction_BlockRespond {
    unsigned char IsToFaceTargetAtStart();
};
unsigned char CCreatureAction_BlockRespond_Methods::IsToFaceTargetAtStart(){ return this->FaceTargetAtStart; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsToFaceTargetAtStart@CCreatureAction_BlockRespond@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
