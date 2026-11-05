// Leaf: ?InitStateGroup@CAIStateGroup_WorkSawingWood_Methods@@QAEXHH@Z
#include "engine/CAIStateGroup_WorkSawingWood.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_WorkSawingWood_Methods : CAIStateGroup_WorkSawingWood {
    void InitStateGroup(int world, int creature);
};
void CAIStateGroup_WorkSawingWood_Methods::InitStateGroup(int world, int creature){ this->ActionPointIndex = 0u; this->TookActionPoint = 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_WorkSawingWood@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
