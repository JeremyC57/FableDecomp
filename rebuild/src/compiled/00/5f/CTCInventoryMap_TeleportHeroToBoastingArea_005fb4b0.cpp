// CTCInventoryMap::TeleportHeroToBoastingArea() const -- while the hero creature is alive and an AI creature with property 0x72 (the boasting-area marker) exists, show the mini map on the region load and queue a move-hero-to-region game event (0x2C) carrying that creature's position and facing angle, flags all false, from the main player
// Leaf: ?TeleportHeroToBoastingArea@CTCInventoryMap@@QBEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FB4B0, 379 bytes, plain `ret`, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
//   Ego_r.pdb ?TeleportHeroToBoastingArea@CTCInventoryMap@@QBEXXZ (public const); called by the landed
//   CTCInventoryQuests::TeleportToBoastingPodium 0x0061A360.
// Callees: CTCBase::PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayer() const 0x00449960, CPlayer::PeekPControlledCreature
//   0x00487DD0, CTCBase::PeekWorld 0x00686A80 / GetWorld 0x00686A70, CThingSearchTools::GetPFirstLeafThing<
//   CThingFilter_AICreatureHasProperty, CThingAICreature> 0x006156E0 (same-TU body, engine/CThingSearchTools.h), CThing::GetPos 0x004C73D0,
//   CGameEvent::AddData 0x009F1760 (the inline AddFloat / AddUBYTE), the physics slot 63 GetFacingAngleXY (+0xFC),
//   CPlayerManager::GetMainPlayerNumber 0x00449700, CWorld::HandleMoveHeroToRegionGameEvent 0x0049EAF0.
// Shape: the same-TU GetPFirstLeafThing body tells VC7.1 the filter temporary and the result slot are not captured, so the result
//   slot is reused for the AddFloat words (retail [esp+10h]); the event constructor and adders are inline.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MOVEHEROTOREGION
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTCINVENTORYMAP_TELEPORT
#include "engine/CTCInventoryMap.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CGameEvent.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_AICreatureHasProperty.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "fable_game.h"

void CTCInventoryMap::TeleportHeroToBoastingArea() const
{
    const CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead()) {
        CThingAICreature* boasting_marker;
        if (PeekWorld().PeekThingSearchTools().GetPFirstLeafThing(CThingFilter_AICreatureHasProperty(0x72), &boasting_marker)) {
            GetWorld().DisplayMiniMapOnRegionLoad = true;
            CGameEvent event(0x2c);
            event.AddFloat(boasting_marker->GetPos().X);
            event.AddFloat(boasting_marker->GetPos().Y);
            event.AddFloat(boasting_marker->GetPos().Z);
            event.AddFloat(boasting_marker->GetPhysicsTC()->GetFacingAngleXY());
            event.AddUBYTE(false);
            event.AddUBYTE(false);
            event.AddUBYTE(false);
            event.SetPlayer(CMainGameComponent::Get()->GetPlayerManager().GetMainPlayerNumber());
            GetWorld().HandleMoveHeroToRegionGameEvent(event);
        }
    }
}