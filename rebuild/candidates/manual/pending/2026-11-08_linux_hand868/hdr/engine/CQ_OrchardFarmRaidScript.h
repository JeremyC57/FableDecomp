#ifndef FABLE_ENGINE_CQ_ORCHARDFARMRAIDSCRIPT_H
#define FABLE_ENGINE_CQ_ORCHARDFARMRAIDSCRIPT_H
// Retail opt-in (chunk 863): NScript::CQ_OrchardFarmRaidScript, new header (no includer before chunk 863; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CQ_OrchardFarmRaidScript (struct_layouts_egor.tsv, 292 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), a further - 4 after the STLport CrateList vector (0xC, Dinkumware 0x10): CrateCount +0x48,
// CommentTimer +0x4c, DoneIntroduction +0x50, HeroAtWrongEntrance +0x51, WhisperSpawned +0x52, CrateList +0x54, MissionSucceeded
// +0x60, MissionFailed +0x64, FailReasons[4] +0x68, HeroTeam +0x78, BanditWavesSpawned +0x7c, RemindHeroOfObjectivesTimer +0x80,
// TextSystemScriptCode +0x84, HeroMetWhisperBeforeFarm +0x88 .. WhisperInCutscene +0x8b, Teams[2] +0x8c (CCrateTeamManager, 0x40
// each, no STL), sizeof 0x10c. Checked against retail: the landed ctor 0x00DCC040 (CTimer +0x4c, vector +0x54, four CWideString
// ctors from +0x68, CTimer +0x80, CCharString +0x84, the Teams loop from +0x8c); DoCutsceneIfRequired 0x00DCFA60 reads HeroTeam
// [esi+78h], stores HeroAtWrongEntrance [esi+51h] and DoneIntroduction [esi+50h].
// MissionFailed: the PDB type is unnamed (<unnamed-type-MissionFailed>, 4 bytes, no enumerator names in the dumps); kept as long.
// Methods: Ego_r.pdb / FableWin.pdb name set ?<Name>@CQ_OrchardFarmRaidScript@NScript@@ (RegisterMain / Main / Init / OnPersist
// virtual; DoCutsceneIfRequired, DoMultiplierCutscene, ProcessGameRulesEvil / Good, ProcessStolenCrate, ReplaceQuestCards QAEXXZ).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40; the nested entity scripts' PMasterData is a CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CCrateTeamManager {                                            // Ego_r.pdb, 0x40, ??0 / ??1 QAE
    public:
        long StateCounter[6];                                            // +0x00
        long MemberCount;                                                // +0x18
        CTimer TeamReinforcementsTimer;                                  // +0x1c
        long MaxTeamMembers;                                             // +0x20
        CScriptThing CrateDropPos;                                       // +0x24
        CScriptThing TeamCrateCarrier;                                   // +0x30
        CCrateTeamManager* EnemyTeam;                                    // +0x3c
    };
    virtual void RegisterMain();                                         // slot 1 0x00DCC660
    virtual void Main();                                                 // slot 2 0x00DCC770
    virtual void Init();                                                 // 0x00DCC140 (slot order after Main unproven)
    virtual void OnPersist(CPersistContext&);                            // 0x00DCC720
    void DoCutsceneIfRequired();                                         // 0x00DCFA60
    void DoMultiplierCutscene();                                         // 0x00DD1AF0
    void ProcessGameRulesEvil();                                         // 0x00DD03D0
    void ProcessGameRulesGood();
    void ProcessStolenCrate();
    void ReplaceQuestCards();                                            // 0x00DD0EB0
    long CrateCount;                                                     // +0x48
    CTimer CommentTimer;                                                 // +0x4c
    bool DoneIntroduction;                                               // +0x50
    bool HeroAtWrongEntrance;                                            // +0x51
    bool WhisperSpawned;                                                 // +0x52
    std::vector<CScriptThing> CrateList;                                 // +0x54
    bool MissionSucceeded;                                               // +0x60
    long MissionFailed;                                                  // +0x64 (<unnamed-type-MissionFailed>)
    CWideString FailReasons[4];                                          // +0x68
    long HeroTeam;                                                       // +0x78
    long BanditWavesSpawned;                                             // +0x7c
    CTimer RemindHeroOfObjectivesTimer;                                  // +0x80
    CCharString TextSystemScriptCode;                                    // +0x84
    bool HeroMetWhisperBeforeFarm;                                       // +0x88
    bool ShownCombatMultiplierTutorial;                                  // +0x89
    bool ShownCombatFlourishTutorial;                                    // +0x8a
    bool WhisperInCutscene;                                              // +0x8b
    CCrateTeamManager Teams[2];                                          // +0x8c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OrchardFarmRaidScript::CCrateTeamManager) == 0x40);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OrchardFarmRaidScript) == 0x10c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_ORCHARDFARMRAIDSCRIPT_H
