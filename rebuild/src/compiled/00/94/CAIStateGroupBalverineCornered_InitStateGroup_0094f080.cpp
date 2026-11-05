// Leaf: ?InitStateGroup@CAIStateGroup_BalverineCornered_Methods@@QAEXHH@Z
#include "engine/CAIStateGroup_BalverineCornered.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BalverineCornered_Methods : CAIStateGroup_BalverineCornered {
    void InitStateGroup(int world, int creature);
};
void CAIStateGroup_BalverineCornered_Methods::InitStateGroup(int world, int creature){ this->TimeOfLastJump = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_BalverineCornered@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
