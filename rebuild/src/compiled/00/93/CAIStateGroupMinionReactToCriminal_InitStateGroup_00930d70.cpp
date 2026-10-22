#include "engine/CAIStateGroup_MinionReactToCriminal.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionReactToCriminal_Methods : CAIStateGroup_MinionReactToCriminal {
    void M(int world, int creature);
};
void CAIStateGroup_MinionReactToCriminal_Methods::M(int world, int creature){ this->FrameLastLeftStategroup = 0u; this->OverrodeLocking = 0; this->HeroInsideHouse = 0; this->MeInsideHouse = 0; }