// CAIStateGroup_NymphAttack::Update()
// Leaf: ?Update@CAIStateGroup_NymphAttack@@UAEXXZ
// Flags: default
#define FABLE_RETAIL_NYMPH_ATTACK_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_NymphAttack.h"
#include "engine/CAIBrain.h"
#include "engine/CTCNymph.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/CCreatureAction_NymphSummonInto.h"
#include "engine/CCreatureAction_NymphSummonOutOf.h"
#include "engine/CCreatureAction_SummonCreatures.h"
#include "engine/CCreatureAction_NymphIdleAfterShot.h"
#include "engine/CCreatureAction_NymphShot.h"

void CAIStateGroup_NymphAttack::Update()
{
    CTCNymph* pTCNymph;
    GetCreature().GetTC(&pTCNymph);
    CIntelligentPointer<CThingCreatureBase> pTarget(pTCNymph->GetTarget());
    if (IS_THING_ALIVE(pTarget)) {
        if (pTCNymph->GetShouldSummon() && IsTargetInLineOfSight(pTarget.Get())) {
            if (AIAction_CombatPerformActionAndWait(CCreatureAction_NymphSummonInto(GetCreature(), *pTarget), false) == AI_ACTION_FINISHED) {
                if (IS_THING_ALIVE(pTarget) && !pTCNymph->HasBeenHit()
                    && AIAction_CombatPerformActionAndWait(CCreatureAction_SummonCreatures(GetCreature(), *pTarget, false), false) == AI_ACTION_FINISHED) {
                    if (!pTCNymph->HasBeenHit())
                        AIAction_CombatPerformActionAndWait(CCreatureAction_NymphSummonOutOf(GetCreature()), false);
                }
            }
        } else {
            long loopCounter = 0;
            const long shotLoops = pTCNymph->GetNymphDef().ShotsBeforeIdle;
            const long idleFrames = (long)(CMainGameComponent::GetConstantFPS() * pTCNymph->GetNymphDef().IdleTimeBetweenShots);
            long lastShotFrame = 0;
            pTCNymph->SetIsReadyToSummon(true);
            while (IS_THING_ALIVE(pTarget) && pTCNymph->IsDecentAttackRange(pTarget->GetPos())) {
                if (CWorld::GetFrame() < lastShotFrame + idleFrames || (shotLoops > 0 && ++loopCounter > shotLoops)) {
                    loopCounter = 0;
                    if (AIAction_CombatPerformActionAndWait(CCreatureAction_NymphIdleAfterShot(GetCreature(), *pTarget), false) != AI_ACTION_FINISHED)
                        break;
                } else {
                    if (!IsTargetInLineOfSight(pTarget.Get()))
                        break;
                    if (AIAction_CombatPerformActionAndWait(CCreatureAction_NymphShot(GetCreature(), *pTarget), false) != AI_ACTION_FINISHED)
                        break;
                    lastShotFrame = CWorld::GetFrame();
                }
                if (pTCNymph->HasBeenHit())
                    break;
            }
        }
        pTCNymph->ClearReadyToAttackFlags();
    }
    Terminate();
}