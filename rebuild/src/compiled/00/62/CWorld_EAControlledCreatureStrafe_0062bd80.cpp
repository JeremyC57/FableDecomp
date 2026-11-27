// 0062bd80 CWorld::EAControlledCreatureStrafe
// Flags: /GS
// Leaf: ?EAControlledCreatureStrafe@CWorld@@QAEXPBVCGameEvent@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTACTIONS
#define FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPED_READERS
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE_INLINE_PASS29
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCWEAPON
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CGameEvent.h"
#include "engine/CGameEventDataPackage.h"
#include "engine/CWorld.h"

#include "engine/ThingAlive.h"
#include "engine/fable_nscript_forward.h"

void CWorld::EAControlledCreatureStrafe(const CGameEvent* event)
{
    CGameEventDataPackage event_data;
    event->GetDataPackage(&event_data);
    CPlayer* pplayer = GetPlayer(event->GetPlayerNumber());
    CThingPlayerCreature* phero = pplayer->GetPControlledCreature();
    FableNScriptUInt64 uid = event_data.GetUHUGE();
    float x_axis = event_data.GetFloat();
    float y_axis = event_data.GetFloat();
    EGameAction game_action = static_cast<EGameAction>(event_data.GetSLONG());
    if (IS_THING_ALIVE(phero)) {
        const CTCWeapon* tc_weapon = 0;
        const CTCCarrying* tc_carrying;
        CThing* carried_thing = phero->PeekTC(&tc_carrying)
            ? tc_carrying->GetThingInPrimarySlot() : 0;
        bool is_weapon = carried_thing ? carried_thing->PeekTC(&tc_weapon) : false;
        if (!is_weapon || (is_weapon && tc_weapon && tc_weapon->IsUsableBy(*phero))) {
            C3DVector strafe_vector(x_axis, y_axis, 0.0f);
            phero->ApplyRelativeMovementAcceleration(strafe_vector, game_action);
        }
    }
}