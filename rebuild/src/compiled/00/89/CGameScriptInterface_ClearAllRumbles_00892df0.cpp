// CGameScriptInterface::ClearAllRumbles() const -- if the main player's controlled creature is alive, clear every rumble on its joystick
//   (the input manager's joystick for the creature's device number, CJoystick vtable slot 18)
// Leaf: ?ClearAllRumbles@CGameScriptInterface@@UBEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00892DF0, 53 bytes: PlayerManager (+0x14)->GetMainPlayer() 0x00449970 .GetPControlledCreature()
//   0x00487DC0; null / CThing::Dead (bit 0 of +0x91) test; CThing::GetJoystickDeviceNumber 0x004C7CA0; GPInputManager (0x013B8388)
//   PJoysticks (+0x2C) [n] and a tail `jmp [eax+48h]` (CJoystick::ClearAllRumbles, slot 18).
// Identity (two sources): Ego_r.pdb 0x008CDF90 + FableWin.pdb 0x02AA7E60 ?ClearAllRumbles@CGameScriptInterface@@UBEXXZ, CGameScriptInterface
//   vtable slot 107 = 0x00892DF0 (engine/CGameScriptInterface.h); no landed row references 0x00892DF0 (exe scan).
// NF171 (2026-11-20, q11922 / q11924): re-authored on the shared headers (the 0x008916A0 SheatheHeroWeapons macro set, CThing MOREACCESS for
//   GetJoystickDeviceNumber, engine/CInputManager.h JOYSTICKS for GetJoystick, engine/CJoystick.h VIRTUALS for slot 18, as the landed 0x0058578B); 0x004C7CA0 now binds
//   Ego_r.pdb + FableWin.pdb ?GetJoystickDeviceNumber@CThing@@QBEJXZ (was the free stand-in ?GetIndex@@YIHPAUCRumbleTarget@@@Z). Was an
//   old-style header-less row: the free __fastcall stand-in CGameScriptInterface_ClearAllRumbles over local CRumbleTarget / CPlayer /
//   CThingPlayerCreature / CRumbleManager models. 0x013B8388 keeps the row's existing ?GPInputManager@@3PAVCInputManager@@A (that address is
//   already bound under two spellings; this row adds none).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CINPUTMANAGER_JOYSTICKS
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CInputManager.h"
#include "engine/CGameScriptInterface.h"

extern CInputManager* GPInputManager;   // 0x013B8388

// Lionhead's IS_THING_ALIVE on a thing pointer (null, then CThing::Dead), as the landed rows spell it.
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

void CGameScriptInterface::ClearAllRumbles() const
{
    CThingPlayerCreature* pplayercreature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(pplayercreature))
        GPInputManager->GetJoystick(pplayercreature->GetJoystickDeviceNumber()).ClearAllRumbles();
}
