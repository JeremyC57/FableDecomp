// Leaf: ??0CCreatureAction_SummonerLightningOrbOutOfGroundSmash@@QAE@AAVCThingCreatureBase@@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_SummonerLightningOrbOutOf.h"

CCreatureAction_SummonerLightningOrbOutOfGroundSmash::CCreatureAction_SummonerLightningOrbOutOfGroundSmash(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 150, CSoundPair())
{
}
