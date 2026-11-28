// Flags: 
// Leaf: ??0CGetBestIciclePointForTarget@?A0x9b671e55@@QAE@ABVCThing@@M@Z
// 008e2a90 ??0CGetBestIciclePointForTarget
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_ICICLE_ASSAULT_PASS30
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CSNOWTROLLDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_COMBATRING_ASSIGN_PASS27
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_HERO_APPEARANCE_UPDATE_PASS30
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/CTCTrapBase.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_IceTrollIcicleAssaultLoop.h"
#include "engine/CCreatureAction_IceTrollIcicleAssaultOutOf.h"
#include "engine/CreatureModes_IciclePoints.h"
#include "engine/NExplosiveTrailHelpers.h"
#include "engine/GFRandom.h"
#include "engine/GFLimit.h"
#include "stlport/fable_algorithm.h"
#include "stlport/position_algorithms.h"

#include "engine/CTCPhysicsBase.h"
namespace {
CGetBestIciclePointForTarget::CGetBestIciclePointForTarget(const CThing& target, float prediction_frames)
    : BestPosScore(-1.0f)
{
    const C3DVector& tpos = target.GetPos();
    C3DVector vel = tpos - target.GetPhysicsTC()->GetLastPosition();
    vel.Z = 0.0f;
    TargetPos = tpos + vel * prediction_frames;
}
}
