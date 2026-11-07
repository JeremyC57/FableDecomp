#ifndef FABLE_ENGINE_CQ_HOBBETOOTHCONTESTSCRIPT_H
#define FABLE_ENGINE_CQ_HOBBETOOTHCONTESTSCRIPT_H
// Retail opt-in (chunk 799): NScript::CQ_HobbeToothContestScript, new header (no includer before chunk 799).
// Layout: Ego_r.pdb CQ_HobbeToothContestScript (struct_layouts_egor.tsv, 124 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): the four counters +0x48..+0x54, the six bools +0x58..+0x5d, WhisperCommentTimer +0x60,
// QuestStartScreenShown +0x64 -- the offsets Main 0x00D9A190 reads and writes (+0x4c / +0x50 compared, +0x58 / +0x59 / +0x5b /
// +0x5c / +0x64 stored, +0x5d tested). Retail methods (Ego_r.pdb ?<Name>@CQ_HobbeToothContestScript@NScript@@...): RegisterMain
// 0x00D9A0A0, OnPersist 0x00D9A160, Main 0x00D9A190 (slot 2), WatchForTermination 0x00D9B2F0 (spawned by Main under its
// thread-name literal), DisplayTimer 0x00D9B420, HeroKilledHobbe 0x00D9B8F0, WhisperKilledHobbe 0x00D9BB50.
// Nested entity scripts: the seven classes Main binds (Ego_r.pdb ?Alloc@<Class>@CQ_HobbeToothContestScript@NScript@@SI...; the
// binding-name literal is the class name less its `C`), forward-declared only (an includer completes the ones it needs).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (MasterScriptData +0x44; Main sets its HobbeContestKillMoreHobbes +0x11B).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_HobbeToothContestScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeToothContestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D9A0A0
    virtual void Main();                                                 // 0x00D9A190
    virtual void Init();
    virtual void OnPersist(CPersistContext&);                            // 0x00D9A160
    void WatchForTermination();                                          // 0x00D9B2F0
    void DisplayTimer();                                                 // 0x00D9B420
    void HeroKilledHobbe();                                              // 0x00D9B8F0
    void WhisperKilledHobbe();                                           // 0x00D9BB50
    void CheckToSeeIfWhisperHit(CScriptThing);
    long NumberOfHobbesInTotal;                                          // +0x48
    long NumberOfHobbeTeethCollectedByHero;                              // +0x4c
    long NumberOfHobbeTeethCollectedByWhisper;                           // +0x50
    long NumberOfHobbeTeethNeeded;                                       // +0x54
    bool CS_IntroDone;                                                   // +0x58
    bool CS_OutroBegin;                                                  // +0x59
    bool DrinkingHobbesNotHit;                                           // +0x5a
    bool MissionSucceeded;                                               // +0x5b
    bool MissionFailed;                                                  // +0x5c
    bool WhisperOrFarmerHit;                                             // +0x5d
    CTimer WhisperCommentTimer;                                          // +0x60
    bool QuestStartScreenShown;                                          // +0x64
    class CQ_HobbeToothHobbe;                                            // Alloc 0x00D9B7C0
    class CDrinkingHobbe;                                                // Alloc 0x00D9BE30
    class CDrinkersReactMarker;                                          // Alloc 0x00D9C1D0
    class CHobbeBarrel;                                                  // Alloc 0x00D9C3C0
    class COrchardFarmFarmer;                                            // Alloc 0x00D9C560
    class CSpawnNewHobbe;                                                // Alloc 0x00D9CE80
    class CQ_HobbeToothWhisper;                                          // Alloc 0x00D9CF60
};
#ifdef FABLE_RETAIL_CQ_HOBBETOOTHCONTESTSCRIPT_CSPAWNNEWHOBBE
// Retail opt-in (chunk 838): the nested entity script CSpawnNewHobbe (Ego_r.pdb, 0x1c: ParentClass +0x14, PMasterData +0x18,
// struct_layouts_egor.tsv; the landed Alloc 0x00D9CE80 allocates 0x1c). Methods: Ego_r.pdb ?Main@CSpawnNewHobbe@...@EAEXXZ (private
// virtual, vtable 0x012D6BB8 slot 1, 0x00D9DAF0), GetParentScript 0x00D9CF20 (slot 3), Alloc 0x00D9CE80.
class CQ_HobbeToothContestScript::CSpawnNewHobbe : public CEntityScriptBase {
public:
    typedef CQ_HobbeToothContestScript TParentClass;
    CSpawnNewHobbe(CQ_HobbeToothContestScript&, CScriptDataBase*, const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HobbeToothContestScript&, CScriptDataBase*, const CScriptThing&); // 0x00D9CE80
    virtual CScriptBase* GetParentScript() const;                        // 0x00D9CF20
    CQ_SunnyvaleMasterData* GetMasterData();
protected:
    CQ_HobbeToothContestScript& ParentClass;                             // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    virtual void Main();                                                 // 0x00D9DAF0
};
#endif
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HobbeToothContestScript) == 0x68);
#ifdef FABLE_RETAIL_CQ_HOBBETOOTHCONTESTSCRIPT_CSPAWNNEWHOBBE
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe) == 0x1c);
#endif
#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HOBBETOOTHCONTESTSCRIPT_H
