#ifndef FABLE_ENGINE_CQ_UNDEADRISINGSCRIPT_H
#define FABLE_ENGINE_CQ_UNDEADRISINGSCRIPT_H
// Retail opt-in (chunk 807): NScript::CQ_UndeadRisingScript, new header (no includer before chunk 807; the 42 landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CQ_UndeadRisingScript (struct_layouts_egor.tsv, 148 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), a further - 4 after the first STLport vector (0xC, Dinkumware 0x10):
// number_of_zombie_tokens_in_right_area +0x48, number_of_boss_kills_in_right_area +0x4c, HeroReachedCentre +0x50,
// BossCutscenePlayed +0x51, MissionSucceeded +0x52, MissionFailed +0x53, LightsActivated[8] +0x54, quest_status +0x5c,
// activation_points +0x60, boss_points +0x6c, sizeof 0x78. Checked against retail: Main 0x00E0B160 tests HeroReachedCentre at
// [esi+50h], stores quest_status 1/2/3 at [esi+5Ch], fills activation_points at lea [esi+60h] and boss_points at lea [esi+6Ch]
// and compares their size() with +0x48 / +0x4c; the landed LampChecker 0x00E0C4C0 reads LightsActivated +0x54 and
// activation_points +0x60; the thread 0x00E0C350 (WatchForTermination) waits on +0x52 / +0x53.
// quest_status: the PDB type is unnamed (<unnamed-type-quest_status>, no enumerator names in the dumps); the enumerators below
// are descriptive (Main stores 1, 2 and 3).
// Retail methods (Ego_r.pdb / FableWin.pdb name set ?<Name>@CQ_UndeadRisingScript@NScript@@): RegisterMain 0x00E0B0A0 (slot 1),
// Main 0x00E0B160 (slot 2), Init 0x00E0B020 (slot 3) of the script vtable 0x012DFD3C that the landed ctor 0x00E0AFE0 stores;
// WatchForTermination 0x00E0C350 and LampChecker 0x00E0C4C0 (spawned by Main under those thread-name literals),
// ActivateNextLamp 0x00E0E040, LookAfterHealth(CScriptThing) 0x00E0DBB0.
// Nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CQ_UndeadRisingScript@NScript@@...; Main binds each under its class name less the
// `C`): CUndeadRisingFirefly (Alloc 0x00E0C970), CUndeadRisingZombieAttacker (0x00E0CF20), CUndeadRisingZombieAttackerBoss
// (0x00E0D590), CUndeadRisingBoss (0x00E0D130), CUndeadRisingCentre (0x00E0D760), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the landed CreateActiveEntityScript 0x00E0E5A0 reads the master data at +0x44).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_UndeadRisingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E0B0A0 (slot 1)
    virtual void Main();                                                 // 0x00E0B160 (slot 2)
    virtual void Init();                                                 // 0x00E0B020 (slot 3)
    void WatchForTermination();                                          // 0x00E0C350
    void LampChecker();                                                  // 0x00E0C4C0
    void ActivateNextLamp();                                             // 0x00E0E040
    void LookAfterHealth(CScriptThing);                                  // 0x00E0DBB0
    unsigned long number_of_zombie_tokens_in_right_area;                 // +0x48
    unsigned long number_of_boss_kills_in_right_area;                    // +0x4c
    bool HeroReachedCentre;                                              // +0x50
    bool BossCutscenePlayed;                                             // +0x51
    bool MissionSucceeded;                                               // +0x52
    bool MissionFailed;                                                  // +0x53
    bool LightsActivated[8];                                             // +0x54
    enum { QUEST_STATUS_NOT_STARTED, QUEST_STATUS_COLLECTING_TOKENS, QUEST_STATUS_FIGHTING_BOSSES, QUEST_STATUS_COMPLETED } quest_status;   // +0x5c
    std::vector<CScriptThing> activation_points;                         // +0x60
    std::vector<CScriptThing> boss_points;                               // +0x6c
    class CUndeadRisingFirefly;                                          // Alloc 0x00E0C970
    class CUndeadRisingZombieAttacker;                                   // Alloc 0x00E0CF20
    class CUndeadRisingZombieAttackerBoss;                               // Alloc 0x00E0D590
    class CUndeadRisingBoss;                                             // Alloc 0x00E0D130
    class CUndeadRisingCentre;                                           // Alloc 0x00E0D760
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_UndeadRisingScript) == 0x78);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_UNDEADRISINGSCRIPT_H
