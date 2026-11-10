// CDragon::OnPredicateFail -- mark the parent dead when the kill predicate succeeds.
// Leaf: ?OnPredicateFail@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ
// Retail: 0x00d259a0, 64 bytes, thiscall.
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DRAGON_ORIGINAL_TYPES
#define FABLE_RETAIL_DRAGON_STATE_BODY
#include "engine/fable_script_def_global.h"
#include "engine/CQ_DragonBossFightScript.h"
void NScript::CQ_DragonBossFightScript::SetDragonState(EDragonState dragonState) {
        DragonState=dragonState;
        switch(dragonState) {
        case DS_HEALTH_HIGH:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsHighHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersHighHealth;
            break;
        case DS_HEALTH_MEDIUM:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsMediumHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersMediumHealth;
            break;
        case DS_HEALTH_LOW:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsLowHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersLowHealth;
            break;
        case DS_HEALTH_VERY_LOW:
            if(IsActiveThreadTerminating()) return;
            TargetNumMinions=pGlobalScriptDef->DBNumMinionsVeryLowHealth;
            TargetNumSummoners=pGlobalScriptDef->DBNumSummonersVeryLowHealth;
            break;
        default: break;
        }
    }
void NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail() {
    if(Me.MsgIsKilledBy("")) ParentClass.SetDragonState(DS_DEAD);
}