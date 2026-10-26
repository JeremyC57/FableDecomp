#include "engine/CAIStateGroup_MinionReactToCrime.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld; class CThingAICreature;
struct CAIStateGroup_MinionReactToCrime_Methods : CAIStateGroup_MinionReactToCrime {
    void Init(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_MinionReactToCrime_Methods::Init(const CWorld&, CThingAICreature&){ GuardTold=0; OpinionDeedHandle=0; AllowDuplicateReactToCrime=0; }