#include "engine/CAIStateGroup_Drunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_Drunk_Methods : CAIStateGroup_Drunk {
    void M(int world, int creature);
};
void CAIStateGroup_Drunk_Methods::M(int world, int creature){ this->FrameToNextTalkToSelf = 0u; }