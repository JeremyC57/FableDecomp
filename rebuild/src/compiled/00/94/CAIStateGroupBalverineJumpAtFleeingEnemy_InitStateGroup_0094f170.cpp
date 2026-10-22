#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods : CAIStateGroup_BalverineJumpAtFleeingEnemy {
    void M(int world, int creature);
};
void CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods::M(int world, int creature){ this->WaitUntilWorldFrameForNextAttempt = 0u; }