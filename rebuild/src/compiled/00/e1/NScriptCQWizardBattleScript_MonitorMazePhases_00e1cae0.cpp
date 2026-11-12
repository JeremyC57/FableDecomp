// NScript::CQ_WizardBattleScript::MonitorMazePhases(CScriptThing& maze) -- thread: mark the maze boss on the minimap and as an evil-hero opinion source, then while it lives wait for each boss-phase change (one script frame at a time, the hero kept more than 5 units away), tracking the phase in MazePhase
// Inline-budget: the unused handle's implicit destructor ~CScriptGameResourceObjectScriptedThingBase (c1 60) is expanded at seven of the nine return sites in retail and called (0x007E74D0, one cross-jumped call at 0x00E1CCC1) only from the two returns of the MazeDefeated arm. VC7.1's static site frequencies here are 3.2 / 46.4 inside the phase loops and 0.2 / 0.1 at the outer-loop and final exits (sc 94 / 137 against T 74; tools/inl.sh), so no plain spelling separates the arm from the outer exits (15 loop-shape probes, chunk 1013 + Q9). Ladder: per-row /d2inlT138 expands every site, then a scoped #pragma inline_depth(0) pair around the arm's two returns keeps those two as calls.
// Leaf: ?MonitorMazePhases@CQ_WizardBattleScript@NScript@@QAEXAAVCScriptThing@@@Z
// Flags: /d2inlT138
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1CAE0, 568 bytes, thiscall, `ret 4`.
// Name: Ego_r.pdb ?MonitorMazePhases@CQ_WizardBattleScript@NScript@@QAEXAAVCScriptThing@@@Z (non-const CScriptThing&).
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430, ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0,
//   operator delete 0x00BFE9BC, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall),
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 70 GetHero, 350 MiniMapAddMarker, 522 EntityGetBossPhase,
//   622 EntitySetAsOpinionSource(const CScriptThing&, const CCharString&); CScriptThing slot 75 IsAlive.
// Data: "HUD_ORB_QUEST_CORE" 0x01258AB0, "OPINION_SOURCE_HERO_EVIL_HIGH" 0x012D25C0, 5.0f.
// Layout: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40, MasterScriptData +0x44); MazeDefeated +0x4F and MazePhase
//   +0x50 (Ego_r.pdb 99 / 100 - 0x14). The handle is the unused CScriptGameResourceObjectScriptedThingBase local
//   (inline ctor: CBaseObject call, vtable 0x0127094C, zeroed counted pointer from ebx).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CQ_WizardBattleScript.h"
#include "engine/fable_script_distance.h"   // IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall)

void NScript::CQ_WizardBattleScript::MonitorMazePhases(CScriptThing& maze)
{
    CScriptGameResourceObjectScriptedThingBase unusedHandle;
    Game->MiniMapAddMarker(maze, "HUD_ORB_QUEST_CORE");
    MazePhase = Game->EntityGetBossPhase(maze);
    Game->EntitySetAsOpinionSource(maze, "OPINION_SOURCE_HERO_EVIL_HIGH");
    while (maze.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        long phase = MazePhase;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            while (!IsDistanceBetweenThingsOver(Game->GetHero(), maze, 5.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            MazePhase = Game->EntityGetBossPhase(maze);
        } while (MazePhase == phase);
        if (IsActiveThreadTerminating())
            return;
        if (maze.IsAlive() && MazeDefeated == false) {
            // Retail calls ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 from these two returns only (0x00E1CCC1).
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
        }
    }
    if (IsActiveThreadTerminating())
        return;
}