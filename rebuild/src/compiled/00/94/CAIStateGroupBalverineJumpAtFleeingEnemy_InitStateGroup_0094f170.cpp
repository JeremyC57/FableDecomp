// Leaf: ?InitStateGroup@CAIStateGroup_BalverineJumpAtFleeingEnemy@@MAEXABVCWorld@@AAVCThingAICreature@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h under FABLE_RETAIL_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY with the PDB parameter / return types (was ?InitStateGroup@CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods@@QAEXHH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
class CWorld;               // the header names its InitStateGroup parameters without declaring them
class CThingAICreature;
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"
void CAIStateGroup_BalverineJumpAtFleeingEnemy::InitStateGroup(const CWorld& world, CThingAICreature& creature) { this->WaitUntilWorldFrameForNextAttempt = 0u; }
