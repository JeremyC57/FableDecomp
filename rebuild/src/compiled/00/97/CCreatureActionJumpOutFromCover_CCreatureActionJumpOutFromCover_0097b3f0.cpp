// Leaf: ??0CCreatureAction_JumpOutFromCover@@QAE@AAVCThingCreatureBase@@0ABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_JUMP_OUT_FROM_COVER_OWNER_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_MOVE_FROM_COVER_PASS30
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_JumpOutFromCover.h"

CCreatureAction_JumpOutFromCover::CCreatureAction_JumpOutFromCover(CThingCreatureBase& creature, CThingCreatureBase& target, const C3DVector& direction)
        : CCombatActionBase(creature, target, 150, GetSoundPair(creature, "SWORD_SWING")), JumpDirection(direction) {}
