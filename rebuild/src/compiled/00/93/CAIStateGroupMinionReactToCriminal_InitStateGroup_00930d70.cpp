// Leaf: ?InitStateGroup@CAIStateGroup_MinionReactToCriminal_Methods@@QAEXHH@Z
#include "engine/CAIStateGroup_MinionReactToCriminal.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionReactToCriminal_Methods : CAIStateGroup_MinionReactToCriminal {
    void InitStateGroup(int world, int creature);
};
void CAIStateGroup_MinionReactToCriminal_Methods::InitStateGroup(int world, int creature){ this->FrameLastLeftStategroup = 0u; this->OverrodeLocking = 0; this->HeroInsideHouse = 0; this->MeInsideHouse = 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_MinionReactToCriminal@@UAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
