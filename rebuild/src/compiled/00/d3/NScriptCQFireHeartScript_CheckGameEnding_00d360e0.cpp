// NScript::CQ_FireHeartScript::CheckGameEnding() -- wait one script frame at a time until the hero force-quits the Fireheart minigame, then respawn the hero unless the script thread is terminating
// Leaf: ?CheckGameEnding@CQ_FireHeartScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D360E0, length 78 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface slots 7
// NewScriptFrame (+0x1c), 155 RespawnHero(bool) (+0x26c) and 256 HasHeroForceQuitFireheartMinigame() (+0x400) through Game (+0x40).
// Shape: `while (!Game->HasHeroForceQuitFireheartMinigame()) { NewScriptFrame(); if (terminating) return; }
// if (!terminating) Game->RespawnHero(true);` -- the loop's virtual test duplicated at entry and bottom.
// Identity (Naming NF194, 2026-11-20, q6412): Ego_r.pdb + FableWin.pdb ?CheckGameEnding@CQ_FireHeartScript@NScript@@QAEXXZ; CQ_FireHeartScript::Main
//   0x00D35090 spawns it under the literal "CheckGameEnding" and binds this spelling. Was the global stand-in CQ_FireHeartScript
//   (?CheckGameEnding@CQ_FireHeartScript@@QAEXXZ) on local CQuestScriptBase / interface models; now the class from
//   engine/CQ_FireHeartScript.h and the interface from the shared headers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"

void NScript::CQ_FireHeartScript::CheckGameEnding()
{
    while (!Game->HasHeroForceQuitFireheartMinigame()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating())
        Game->RespawnHero(true);
}
