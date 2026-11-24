// CGameScriptInterface::IsPlayerCreatureFlourishEnabled() const -- true when the main player's controlled creature is alive, has
//   a combat component and that component allows a melee flourish.
// Leaf: ?IsPlayerCreatureFlourishEnabled@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00899DB0, 91 bytes, plain `ret`; CGameScriptInterface vtable slot 152 (engine/CGameScriptInterface.h;
//   Ego_r.pdb / FableWin.pdb ?IsPlayerCreatureFlourishEnabled@CGameScriptInterface@@UBE_NXZ).
// NF256 (2026-11-22, CODEXP27 L57): ported off the free __fastcall stand-in (CThingOverlay / CInterfaceMap local models, extern "C"
//   @Sub_IsMeleeFlourishValid@4) onto the shared headers as the member: the alive test is IS_THING_ALIVE (engine/ThingAlive.h,
//   CThing::Dead = bit 0 of +0x91), HasTC(TCI_COMBAT) is the UsingComponents bit 48 (`test dword [eax+24h],10000h`), and the
//   component comes from the non-const CThing::GetTC(ETCInterfaceType) (CVectorMap<ETCInterfaceType,CTCBase*>::Find through the
//   non-const LowerBound 0x0040F020, Components at +0x44); CTCCombat::IsMeleeFlourishValid 0x00733EE0 (engine/CTCCombat.h) is
//   called on it as a thiscall.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (PlayerManager +0x14), CPlayer::GetPControlledCreature 0x00487DC0,
//   CVectorMap<...>::LowerBound 0x0040F020, CTCCombat::IsMeleeFlourishValid 0x00733EE0.
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
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/ThingAlive.h"
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::IsPlayerCreatureFlourishEnabled() const
{
    CThingPlayerCreature* pplayercreature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(pplayercreature) && pplayercreature->HasTC(TCI_COMBAT))
        return static_cast<CTCCombat&>(pplayercreature->GetTC(TCI_COMBAT)).IsMeleeFlourishValid();
    return false;
}
