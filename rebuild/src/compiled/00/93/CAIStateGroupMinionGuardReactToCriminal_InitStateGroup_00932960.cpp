// Leaf: ?InitStateGroup@CAIStateGroup_MinionGuardReactToCriminal@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CAIStateGroup_MinionGuardReactToCriminal.h under FABLE_RETAIL_CAISTATEGROUP_MINIONGUARDREACTTOCRIMINAL_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CAIStateGroup_MinionGuardReactToCriminal_Methods::InitStateGroup); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONGUARDREACTTOCRIMINAL_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionGuardReactToCriminal.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld;
class CThingAICreature;

void CAIStateGroup_MinionGuardReactToCriminal::InitStateGroup(const CWorld& world, CThingAICreature& creature) {
    this->WarningsGiven = 0u;
    this->FrameWarningStarted = 0xe0000000u;
    this->DeedLastWarnedAbout = 0xffffffffu;
}
