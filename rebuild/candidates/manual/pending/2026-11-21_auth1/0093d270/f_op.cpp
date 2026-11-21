// CAIStateGroup_MeleeCombatNearbyBase::IsRunnable() -- Ego_d-text baseline
// Leaf: ?IsRunnable@CAIStateGroup_MeleeCombatNearbyBase@@MAE_NXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_ASSERTED_BODY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CCombatManager.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyBase.h"

bool CAIStateGroup_MeleeCombatNearbyBase::IsRunnable()
{
    const CTCTargetingBase* ptc_targeting;
    GetCreature().PeekTC(&ptc_targeting);
    CThingCreatureBase* pcreature_base = ptc_targeting->GetPBestEnemyCreatureForAttack();
    if (IS_THING_ALIVE(pcreature_base))
    {
        PEnemy = pcreature_base;
        if (IsStillRunnable(false))
        {
            CTCCreatureNavigation* ptc_nav;
            GetCreature().GetTC(&ptc_nav);
            if (ptc_nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(), PEnemy->GetPos(), GetCreature().GetCombatManager().GetProximityForNavigation()))
                return true;
        }
    }
    return false;
}
