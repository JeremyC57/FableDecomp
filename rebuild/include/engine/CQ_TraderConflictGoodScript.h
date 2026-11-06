#ifndef FABLE_ENGINE_CQ_TRADERCONFLICTGOODSCRIPT_H
#define FABLE_ENGINE_CQ_TRADERCONFLICTGOODSCRIPT_H
// Retail opt-in (chunk 787): NScript::CQ_TraderConflictGoodScript, new header (no includer before chunk 787).
// Layout: Ego_r.pdb CQ_TraderConflictGoodScript (struct_layouts_egor.tsv, 128 bytes) at retail offsets: PDB - 0x14 up to AllCreatures
// (retail CLevelScriptBase is 0x48), PDB - 0x18 after it (the retail STLport vector is 12 bytes, Ego_r's Dinkumware one 16):
// AllCreatures +0x48 (the landed UpdateLiveEnemies 0x00DFC320 rebuilds it), TradersFollowing +0x54, TradersReachedTeleporter +0x58,
// BanditSecurityLinesSaid +0x5c, FirstRegionDead +0x60 .. MissionSucceeded +0x67 (Main 0x00DFA450 writes IntroDone +0x62 and
// FirstRegionDead +0x60, reads OutroDone +0x64; the chunk 782 AttackPeople 0x00DFD600 model reads EnteredNewRegion +0x61 and
// the two trader counts).
// Methods (Ego_r.pdb ?<Name>@CQ_TraderConflictGoodScript@NScript@@QAEXXZ unless noted): WatchTimeLimit 0x00DFAEB0, WatchForKilledPeople
// 0x00DFC290, UpdateLiveEnemies 0x00DFC320, WatchForHittingEnemies 0x00DFC630, WatchForRegionTransitions 0x00DFC920, WatchForTradersFreed
// 0x00DFCC10, AttackPeople 0x00DFD600, PlayCutscene(CCharString) 0x00DFDED0 (Ego_r/FableWin ?PlayCutscene@...@QAEXVCCharString@@@Z).
// Nested entity scripts: the seven classes Main binds (Ego_r.pdb ?Alloc@<Class>@CQ_TraderConflictGoodScript@NScript@@SI...; the binding
// name is the class name less its `C`), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, the one CLevelScriptBase instance in the Ego_r.pdb name set.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_TraderConflictGoodScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictGoodScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();                                                 // 0x00DFA450
    virtual void Init();                                                 // 0x00DFA0E0
    void WatchTimeLimit();                                               // 0x00DFAEB0
    void WatchForKilledPeople();                                         // 0x00DFC290
    void UpdateLiveEnemies();                                            // 0x00DFC320
    void WatchForHittingEnemies();                                       // 0x00DFC630
    void WatchForRegionTransitions();                                    // 0x00DFC920
    void WatchForTradersFreed();                                         // 0x00DFCC10
    void AttackPeople();                                                 // 0x00DFD600
    void PlayCutscene(CCharString);                                      // 0x00DFDED0
    std::vector<CScriptThing> AllCreatures;                              // +0x48
    long TradersFollowing;                                               // +0x54
    long TradersReachedTeleporter;                                       // +0x58
    long BanditSecurityLinesSaid;                                        // +0x5c
    bool FirstRegionDead;                                                // +0x60
    bool EnteredNewRegion;                                               // +0x61
    bool IntroDone;                                                      // +0x62
    bool OutroStart;                                                     // +0x63
    bool OutroDone;                                                      // +0x64
    bool OpenedCage;                                                     // +0x65
    bool CommentedOnBanditCostume;                                       // +0x66
    bool MissionSucceeded;                                               // +0x67
    class CTraderToRescue;                                               // Alloc 0x00DFB050
    class CTC_BanditGuard;                                               // Alloc 0x00DFB250
    class CTC_BanditHostageKeeper;                                       // Alloc 0x00DFB990
    class CBCMTrader;                                                    // Alloc 0x00DFBCF0
    class CBCGameMaster;                                                 // Alloc 0x00DFBDE0
    class CBanditExtra;                                                  // Alloc 0x00DFBED0
    class CCampHostageDoor;                                              // Alloc 0x00DFBFA0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_TraderConflictGoodScript) == 0x68);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_TRADERCONFLICTGOODSCRIPT_H
