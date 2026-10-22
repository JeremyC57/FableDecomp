#include "engine/CAIStateGroup_BalverineCornered.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BalverineCornered_Methods : CAIStateGroup_BalverineCornered {
    void M(int world, int creature);
};
void CAIStateGroup_BalverineCornered_Methods::M(int world, int creature){ this->TimeOfLastJump = 0u; }