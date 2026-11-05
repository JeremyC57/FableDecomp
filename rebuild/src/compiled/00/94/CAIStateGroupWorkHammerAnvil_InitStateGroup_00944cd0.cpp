// Leaf: ?InitStateGroup@CAIStateGroup_WorkHammerAnvil_Methods@@QAEXABVCWorld@@AAVCThingAICreature@@@Z
#include "engine/CAIStateGroup_WorkHammerAnvil.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld; class CThingAICreature;
struct CAIStateGroup_WorkHammerAnvil_Methods : CAIStateGroup_WorkHammerAnvil {
    void InitStateGroup(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_WorkHammerAnvil_Methods::InitStateGroup(const CWorld&, CThingAICreature&){ ActionPointIndex=0; TookActionPoint=0; NextFrameHammer=0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_WorkHammerAnvil@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
