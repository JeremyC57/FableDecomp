// NScript::CQ_MinionClifftopChaseScript::ProcessMission() -- walk the chase objectives: wait for Deserted Farm, set objective 3, wait for Gibbet Woods, set objective 4, wait for the rock troll and let a guard comment on it, wait for the prison path, start the escape timer and objective 5, then fail the quest if the timer runs out before the map keeper's dialogue
// Leaf: ?ProcessMission@CQ_MinionClifftopChaseScript@NScript@@QAEXXZ
// Flags: /d2inlS22
// Inline-budget: /d2inlS22 (per-row flag, preference 2) -- retail expands CTimer::operator=(long) and CTimer::GetTime() (pDSTGame slots 89 / 90) at all three sites in this 1110-byte body; with plain inline definitions the default budget leaves them called (DIFFER 1083v1110), /d2inlS20 and /d2inlS21 give DIFFER(1105v1110), /d2inlS22 / 25 / 30 / 40 / 60 / 80 all RELOCATION_MATCH: 22 is the smallest. (The HEAD local model used FABLE_FORCEINLINE on its own CTimer stand-in instead.)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA8510, 1110 bytes (int3 padded to 1120).
// Identity (two sources): Ego_r.pdb ?ProcessMission@CQ_MinionClifftopChaseScript@NScript@@QAEXXZ (engine/CQ_MinionClifftopChaseScript.h
//   declares it); the landed Main 0x00DA34B0 spawns this body under the thread-name literal "ProcessMission" (0x012D7194) and binds that
//   spelling through the shared header.
//   Name-fix NF186 (q6265): was a global `struct CQ_MinionClifftopChaseScript : CScriptBase` local model (leaf
//   ?ProcessMission@CQ_MinionClifftopChaseScript@@QAEXXZ; catalog CQ_MinionClifftopChaseScript::ProcessMission, no NScript::). Its callee
//   GuardComment 0x00DA8970 now binds the PDB ?GuardComment@CQ_MinionClifftopChaseScript@NScript@@QAEXVCCharString@@@Z (the self row's leaf)
//   instead of the global spelling.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, GuardComment 0x00DA8970 (by-value
//   comment); pDSTGame 0x0143E8F8 timer slots 89 SetTimer / 90 GetTimer; the quest interface (+0x40) slots 7, 12,
//   296 SetQuestCardObjective, 321 DisplayQuestInfo, 326 AddQuestInfoTimer, 655 GetActiveQuestName, 697 StopOverrideMusic;
//   pGlobalScriptDef 0x0143E90C (+0xd54 MCC_Timer).
// Layout: engine/CQ_MinionClifftopChaseScript.h (Ego_r.pdb - 0x14): MapKeeperSaidDialog +0x48, MissionFailed +0x49, RockTrollAppeared +0x54,
//   MissionTimer +0x5c (CTimer), MissionTimerID +0x60, ReasonForFailure +0x64 (the local model's TrollFlag / EscapeTimer / TimerBarID).
// CTimer: engine/CTimer.h; its assignment and GetTime are defined inline here (pDSTGame slots 89 / 90, as the landed 0x00D647F0 defines them)
//   and expanded at every site (retail calls neither; see the Inline-budget line).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_GUARDCOMMENT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "engine/CQ_MinionClifftopChaseScript.h"
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>)
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
namespace NScript {
void CQ_MinionClifftopChaseScript::ProcessMission()
{
    while (!Game->IsRegionLoaded("DesertedFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_03", "GibbetWoods", "BowerstoneSlums");
    while (!Game->IsRegionLoaded("GibbetWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_04", "PrisonPath", "BowerstoneSlums");
    while (!RockTrollAppeared) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GuardComment("ROCK_TROLL");
    while (!Game->IsRegionLoaded("PrisonPath")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->StopOverrideMusic(false);
    long escapeTime = pGlobalScriptDef->MCC_Timer;
    MissionTimer = escapeTime;
    Game->DisplayQuestInfo(true);
    MissionTimerID = Game->AddQuestInfoTimer(MissionTimer, "HUD_CLOCK_ICON", 1.0f);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_05", "PrisonPath", "BowerstoneSlums");
    while (!MapKeeperSaidDialog && !MissionFailed && MissionTimer.GetTime() != 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionTimer.GetTime() == 0) {
        if (IsActiveThreadTerminating())
            return;
        ReasonForFailure = 1;
        MissionFailed = true;
    }
}
}
