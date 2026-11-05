// Leaf: ?IsActionReadyForInterruption@CCreatureAction_ActivateQuest_Methods@@QAEEXZ
#include "engine/CCreatureAction_ActivateQuest.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_ActivateQuest_Methods : CCreatureAction_ActivateQuest {
    unsigned char IsActionReadyForInterruption();
};
unsigned char CCreatureAction_ActivateQuest_Methods::IsActionReadyForInterruption(){ return this->f; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsActionReadyForInterruption@CCreatureAction_ActivateQuest@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
