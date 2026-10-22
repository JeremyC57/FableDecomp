#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionGuardReactToBribe_Methods : CAIStateGroup_MinionGuardReactToBribe {
    void M(int world, int creature);
};
void CAIStateGroup_MinionGuardReactToBribe_Methods::M(int world, int creature){ this->FrameLastReactedToReceiveing = 0x80000000; }