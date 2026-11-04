// NScript::CV_ChickenKickingScript::CreateSpectators() -- spawned quest thread: until the spectators exist, whenever Oak Bay is loaded and the hero knows the ghost has gone, create three unemployed villagers ("Spectator") at the Spectator1..3 markers; otherwise wait for Oak Bay to unload and try again
// Leaf: ?CreateSpectators@CV_ChickenKickingScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E62D60, 1161 bytes, thiscall, plain `ret`, no EH frame. Literals "OakBay" 0x012D31E4,
//   "Spectator" 0x012E73A0, "Spectator1" 0x012E7450 / "Spectator2" 0x012E7418 / "Spectator3" 0x012E73E0,
//   "CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED" 0x012E7424, "CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED" 0x012E73EC.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?CreateSpectators@CV_ChickenKickingScript@NScript@@QAEXXZ (declared in
//   engine/NScript_CV_ChickenKickingScript.h after Main); the landed CV_ChickenKickingScript::Main 0x00E62A00 spawns it as the
//   "CreateSpectators" thread (reloc binding of 0x00E62D60 from 0x00E62A00); the literals are the CSpectator entity's script name.
// Layout: engine/NScript_CV_ChickenKickingScript.h (Game +0x40, KnowGhostHasGone +0x54, SpectatorsCreated +0x55).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString 0x0099EBF0 / ~ 0x0099EAE0; the two CScriptThing
//   temporaries of each CreateCreature line are destroyed inline (vtable 0x01238C8C, CCountedPointer release with operator delete
//   0x00BFE9BC, ~CBaseClass 0x0099A2E0); CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 72
//   GetThingWithScriptName, 91 CreateCreature; CScriptThing slot 6 GetPos.
// Shape: the "while loaded" wait is written `!= false` (test/setne), as the landed same-TU LookAfterOrganiser 0x00E631F0; the
//   first wait's `&&` puts the "OakBay" temporary under the conditional-destruction flag (the `or eax,1` word at esp+0x14).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/NScript_CV_ChickenKickingScript.h"
inline CScriptThing::~CScriptThing() { }
void NScript::CV_ChickenKickingScript::CreateSpectators()
{
    while (!SpectatorsCreated) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (Game->IsRegionLoaded("OakBay") && !SpectatorsCreated) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (KnowGhostHasGone) {
                if (IsActiveThreadTerminating()) return;
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("Spectator1").GetPos(), "Spectator", false);
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", Game->GetThingWithScriptName("Spectator2").GetPos(), "Spectator", false);
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("Spectator3").GetPos(), "Spectator", false);
                SpectatorsCreated = true;
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (Game->IsRegionLoaded("OakBay") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
}