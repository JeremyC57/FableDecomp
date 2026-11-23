// Leaf: ?InitStateGroup@CAIStateGroup_BalverineCornered@@MAEXABVCWorld@@AAVCThingAICreature@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CAIStateGroup_BalverineCornered.h under FABLE_RETAIL_CAISTATEGROUP_BALVERINECORNERED with the PDB parameter / return types (was ?InitStateGroup@CAIStateGroup_BalverineCornered_Methods@@QAEXHH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CAISTATEGROUP_BALVERINECORNERED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
class CWorld;               // the header names its InitStateGroup parameters without declaring them
class CThingAICreature;
#include "engine/CAIStateGroup_BalverineCornered.h"
void CAIStateGroup_BalverineCornered::InitStateGroup(const CWorld& world, CThingAICreature& creature) { this->TimeOfLastJump = 0u; }
