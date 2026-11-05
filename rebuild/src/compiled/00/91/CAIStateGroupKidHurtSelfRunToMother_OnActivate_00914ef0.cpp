// Leaf: ?OnActivate@CAIStateGroup_KidHurtSelfRunToMother_Methods@@QAEXXZ
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_KidHurtSelfRunToMother_Methods : CAIStateGroup_KidHurtSelfRunToMother {
    void OnActivate();
};
void CAIStateGroup_KidHurtSelfRunToMother_Methods::OnActivate(){ this->AddedSadMode = 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnActivate@CAIStateGroup_KidHurtSelfRunToMother@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
