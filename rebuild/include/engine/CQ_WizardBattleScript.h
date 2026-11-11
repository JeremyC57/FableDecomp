#ifndef FABLE_ENGINE_CQ_WIZARDBATTLESCRIPT_H
#define FABLE_ENGINE_CQ_WIZARDBATTLESCRIPT_H
// Retail opt-in (chunk Q9, row 0x00E1CAE0 MonitorMazePhases): NScript::CQ_WizardBattleScript, new header (no includer before
// chunk Q9; the landed PhaseTwo 0x00E1C580 / PhaseOne 0x00E1CFD0 / WatchForLeaving 0x00E1C4D0 / CutsceneScreamers 0x00E1CD40
// keep their local models).
// Layout: Ego_r.pdb CQ_WizardBattleScript (struct_layouts_egor.tsv, 148 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): VillagersTurnOnHero +0x48 .. MazeDefeated +0x4f, MazePhase +0x50, NumberOfScreamersAlive +0x54,
// MazeConversation +0x58, Maze +0x5c, TopOfLighthouse +0x68, MiddleOfLighthouse +0x74, sizeof 0x80 (Alloc 0x00E1D9F0 allocates
// 0x80 and calls the ctor 0x00E1A800). Checked against retail: MonitorMazePhases reads MazeDefeated +0x4f and writes MazePhase
// +0x50; the landed PhaseTwo reads MissionFailed +0x4c, MazeConversation +0x58 and Maze +0x5c.
// Methods: Ego_r.pdb ?<Name>@CQ_WizardBattleScript@NScript@@ (FableWin.pdb has the same set); only the retail addresses of
// landed or graded rows are given.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WizardBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void WatchForLeaving();                                              // 0x00E1C4D0
    void MonitorMazePhases(CScriptThing& maze);                          // 0x00E1CAE0
    void PhaseTwo();                                                     // 0x00E1C580
    void CutsceneScreamers();                                            // 0x00E1CD40
    void PhaseOne();                                                     // 0x00E1CFD0
    bool VillagersTurnOnHero;                                            // +0x48
    bool VillagersChased;                                                // +0x49
    bool FightersStart;                                                  // +0x4a
    bool MissionSucceeded;                                               // +0x4b
    bool MissionFailed;                                                  // +0x4c
    bool MissionOver;                                                    // +0x4d
    bool QuestStartScreenShown;                                          // +0x4e
    bool MazeDefeated;                                                   // +0x4f
    long MazePhase;                                                      // +0x50
    long NumberOfScreamersAlive;                                         // +0x54
    long MazeConversation;                                               // +0x58
    CScriptThing Maze;                                                   // +0x5c
    CScriptThing TopOfLighthouse;                                        // +0x68
    CScriptThing MiddleOfLighthouse;                                     // +0x74
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_WizardBattleScript) == 0x80);
#endif // FABLE_RETAIL_NSCRIPT
#endif
