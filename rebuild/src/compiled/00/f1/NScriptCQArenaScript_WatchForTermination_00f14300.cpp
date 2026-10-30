// NScript::CQ_ArenaScript::WatchForTermination() -- wait until the arena mission fails or succeeds, restart time; on success pay the gold total (plus the spare-Whisper morality if Whisper lives), teleport the hero to ArenaExtEntrance, wait for ArenaExterior to load, complete the quest, fade in and mark the arena finished; finally re-enable teleporting and deactivate the quest
// Leaf: ?WatchForTermination@CQ_ArenaScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F14300, 594 bytes, thiscall, plain `ret`.
// Identity (proven): spawned as "WatchForTermination" (0x012C84B0) by CQ_ArenaScript::Main 0x00F0FB70 (slot 2 of the level-script
//   vtable 0x012C7798 whose slot 1 is the landed NScript::CQ_ArenaScript::RegisterMain 0x00CFAA50): the CSpawnedFunc Func store
//   at 0x00F10295 writes this row right after that literal (CSpawnedFunc<CQ_ArenaScript> vtable 0x012C7834); the row has no
//   other reference. Ego_r.pdb ?WatchForTermination@CQ_ArenaScript@NScript@@QAEXXZ. Catalog label
//   Script_Arena_Teleport_Thread (invented).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   ~CScriptThing 0x004AA840; Game (+0x40) slots 7 NewScriptFrame, 12 IsRegionLoaded, 70 GetHero, 72 GetThingWithScriptName,
//   126 GiveHeroGold, 156 GiveHeroMorality, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 374 FadeScreenIn, 381
//   SetAllowScreenFadingOnNextRegionChange, 472 EntityTeleportToThing, 565 SetTeleportingAsActive, 648 SetTimeAsStopped,
//   655 GetActiveQuestName; pGlobalScriptDef 0x0143E90C.
// Literals: "ArenaExtEntrance" 0x012F8F84, "ArenaExterior" 0x012C6148.
// Layout: retail NScript headers (CLevelScriptBase 0x48: Game +0x40, MasterScriptData +0x44); CQ_ArenaScript members at Ego_r.pdb
//   - 0x14 up to Rounds (STLport vector<CArenaRoundDef>, 0xc) and - 0x18 after it: GoldTotal +0xb8, MissionSucceeded +0xe0,
//   MissionFailed +0xe1, StopTimeIndex +0x118. CQ_SunnyvaleMasterData (Ego_r offsets, no containers before them):
//   WhisperKilledByHero +0x74, ArenaFinished +0x75. CScriptDef ArenaSpareWhisperMorality at retail +0xaec (Ego_r +0xb90).
// Shape: the landed CQ_WizardBattleScript::WatchForTermination 0x00E1AD60 form (`&&` wait, if/else with a terminate test in
//   each arm, shared deactivation tail); the teleport target is a named CScriptThing local destroyed out of line.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

class CScriptDef {
public:
    unsigned char _before[0xaec];
    float ArenaSpareWhisperMorality;                    // +0xaec
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _before[0x74];
    bool WhisperKilledByHero;                           // +0x74
    bool ArenaFinished;                                 // +0x75
};
class CQ_ArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CCharString CrowdLoopTags[4][5];                    // +0x48
    unsigned char Rounds[0xc];                          // +0x98 std::vector<CArenaRoundDef>
    long ArenaState;                                    // +0xa4
    long ArenaRound;                                    // +0xa8
    long ArenaRoundWave;                                // +0xac
    long GoldMultiplier;                                // +0xb0
    bool ChamLeaving;                                   // +0xb4
    long GoldTotal;                                     // +0xb8
    bool InHitCutsceneAlready;                          // +0xbc
    bool NeedBertForSpeech;                             // +0xbd
    CScriptThing CellsVillage;                          // +0xc0
    bool PlayerLeaving;                                 // +0xcc
    bool PlayerWon;                                     // +0xcd
    long TotalCreatures[3];                             // +0xd0
    long ExtraCreatures;                                // +0xdc
    bool MissionSucceeded;                              // +0xe0
    bool MissionFailed;                                 // +0xe1
    bool ArenaSpawnNeeded[16];                          // +0xe2
    long NewCrowdPoints;                                // +0xf4
    long NewCrowdHappiness;                             // +0xf8
    long NewCrowdBaseLevel;                             // +0xfc
    CTimer GlobalCrowdTimer;                            // +0x100
    bool FinalBattleCS;                                 // +0x104
    CScriptThing Whisper;                               // +0x108
    bool PauseCrowdChecker;                             // +0x114
    bool WhisperNeededForCutscene;                      // +0x115
    long StopTimeIndex;                                 // +0x118
    void WatchForTermination();
};

void CQ_ArenaScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetTimeAsStopped(false, StopTimeIndex);
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroGold(GoldTotal);
        if (!MasterScriptData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveHeroMorality(pGlobalScriptDef->ArenaSpareWhisperMorality);
        }
        CScriptThing entrance = Game->GetThingWithScriptName("ArenaExtEntrance");
        Game->SetAllowScreenFadingOnNextRegionChange(false);
        Game->EntityTeleportToThing(Game->GetHero(), entrance, false);
        while (!Game->IsRegionLoaded("ArenaExterior")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
        Game->FadeScreenIn();
        Game->SetAllowScreenFadingOnNextRegionChange(true);
        MasterScriptData->ArenaFinished = true;
    }
    Game->SetTeleportingAsActive(true);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript