// CGameScriptInterface::EntityTeleportToHeroPosition(const CScriptThing& entity) const -- when the entity is alive and more than 2 units (in XY) from the hero's controlled creature, find a wander position near the hero (radius 0.8) through the entity's navigation component and reset the entity's physics position there
// Leaf: ?EntityTeleportToHeroPosition@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008956B0, 262 bytes, `ret 4`.
// Callees: CScriptThing slots 75 IsAlive (+0x12c) and 11 GetPThing (+0x2c); CPlayerManager::GetMainPlayer 0x00449970;
//   CPlayer::PeekPControlledCreature 0x00487DD0 (Ego_r/FableWin ?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ);
//   CThing::GetPos 0x004C73D0; the const TC-map LowerBound 0x004365B0; CTCCreatureNavigation::GetNearbyWanderPosition
//   0x006CB6E0 (QBE_NABVC3DVector@@MAAV2@_N@Z); CTCPhysicsBase slot 31 ResetPosition (+0x7c). Float pool 4.0f @ 0x0123088C.
// Shape: the hero and thing XY are two C2DVector locals (the hero one shares the frame with the inner-scope C3DVector
//   wander target; the dead dx spill lands in the entity parameter slot); squared distance written dx*dx + dy*dy.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityTeleportToHeroPosition(const CScriptThing& entity) const
{
    if (entity.IsAlive()) {
        CThing* thing = entity.GetPThing();
        const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
        if (hero && !hero->IsDead()) {
            const C3DVector& heroPos = hero->GetPos();
            C2DVector heroXY(heroPos.X, heroPos.Y);
            const C3DVector& thingPos = thing->GetPos();
            C2DVector pos(thingPos.X, thingPos.Y);
            float dx = heroXY.X - pos.X;
            float dy = heroXY.Y - pos.Y;
            const CTCCreatureNavigation* navigation;
            if (dx * dx + dy * dy > 4.0f && thing->PeekTC(TCI_NAVIGATION, &navigation)) {
                C3DVector target;
                if (navigation->GetNearbyWanderPosition(hero->GetPos(), 0.8f, target, false)) {
                    CTCPhysicsBase* physics = thing->GetPhysicsTC();
                    if (physics)
                        physics->ResetPosition(target);
                }
            }
        }
    }
}