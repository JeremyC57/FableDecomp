// 00865eb0 CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise
// Leaf: ?Initialise@CCombatAction_GenericStrikeResponseKnockdownRecoverBase@@UAEXXZ
// Evidence: donor0281d300 and native virtual slot12; original const-reference animation name temporary.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING
#define FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER
#define FABLE_RETAIL_CCOMBATACTION_KNOCKDOWNRECOVER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_CANIMCOMPONENTFOLLOWINGONANIM_NAME
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentFollowingOnAnim.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdownRecoverBase.h"
void CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise()
{
    CAnimationEntry* entry=GetPPreceedingAnimationEntry().Get();
    bool found=false;
    if(entry){
        const CAnimComponentFollowingOnAnim* data;
        entry->GetComponent(&data);
        const CCharString& name=data->GetNextAnimName();
        if(name!=""){
            SetCombatAnimation(name);
            found=true;
        }
    }
    if(!found){
        SetCombatAnimation("RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER");
    }
    CCombatActionBase::Initialise();
}