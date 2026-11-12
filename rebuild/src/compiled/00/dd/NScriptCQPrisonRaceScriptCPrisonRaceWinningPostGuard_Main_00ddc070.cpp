// NScript::CQ_PrisonRaceScript::CPrisonRaceWinningPostGuard::Main() -- the guard at the prison race finish: take scripted control, sheathe, stand at GuardPosition facing its angle; once the hero wins the race salute and then idle until the script ends
// Inline-budget: the handle's implicit destructor is expanded at the first two loops' terminate exits (0x00DDC1CB, and 0x00DDC210 which cross-jumps into it) and called out of line (0x007E74D0) at the other exits (0x00DDC2B5 / 0x00DDC2B9). `#pragma inline_depth(0)` round those `return` statements (the landed 0x00D73F40 device) keeps them on the COMDAT; /d2inlT200 (as Q10's 0x00E4AA00) lets the second inline site expand -- without it only the first loop's exit is expanded (583v597).
// Flags: /d2inlT200
// Leaf: ?Main@CPrisonRaceWinningPostGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDC070, 597 bytes (+ int3 padding), thiscall, plain `ret`; entity vtable 0x012DC9E8 slot 1.
// Identity: registration "PrisonRaceWinningPostGuard" 0x00DDAEFF in CQ_PrisonRaceScript::RegisterMain, Alloc 0x00DDBF90, vtable
//   slot 1; Ego_r.pdb / FableWin.pdb ?Main@CPrisonRaceWinningPostGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ.
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 (the handle's inline ctor and dtor), the scripted-thing handle's
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation 0x007E73D0 (slot 18) / IsPerformingScriptTask 0x007E7450 (slot 26) /
//   dtor 0x007E74D0; operator delete 0x00BFE9BC; IsActiveThreadTerminating 0x00F35B30; ~CScriptThing 0x004AA840; CCharString
//   0x0099EBF0 / 0x0099EAE0 ("GuardPosition" 0x012DC9D8, "ST_SALUTE" 0x012DCA04); CScriptThing slot 10 GetAngleXY; Game slots 7
//   NewScriptFrame, 8 StartScriptingEntity, 77 GetNearestWithScriptName, 472 EntityTeleportToThing, 474 EntitySetFacingAngle,
//   506 EntitySheatheWeapons. Global NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748.
// Shape: the final idle loop as the landed 0x00CFCCA0 / 0x00D43190 (`if (terminating) return; do NewScriptFrame(); while (!terminating)`):
//   the pre-check and the bottom test share one test, so retail enters the loop by `jmp` to its test (0x00DDC2A0).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/fable_script_animation_defaults.h"
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_WINNINGPOSTGUARD
#include "engine/CQ_PrisonRaceScript.h"

void NScript::CQ_PrisonRaceScript::CPrisonRaceWinningPostGuard::Main()
{
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    Game->EntitySheatheWeapons(Me, false);
    Game->EntityTeleportToThing(Me, Game->GetNearestWithScriptName(Me, "GuardPosition"), false);
    Game->EntitySetFacingAngle(Me, Game->GetNearestWithScriptName(Me, "GuardPosition").GetAngleXY(), true);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.HeroWonRace)
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    guard.PlayAnimation("ST_SALUTE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
    while (guard.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}