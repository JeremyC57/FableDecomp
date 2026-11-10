// CWorld::EAControlledCreatureStrafe(const CGameEvent*) -- game-event action: read the strafe packet (uid, x, y, action) and apply a relative movement to the player's controlled creature unless it carries a weapon it cannot use
// Grade: DIFFER(337v354) -- retail materialises the has-weapon bool (0x0062BE8D `mov al,1` / 0x0062BE91 `xor al,al` / `test al,al`) with the weapon in ebp zeroed first, and packs uid with action (frame 0x4C); ours folds the bool into jumps and packs uid with the C3DVector temporary (frame 0x48)
// Retail: 0x0062BD80 (354 bytes, `ret 4`, /GS), FableWin.pdb ?EAControlledCreatureStrafe@CWorld@@QAEXPBVCGameEvent@@@Z
// Flags: /GS
// Leaf: ?EAControlledCreatureStrafe@CWorld@@QAEXPBVCGameEvent@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
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

void CWorld::EAControlledCreatureStrafe(const CGameEvent* event)
{
    CGameEventDataPackage package;
    event->GetDataPackage(&package);
    CThingPlayerCreature* creature = GetPlayer(event->GetPlayerNumber())->GetPControlledCreature();
    {
        unsigned __int64 uid;
        package.GetData(sizeof(uid), &uid);
    }
    float x;
    package.GetData(sizeof(x), &x);
    float y;
    package.GetData(sizeof(y), &y);
    EGameAction action;
    package.GetData(sizeof(action), &action);
    if (creature && !creature->IsDead()) {
        const CTCWeapon* weapon = 0;
        bool hasWeapon = false;
        const CTCCarrying* carrying;
        if (creature->PeekTC(TCI_CARRYING, &carrying)) {
            CThing* item = carrying->GetThingInPrimarySlot();
            if (item && item->PeekTC(TCI_WEAPON, &weapon))
                hasWeapon = true;
        }
        if (hasWeapon && (!weapon || !weapon->IsUsableBy(*creature)))
            return;
        creature->ApplyRelativeMovementAcceleration(C3DVector(x, y, 0.0f), action);
    }
}
