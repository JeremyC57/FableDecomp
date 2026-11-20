// NScript::CV_BeardyBaldyScript::GoTalkToBarber() -- the mirror step of GoTalkToBeardyBaldy 0x00E50650: take the quest marker off
// BB_BeardyBaldyMan when he lives and put it on the barber nearest the hero when she lives
// Leaf: ?GoTalkToBarber@CV_BeardyBaldyScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E50E40, 400 bytes (NOT the twin of 0x00E50650: same callees, markers swapped).
// Identity: Ego_r.pdb + FableWin.pdb ?GoTalkToBarber@CV_BeardyBaldyScript@NScript@@QAEXXZ; WatchForQuestCardConditions 0x00E50910
//   calls it after objective 02 (0x00E50A96, 0x00E50DB8) and 0x00E50650 (GoTalkToBeardyBaldy) after objective 03, where the FSE Lua
//   port BeardyBaldy.lua calls GoTalkToBarber. Was: ?GoTalkToBeardyBaldy@CV_BeardyBaldyScript@@QAEXXZ (local struct, the
//   label shared with 0x00E50650).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete @ 0x00BFE9BC, ~CBaseClass
//          @ 0x0099A2E0), CScriptThing::IsAlive inline (game-side thing slot 75, +0x12c); CGameScriptInterface slots
//          70 GetHero (+0x118), 72 GetThingWithScriptName (+0x120), 81 GetNearestWithDefName (+0x144), 350
//          MiniMapAddMarker (+0x578), 352 MiniMapRemoveMarker (+0x580).
// Shape: the two handles are destroyed out of line at the terminating returns and expanded inline at the end.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_GOTALKTOBARBER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
#include "engine/CV_BeardyBaldyScript.h"
#include "engine/CGameScriptInterface.h"
// Retail expands these at this row's sites: defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }

void NScript::CV_BeardyBaldyScript::GoTalkToBarber()
{
    CScriptThing beardy = Game->GetThingWithScriptName("BB_BeardyBaldyMan");
    CScriptThing barber = Game->GetNearestWithDefName(Game->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    if (beardy.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapRemoveMarker(beardy);
    }
    if (barber.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapAddMarker(barber, "HUD_ORB_QUEST_VIGNETTE");
    }
}