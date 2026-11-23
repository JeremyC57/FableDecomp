// CAIStateGroup_MinionReactToCriminal::InitStateGroup -- resets the per-run state (frame, locking and both inside-house flags)
// Leaf: ?InitStateGroup@CAIStateGroup_MinionReactToCriminal@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CAIStateGroup_MinionReactToCriminal.h (class-model switch
//   FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL, landed definer 0x00931200) with the PDB parameter types (was ?InitStateGroup@CAIStateGroup_MinionReactToCriminal_Methods@@QAEXHH@Z); the override is
//   declared by the add-only opt-in FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL_INITSTATEGROUP_DECL (this batch's hdr/, equiv_check:
//   0 differing landed rows). The retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL_INITSTATEGROUP_DECL
class CWorld;
class CThingAICreature;
#include "engine/CAIStateGroup_MinionReactToCriminal.h"
void CAIStateGroup_MinionReactToCriminal::InitStateGroup(const CWorld& world, CThingAICreature& creature){ this->FrameLastLeftStategroup = 0u; this->OverrodeLocking = 0; this->HeroInsideHouse = 0; this->MeInsideHouse = 0; }
