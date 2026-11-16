#ifndef FABLE_ENGINE_CQ_TRADERCONFLICTEVILSCRIPT_H
#define FABLE_ENGINE_CQ_TRADERCONFLICTEVILSCRIPT_H
// NEW shared header (wfE1, row 0x00DF6010): NScript::CQ_TraderConflictEvilScript, the trader-conflict (evil) level script.
// Layout: Ego_r.pdb CQ_TraderConflictEvilScript (136 bytes in Ego_r; its CLevelScriptBase is 20 bytes and its vector 4 bytes larger,
// as the landed CQ_TraderConflictGoodScript: AllCreatures Ego_r 92 = TLC +0x48): AllCreatures +0x48, CounterID +0x54,
// NumberSpawned +0x58, InitialNumberInRegion +0x5c, QuestStartScreened +0x60, NextTimeToSpawnGuards +0x64, MissionSucceeded +0x68,
// HeroAttackedBandit +0x69, PlayerEngaged +0x6a, ScreamOutTimer +0x6c; sizeof 0x70. Members and entity classes: Ego_r.pdb +
// FableWin.pdb name sets (?Main / ?Init / ?RegisterMain / ?UpdateLiveEnemies / ?WatchTimeLimit / ?PlayCutscene /
// ?GetNumberLeftToKill / ?GetNumberOfGuardsLeftToKill @CQ_TraderConflictEvilScript@NScript@@; the five entity Allocs).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_TraderConflictEvilScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "stlport/fable_containers.h"
class CScriptDataBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00DF5F50
    virtual void Main();                                                 // 0x00DF6010
    virtual void Init();
    void WatchTimeLimit();                                               // 0x00DF7980 (functor stored by Main 0x00DF6612)
    void UpdateLiveEnemies();                                            // 0x00DF9E00
    void PlayCutscene(CCharString);                                      // 0x00DF9B80
    long GetNumberLeftToKill();
    long GetNumberOfGuardsLeftToKill();
    std::vector<CScriptThing> AllCreatures;                              // +0x48
    long CounterID;                                                      // +0x54
    long NumberSpawned;                                                  // +0x58
    long InitialNumberInRegion;                                          // +0x5c
    bool QuestStartScreened;                                             // +0x60
    long NextTimeToSpawnGuards;                                          // +0x64
    bool MissionSucceeded;                                               // +0x68
    bool HeroAttackedBandit;                                             // +0x69
    bool PlayerEngaged;                                                  // +0x6a
    CTimer ScreamOutTimer;                                               // +0x6c
    class CTC_GuardSpawnPoint;                                           // Alloc 0x00DF7B20
    class CTC_BanditFollower;                                            // Alloc 0x00DF7F90
    class CTC_BanditFighter;                                             // Alloc 0x00DF8890
    class CTC_Villager;                                                  // Alloc 0x00DF90B0
    class CIsAGuard;                                                     // Alloc 0x00DF9640
};
#define FABLE_TCE_ENTITY_DECL(Name, Addr) \
class CQ_TraderConflictEvilScript::Name : public CEntityScriptBase { \
public: \
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderConflictEvilScript&, CScriptDataBase*, const CScriptThing&); \
};
FABLE_TCE_ENTITY_DECL(CTC_GuardSpawnPoint, 0x00DF7B20)
FABLE_TCE_ENTITY_DECL(CTC_BanditFollower, 0x00DF7F90)
FABLE_TCE_ENTITY_DECL(CTC_BanditFighter, 0x00DF8890)
FABLE_TCE_ENTITY_DECL(CTC_Villager, 0x00DF90B0)
FABLE_TCE_ENTITY_DECL(CIsAGuard, 0x00DF9640)
#undef FABLE_TCE_ENTITY_DECL
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_TraderConflictEvilScript) == 0x70);
#endif
#endif
