// Leaf: ?Update@CAIStateGroup_ScreamerPursueTarget@@UAEXXZ
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_NSTEALTH_EDANGERSTATUS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SCREAMER_VTABLE_PASS27
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_ScreamerPursueTarget.h"
#include "engine/CAIBrain.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAdaptThingCompareToSTLComparitor.h"
#include "engine/CCreatureAction_ScreamerIntoAggressive.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CTCStealth.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CFilter_Bind2.h"
#include "engine/NCreatureMode.h"
#include "engine/GFSqr.h"
#include "engine/GFRandom.h"

#include "engine/EAIAction.h"
#include "engine/EMovementType.h"

void CAIStateGroup_ScreamerPursueTarget::Update()
{
    CTCCreatureModeManager* ptc_cmm;
    if (!GetCreature().GetTC(&ptc_cmm)) {
        FABLE_RETAIL_ASSERT(false);
    }
    CTCTargetingAI* pTCTarget;
    if (!GetCreature().GetTC(&pTCTarget)) {
        FABLE_RETAIL_ASSERT(false);
    }

    std::vector<CThing*> targets;
    targets.push_back(GetPlayerManager().GetMainPlayer().GetPControlledCreature());
    if (targets.empty()) {
        Terminate();
        return;
    }
    std::sort(targets.begin(), targets.end(),
        AdaptThingCompareToSTLComparitor(CThingCompare_Nearest(GetCreature().GetPos())));
    PTarget = CIntelligentPointer<CThing>(targets.front());
    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(PTarget));
    CTCStealth* ptc_stealth;
    if (PTarget->GetTC(&ptc_stealth))
        ptc_stealth->SetAsInDanger(GetCreature(),NStealth::DANGER_STATUS_CHASING);

    TickToGetAggressive = 0;
    bool quit_out = false;
    bool nav;
    EAIActionReturn ret;
    do {
        if (IsStopping() || !IS_THING_ALIVE(PTarget)) {
            quit_out = true;
        } else {
            bool getAggressive = false;
            if (pTCTarget->GetPBestEnemyCreatureForAttack() == PTarget.Get()) {
                C3DVector deltaToTarg = PTarget->GetPos() - GetCreature().GetPos();
                if (deltaToTarg.GetSquaredMagnitude() < GFSqr(GetCreature().GetCombatTypeDef().FrontFarLimit))
                    quit_out = true;
            }
            if (TickToGetAggressive == 0) {
                if (!ptc_cmm->IsModeActive(NCreatureMode::MODE_SCREAMER_AGGRESSIVE)) {
                    const CMessageEvent* pevent = GetWorld().GetMessageEventManager().FindMostRecentMessage(
                        GFFilter_Bind2(CMessageEventFilter_IsOfType(MESSAGE_EVENT_YELL_I_SEE_HIM),
                            CMessageEventFilter_IsTargettedAtThing(*PTarget)));
                    if (pevent) {
                        C3DVector delta = PTarget->GetPos() - GetCreature().GetPos();
                        float dist = delta.GetMagnitude2D();
                        TickToGetAggressive = GetWorld().GetFrame() + 5;
                        TickToGetAggressive += GFRandom(20,GetWorld().GetWorldSeed());
                        TickToGetAggressive += static_cast<long>(dist * 3.0f);
                    }
                }
            } else if (GetWorld().GetFrame() >= TickToGetAggressive) {
                getAggressive = !ptc_cmm->IsModeActive(NCreatureMode::MODE_SCREAMER_AGGRESSIVE);
            }
            if (getAggressive) {
                ret = AIAction_PerformActionAndWait(CCreatureAction_ScreamerIntoAggressive(GetCreature(),*PTarget),false);
                if (ret != AI_ACTION_FINISHED)
                    quit_out = true;
            } else {
                ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTarget,GetCreature().GetCombatTypeDef().PreferredCombatDistance,
                    nav,AI_MOVEMENT_WALKING,true,true,true,true,0.0f);
                if (ret != AI_ACTION_IN_PROGRESS)
                    quit_out = true;
            }
        }
    } while (!quit_out);
    Terminate();
}