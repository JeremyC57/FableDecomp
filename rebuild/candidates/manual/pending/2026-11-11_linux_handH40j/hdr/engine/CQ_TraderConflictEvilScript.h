#ifndef FABLE_ENGINE_CQ_TRADERCONFLICTEVILSCRIPT_H
#define FABLE_ENGINE_CQ_TRADERCONFLICTEVILSCRIPT_H
// Retail opt-in (chunk 811): NScript::CQ_TraderConflictEvilScript, new header (the CQ_TraderConflictGoodScript.h model).
// Layout: Ego_r.pdb CQ_TraderConflictEvilScript (struct_layouts_egor.tsv, 136 bytes) at retail offsets: PDB - 0x14 up to AllCreatures
// (retail CLevelScriptBase is 0x48), PDB - 0x18 after it (the retail STLport vector is 12 bytes, Ego_r's Dinkumware one 16):
// AllCreatures +0x48, CounterID +0x54, NumberSpawned +0x58, InitialNumberInRegion +0x5c, QuestStartScreened +0x60,
// NextTimeToSpawnGuards +0x64, MissionSucceeded +0x68, HeroAttackedBandit +0x69, PlayerEngaged +0x6a, ScreamOutTimer +0x6c;
// 0x70 bytes, as the landed Alloc 0x00DFA020 (`push 70h`) allocates. Vtable 0x012DE6A4 (slot 1 RegisterMain 0x00DF5F50, slot 2 Main
// 0x00DF6010).
// Methods (Ego_r.pdb ?<Name>@CQ_TraderConflictEvilScript@NScript@@QAEXXZ unless noted): Init 0x00DF5CD0 (UAEXXZ), WatchTimeLimit
// 0x00DF7980, UpdateLiveEnemies 0x00DF9B80, PlayCutscene(CCharString) 0x00DF9E00 (?PlayCutscene@...@QAEXVCCharString@@@Z).
// Nested entity scripts: the five classes Main binds (Ego_r.pdb ?Alloc@<Class>@CQ_TraderConflictEvilScript@NScript@@SI...; the binding
// name is the class name less its `C`), forward-declared only.
// Behind FABLE_RETAIL_NSCRIPT with no #error branch (a TU without the opt-in sees nothing).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00DF5F50
    virtual void Main();                                                 // 0x00DF6010
    virtual void Init();                                                 // 0x00DF5CD0
    void WatchTimeLimit();                                               // 0x00DF7980
    void UpdateLiveEnemies();                                            // 0x00DF9B80
    void PlayCutscene(CCharString);                                      // 0x00DF9E00
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
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_TraderConflictEvilScript) == 0x70);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_TRADERCONFLICTEVILSCRIPT_H
