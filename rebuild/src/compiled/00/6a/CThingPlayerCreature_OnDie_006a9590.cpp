// CThingPlayerCreature::OnDie() -- tells the owning player a creature died; for the main player copies the global definition's respawn delay into the player and lets the nearest village clear its crimes; then the base OnDie
// Leaf: ?OnDie@CThingPlayerCreature@@MAEXXZ
// Retail: Fable.exe 0x006A9590, 73 bytes. Identity: Ego_r.pdb ?OnDie@CThingPlayerCreature@@MAEXXZ (protected virtual; the CThing slot-11
//   override: vtable 0x01245828 holds 0x006A9590).
// Callees: CThing::GetPlayer 0x004C7A30, CPlayer::GlobalEventCreatureDied 0x0087C580, CPlayer::IsMainPlayer 0x00487520 (out of line),
//   CThing::PeekGlobalDefinition 0x004C79A0 (protected const; CGlobalDef::RespawnDelay), CPlayer::GetPNearestTCVillage 0x00488620,
//   CTCVillage::ClearCrimes 0x0065C2D0, CThingCreatureBase::OnDie 0x00663F50 (tail jump).
// Shape: the player in a local (esi), `this` kept in edi; the respawn delay is read and stored to the player's PlayerRespawnDelay
//   +0x3c before the village call; the base call is the last statement (sibling `jmp`).
// NF62 (2026-11-17, round 7): ported onto the shared CThing / CThingPlayerCreature / CPlayer / CGlobalDef / CTCVillage headers (the local
//   CThing / CThingPhysical / CThingGameObject / CThingCreatureBase / CPlayer / CTCVillage models and the CWorldState stand-in are gone);
//   0x004C79A0 now binds ?PeekGlobalDefinition@CThing@@IBEABVCGlobalDef@@XZ (was the stand-in GetWorldState). Overlay opt-ins:
//   FABLE_RETAIL_CTHINGCREATUREBASE_ONDIE, FABLE_RETAIL_CTHINGPLAYERCREATURE_ONDIE, FABLE_RETAIL_CPLAYER_GLOBALEVENTCREATUREDIED.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKGLOBALDEFINITION
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ONDIE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_ONDIE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER_NEARESTVILLAGE
#define FABLE_RETAIL_CPLAYER_GLOBALEVENTCREATUREDIED
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
#include "engine/CPlayer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CGlobalDef.h"
#include "engine/CTCVillage.h"

void CThingPlayerCreature::OnDie()
{
    CPlayer* player = (&GetPlayer());
    player->GlobalEventCreatureDied(this);
    if (player->IsMainPlayer())
    {
        player->PlayerRespawnDelay = PeekGlobalDefinition().RespawnDelay;
        CTCVillage* village = player->GetPNearestTCVillage();
        if (village)
            village->ClearCrimes();
    }
    CThingCreatureBase::OnDie();
}
