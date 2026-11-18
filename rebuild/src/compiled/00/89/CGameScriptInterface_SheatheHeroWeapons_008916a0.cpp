// CGameScriptInterface::SheatheHeroWeapons() const -- if the main player's controlled creature is alive and carries a weapon: clear its queued actions, finish the current one twice, clear it, then make the creature sheathe the weapon back into the inventory
// Leaf: ?SheatheHeroWeapons@CGameScriptInterface@@UBEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008916A0, 115 bytes, plain `ret`; CGameScriptInterface vtable slot 130 (engine/CGameScriptInterface.h).
// Body: PlayerManager (+0x14)->GetMainPlayer() 0x00449970 .GetPControlledCreature() 0x00487DC0; IS_THING_ALIVE (null, CThing::Dead bit 0 of
//   +0x91) and CTCCarrying::IsThingCarryingWeapon 0x006A1540; then CThingCreatureBase::ClearQueuedActions 0x00663600, FinishCurrentAction
//   0x00662650 twice, ClearCurrentAction 0x006633A0, and SetCurrentAction 0x006644F0 with a CCreatureAction_SheatheItemToInventory(creature, 0,
//   100) temporary (ctor 0x0069FFD0, implicit virtual destructor's COMDAT 0x0048C950). Local as Ego_d.pdb names it (pplayercreature).
// NF79 (round 7, q10972): ported onto the shared headers (CGameScriptInterface.h NSCRIPT, CPlayerManager.h, CPlayer.h, the retail CThing /
//   CThingCreatureBase chain with FINISHACTION / CLEARQUEUED and my opt-in CTHINGCREATUREBASE_CLEARCURRENTACTION, CTCCarrying.h,
//   CCreatureAction_SheatheItemToInventory.h with NF77's IMPLICIT_DTOR): binds 0x0048C950 as ??1CCreatureAction_SheatheItemToInventory@@UAE@XZ
//   (was the stand-in ?Destroy@CGSISheatheHeroWeapons_ActionStorage), the ctor, ClearQueuedActions / FinishCurrentAction / ClearCurrentAction
//   / SetCurrentAction / IsThingCarryingWeapon under their PDB spellings (were CGSISheatheHeroWeapons_Target stand-ins PrepareSheathe /
//   UpdateSheathe / FinaliseSheathe); the interface read through its PlayerManager member (was a reinterpret_cast view of `this`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_CLEARQUEUED
#define FABLE_RETAIL_CTHINGCREATUREBASE_CLEARCURRENTACTION
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SHEATHEITEMTOINVENTORY_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SheatheItemToInventory.h"
#include "engine/CTCCarrying.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CGameScriptInterface.h"

// Lionhead's IS_THING_ALIVE on a thing pointer (null, then CThing::Dead), as the landed rows spell it.
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

void CGameScriptInterface::SheatheHeroWeapons() const
{
    CThingPlayerCreature* pplayercreature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(pplayercreature) && CTCCarrying::IsThingCarryingWeapon(*pplayercreature)) {
        pplayercreature->ClearQueuedActions();
        pplayercreature->FinishCurrentAction();
        pplayercreature->FinishCurrentAction();
        pplayercreature->ClearCurrentAction();
        pplayercreature->SetCurrentAction(CCreatureAction_SheatheItemToInventory(*pplayercreature, 0, 100));
    }
}
