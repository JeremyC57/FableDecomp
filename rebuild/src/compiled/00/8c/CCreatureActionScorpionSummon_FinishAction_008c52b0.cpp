// Leaf: ?FinishAction@CCreatureAction_ScorpionSummon@@UAEXXZ
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_RTTI_DECLS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CLASS
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ScorpionSummon.h"
extern void __fastcall dv_h1_008c4fd0(void*);
extern void __fastcall dv_h2_00859a20(void*);
void CCreatureAction_ScorpionSummon::FinishAction()
{
    void* action = (void*)this; dv_h1_008c4fd0(action); dv_h2_00859a20(action); }
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_ScorpionSummon_FinishAction -> CCreatureAction_ScorpionSummon::FinishAction, the RTTI-placed PDB member; class model: engine/CCreatureAction_ScorpionSummon.h opt-in FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CLASS, FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_RTTI_DECLS (preamble of NCreatureModeCModeScorpionKingVulnerable_FrameUpdate_008e0a10.cpp).
