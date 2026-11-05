// Leaf: ?InitStateGroup@CAIStateGroup_MinionReactToCrime_Methods@@QAEXABVCWorld@@AAVCThingAICreature@@@Z
#include "engine/CAIStateGroup_MinionReactToCrime.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld; class CThingAICreature;
struct CAIStateGroup_MinionReactToCrime_Methods : CAIStateGroup_MinionReactToCrime {
    void InitStateGroup(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_MinionReactToCrime_Methods::InitStateGroup(const CWorld&, CThingAICreature&){ GuardTold=0; OpinionDeedHandle=0; AllowDuplicateReactToCrime=0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_MinionReactToCrime@@UAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
