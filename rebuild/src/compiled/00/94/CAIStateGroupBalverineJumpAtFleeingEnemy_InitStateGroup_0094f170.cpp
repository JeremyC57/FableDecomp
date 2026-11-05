// Leaf: ?InitStateGroup@CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods@@QAEXHH@Z
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods : CAIStateGroup_BalverineJumpAtFleeingEnemy {
    void InitStateGroup(int world, int creature);
};
void CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods::InitStateGroup(int world, int creature){ this->WaitUntilWorldFrameForNextAttempt = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_BalverineJumpAtFleeingEnemy@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
