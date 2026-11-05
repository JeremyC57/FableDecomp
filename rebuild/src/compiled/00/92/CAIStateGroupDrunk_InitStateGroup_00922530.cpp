// Leaf: ?InitStateGroup@CAIStateGroup_Drunk_Methods@@QAEXHH@Z
#include "engine/CAIStateGroup_Drunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_Drunk_Methods : CAIStateGroup_Drunk {
    void InitStateGroup(int world, int creature);
};
void CAIStateGroup_Drunk_Methods::InitStateGroup(int world, int creature){ this->FrameToNextTalkToSelf = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_Drunk@@UAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
