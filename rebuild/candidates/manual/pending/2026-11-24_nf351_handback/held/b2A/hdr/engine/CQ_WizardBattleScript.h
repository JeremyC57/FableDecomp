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
#ifdef FABLE_RETAIL_CQ_WIZARDBATTLESCRIPT_INITIALISEVARIABLES
    // NF348 opt-in (row 0x00E1AD30): FableWin.pdb ?InitialiseVariables@CQ_WizardBattleScript@NScript@@QAEXXZ (absent from the Ego_r.pdb name set), out of
    // line at 0x00E1AD30, the first call of Main 0x00E1A990 (as FableWin's Main). Declaration only.
    void InitialiseVariables();                                          // 0x00E1AD30
#endif
#ifdef FABLE_RETAIL_CQ_WIZARDBATTLESCRIPT_MAIN
    // NF351 opt-in (row 0x00E1A990): Ego_r.pdb + FableWin.pdb ?Main@CQ_WizardBattleScript@NScript@@UAEXXZ (public virtual: it overrides
    // CScriptBase slot 2 Main), ?WatchForTermination@CQ_WizardBattleScript@NScript@@QAEXXZ and ?DoMission@CQ_WizardBattleScript@NScript@@QAEXXZ
    // (Main spawns both as threads), and the six nested entity-script classes whose ?Alloc@<Class>@CQ_WizardBattleScript@NScript@@SI...
    // Main binds (both PDB name sets), forward-declared only. Declarations only.
    virtual void Main();                                                 // 0x00E1A990
    void WatchForTermination();                                          // 0x00E1AD60
    void DoMission();                                                    // 0x00E1C0F0
    class CChasingScreamer;                                              // Alloc 0x00E1AE50
    class CHKC_VillagerScreamer;                                         // Alloc 0x00E1B100
    class CHKC_ScreamerPoint;                                            // Alloc 0x00E1B660
    class CHKC_Maze;                                                     // Alloc 0x00E1B950
    class CEndGameBarrier;                                               // Alloc 0x00E1BA60
    class CInitialScreamerPoint;                                         // Alloc 0x00E1BB80
#endif
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
FABLE_STATIC_ASSERT_AT(43, sizeof(NScript::CQ_WizardBattleScript) == 0x80);
#endif // FABLE_RETAIL_NSCRIPT
#endif
