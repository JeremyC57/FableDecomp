// Leaf: ?InitStateGroup@CAIStateGroup_MinionGuardReactToBribe_Methods@@QAEXHH@Z
#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionGuardReactToBribe_Methods : CAIStateGroup_MinionGuardReactToBribe {
    void InitStateGroup(int world, int creature);
};
void CAIStateGroup_MinionGuardReactToBribe_Methods::InitStateGroup(int world, int creature){ this->FrameLastReactedToReceiveing = 0x80000000; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_MinionGuardReactToBribe@@UAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
