// CAIStateGroup_Drunk::Update
// Leaf: ?Update@CAIStateGroup_Drunk@@UAEXXZ
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CTHING_COMPONENT_QUERY_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUP_DRUNK_UPDATE_PASS27
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CTCDRUNKENNESS_DRANKFRAMES
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PEEKMARKERS_PASS27
#define FABLE_RETAIL_CTCEXCLUSIVEUSE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_PASS24
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingMarker.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CAIStateGroup_Drunk.h"
#include "engine/CAIBrain.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CThingFilter_IsNotBeingExclusivelyUsed.h"
#include "engine/CTCDrunkenness.h"
#include "engine/CTCExclusiveUse.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"


// Inline-budget: the native village marker loop calls this instantiation out of line.
template<> __declspec(noinline) inline bool FABLE_FASTCALL IS_THING_ALIVE(const CIntelligentPointer<CThingMarker>& thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

// Inline-budget: retain the original out-of-line marker predicate and component query.
__declspec(noinline) inline bool CThingFilter_IsNotBeingExclusivelyUsed::operator()(const CThing& thing) const
{
    const CTCExclusiveUse* exclusive;
    return thing.PeekTC(&exclusive) && exclusive->GetUser() == 0;
}

void CAIStateGroup_Drunk::Update()
{
    CTCCreatureNavigation* pnav;
    CTCDrunkenness* tc_drunkenness;
    EAIActionReturn ret;
    float drunkenness;
    C3DVector wander_pos;

    if (!(GetCreature().GetTC(&tc_drunkenness) && tc_drunkenness->IsDrunk())) {
        Terminate();
        return;
    }
    drunkenness = tc_drunkenness->GetDrunkenness();

    if (GFFloatRandom(1.0f,GetPWorldSeed()) < 0.15f) {
        CIntelligentPointer<CThing> best;
        float best_dist = 0.0f;
        long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_DISCHARGE_BODILY_FLUIDS");
        CTCVillageMember* tc_village_member;
        if (GetCreature().GetTC(&tc_village_member) && tc_village_member->GetPVillage()) {
            const std::list<CIntelligentPointer<CThingMarker> >& village_markers = tc_village_member->GetPVillage()->PeekVillageMarkers();
            for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator i=village_markers.begin(); i!=village_markers.end(); ++i) {
                if (IS_THING_ALIVE(*i) && i->Get()->GetDefGlobalIndex()==def_index && CThingFilter_IsNotBeingExclusivelyUsed()(**i)) {
                    float dist = (i->Get()->GetPos()-GetCreature().GetPos()).GetSquaredMagnitude();
                    if (!IS_THING_ALIVE(best) || dist < best_dist) {
                        best = i->Get();
                        best_dist = dist;
                    }
                }
            }
        }
        if (IS_THING_ALIVE(best)) {
            CTCExclusiveUse* exclusive;
            if (IS_THING_ALIVE(best) && best->GetTC(&exclusive))
                exclusive->SetUser(&GetCreature());
            EAIActionReturn ret;
            do {
                bool nav_route_calculated;
                TalkToSelfUpdate();
                ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,best->GetPos(),nav_route_calculated,
                    AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
            } while (ret==AI_ACTION_IN_PROGRESS);

            ret = AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,
                best->GetPhysicsTC()->GetRHSet().GetForward()+best->GetPos(),17,0.0027777f);
            if (GFRandom(2,GetPWorldSeed())==0)
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                    "ST_PISSING",CSoundPair(0),false,true,false,0,0),false);
            else
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                    "ST_PUKING",CSoundPair(0),false,true,false,0,0),false);
            if (IS_THING_ALIVE(best) && best->GetTC(&exclusive))
                exclusive->SetUser(0);
            if (ret!=AI_ACTION_FINISHED) {
                Terminate();
                return;
            }
            drunkenness -= 0.6f;
        }
    }

    if (drunkenness<0.0f)
        drunkenness=0.0f;
    tc_drunkenness->SetDrunkenness(drunkenness);
    if (!tc_drunkenness->IsDrunk()) {
        Terminate();
        return;
    }
    ret=AI_ACTION_FAILED;
    if (!GetCreature().GetTC(&pnav))
        FABLE_RETAIL_ASSERT(false);
    if (pnav->GetNearbyWanderPosition(GetCreature().PeekPos(),5.0f,wander_pos,true)) {
        bool nav_route_calculated=false;
        do {
            TalkToSelfUpdate();
            ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,wander_pos,nav_route_calculated,
                AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
        } while (ret==AI_ACTION_IN_PROGRESS);
        if (ret!=AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        if (GFRandom(100,GetPWorldSeed())<50) {
            unsigned long no_of_idles=GFRandom(2,GetPWorldSeed());
            if (no_of_idles!=0) {
                for (long c1=0;c1<no_of_idles;++c1) {
                    ret=AI_ACTION_FAILED;
                    do {
                        TalkToSelfUpdate();
                        ret = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            "ST_IDLE",0.0f,CSoundPair(0),false,true,false,0,0),false);
                    } while (ret==AI_ACTION_IN_PROGRESS);
                    if (ret!=AI_ACTION_FINISHED) {
                        Terminate();
                        return;
                    }
                }
            }
        }
    }
    Terminate();
}