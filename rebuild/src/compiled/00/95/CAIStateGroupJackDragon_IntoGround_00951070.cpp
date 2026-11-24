// Build the dragon's landing route and transition into the grounded state.
// Leaf: ?IntoGround@CAIStateGroup_JackDragon@@AAEXXZ
// Flags:
// Inline-budget: vector<C3DVector>::_M_insert_overflow 0x0066E370 expands its fill and calls the fastcall tail copy 0x0066DDF0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_CREATEIBC_PASS26
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_INTOGROUND
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_INTOGROUND_PASS27
#define FABLE_RETAIL_CCREATUREACTION_DRAGONLAND_PASS27
#define FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_DragonLand.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CJackDragonDef.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CWorld.h"
#include "engine/CTCBoss.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy))
        return false;
    if (IsStopping())
        return false;
    return true;
}

void CAIStateGroup_JackDragon::ResetTimers()
{
    FramesInSnapRange = 0;
    const long frame = GetWorld().GetFrame();
    FrameOfLastSlam = frame;
    FrameWhenLastInRange = frame;
}

void CAIStateGroup_JackDragon::IntoGround()
{
    CDefPointer<const CJackDragonDef> p_jack_dragon_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_dragon_def);
    C3DVector land_offset(0.0f, -37.930191f, 0.0f + 30.148378f);
    C3DVector start_offset(0.0f, -80.0f, 30.0f);
    if (IS_THING_ALIVE(PLandedMarker) && PLandedMarker->GetPhysicsTC()) {
        land_offset.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
        start_offset.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
    }
    float fly_speed = 30.0f;
    std::vector<C3DVector> movement_points;
    movement_points.push_back(GetCreature().GetPos());
    if (GoLeft)
        movement_points.push_back(DragonPosIntoGround2Left);
    else
        movement_points.push_back(DragonPosIntoGround2Right);
    movement_points.push_back(DragonPosIntoGround3);
    movement_points.push_back(PLandedMarker->GetPos() + land_offset +
        C3DVector(0.0f, 0.0f, p_jack_dragon_def->DragonGroundHeightOffsetBodge));
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);
    {
        CCountedPointer<CCreatureActionBase> p_land_action(new CCreatureAction_DragonLand(GetCreature(), *PEnemy));
        StartFlying(GetCreature(), movement_points, PreviousFlightPos, p_land_action);
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CreateIBC();
    ptc_boss->SetGenericCounter(0);
    ResetTimers();
    FrameWhenLanded = GetWorld().GetFrame();
    CurrentState = DRAGON_STATE_GROUND;
}