#ifndef FABLE_ENGINE_CQ_MINIONCLIFFTOPCHASESCRIPT_H
#define FABLE_ENGINE_CQ_MINIONCLIFFTOPCHASESCRIPT_H
// Retail opt-in (chunk 787): NScript::CQ_MinionClifftopChaseScript, new header (no includer before chunk 787).
// Chunk 797: opt-in FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MAPKEEPER adds the CMapKeeper class (after the main class).
// Layout: Ego_r.pdb CQ_MinionClifftopChaseScript (struct_layouts_egor.tsv, 128 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): bools +0x48..+0x4e, KilledGuardingMinions +0x50, RockTrollAppeared +0x54, MCC4Taken +0x55,
// MakeUniqueComment +0x58, MissionTimer +0x5c, MissionTimerID +0x60, ReasonForFailure +0x64, MinionCount +0x68 -- the offsets the
// landed Init 0x00DA3100 writes. Retail methods: Init 0x00DA3100, Main 0x00DA34B0 (slot 2), WatchForTermination 0x00DA3F40,
// ProcessMission 0x00DA8510 (both spawned by Main under their thread-name literals; Ego_r.pdb ?<Name>@CQ_MinionClifftopChaseScript@NScript@@QAEXXZ).
// Nested entity scripts: the twenty classes Main binds (Ego_r.pdb ?Alloc@<Class>@CQ_MinionClifftopChaseScript@NScript@@SI...; the
// binding-name literal is the class name less its `C`), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, the one CLevelScriptBase instance in the Ego_r.pdb name set.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_MinionClifftopChaseScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
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
class CQ_MinionClifftopChaseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();                                                 // 0x00DA34B0
    virtual void Init();                                                 // 0x00DA3100
    void WatchForTermination();                                          // 0x00DA3F40
    void ProcessMission();                                               // 0x00DA8510
    bool MapKeeperSaidDialog;                                            // +0x48
    bool MissionFailed;                                                  // +0x49
    bool MissionSucceeded;                                               // +0x4a
    bool QuestStartScreened;                                             // +0x4b
    bool KilledMinionFieldGenerator;                                     // +0x4c
    bool IsIntroCSFinished;                                              // +0x4d
    bool PointedToPrisonPath;                                            // +0x4e
    long KilledGuardingMinions;                                          // +0x50
    bool RockTrollAppeared;                                              // +0x54
    bool MCC4Taken;                                                      // +0x55
    CTimer MakeUniqueComment;                                            // +0x58
    CTimer MissionTimer;                                                 // +0x5c
    long MissionTimerID;                                                 // +0x60
    long ReasonForFailure;                                               // +0x64
    long MinionCount;                                                    // +0x68
    class CMapKeeper;                                                    // Alloc 0x00DA4250
    class CMinionEscort;                                                 // Alloc 0x00DA4490
    class CMinionEscortEnd;                                              // Alloc 0x00DA4690
    class CMinionWarningGuard;                                           // Alloc 0x00DA4920
    class CBalverineFightingGuard;                                       // Alloc 0x00DA4B80
    class CMagicBarrier;                                                 // Alloc 0x00DA4D40
    class CMagicBarrierGraveyard;                                        // Alloc 0x00DA53A0
    class CMinionSpawner;                                                // Alloc 0x00DA58B0
    class CMinionAttackingGuard;                                         // Alloc 0x00DA4A30
    class CMinionFieldGenerator;                                         // Alloc 0x00DA5B40
    class CCorneredGuard;                                                // Alloc 0x00DA5C90
    class CMinionTrollSummoner;                                          // Alloc 0x00DA5DA0
    class CRockTroll;                                                    // Alloc 0x00DA5E70
    class CMinionSpawnAdv;                                               // Alloc 0x00DA60A0
    class CM_Minion;                                                     // Alloc 0x00DA6170
    class CM_Guard;                                                      // Alloc 0x00DA6320
    class CGuardToLeadTheWay;                                            // Alloc 0x00DA6560
    class CM_NotThisWay;                                                 // Alloc 0x00DA6670
    class CGuardNotThisWay;                                              // Alloc 0x00DA6810
    class CInvisibleCage;                                                // Alloc 0x00DA57A0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_MinionClifftopChaseScript) == 0x6c);
#ifdef FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MAPKEEPER
// Opt-in (chunk 797): the CMapKeeper entity script (Ego_r.pdb CMapKeeper, 44 bytes: ParentClass +0x14, PMasterData +0x18,
// DoneIntro +0x1c, ImprisonedFX +0x20; the offsets the landed Init 0x00DA4350 / OnPredicateFail 0x00DA4300 use). Entity vtable
// 0x012D73E0 (stored by the landed Alloc 0x00DA4250): slot 1 Main 0x00DA91A0, slot 2 Init 0x00DA4350, slot 5 OnPredicateFail.
namespace NScript {
class CQ_MinionClifftopChaseScript::CMapKeeper : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_MinionClifftopChaseScript&, CScriptDataBase*, const CScriptThing&); // 0x00DA4250
    virtual CScriptBase* GetParentScript() const;                        // 0x00DA42F0
private:
    virtual void Main();                                                 // 0x00DA91A0
    virtual void Init();                                                 // 0x00DA4350
    virtual void OnPredicateFail();                                      // 0x00DA4300
protected:
    CQ_MinionClifftopChaseScript& ParentClass;                           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool DoneIntro;                                                      // +0x1c
    CScriptThing ImprisonedFX;                                           // +0x20
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_MinionClifftopChaseScript::CMapKeeper) == 0x2c);
#endif

#ifdef FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MINIONSPAWNADV
// Retail opt-in (chunk 841): CMinionSpawnAdv, Ego_r.pdb 28 bytes (ParentClass +0x14, PMasterData +0x18; the Alloc 0x00DA60A0); entity
// vtable 0x012D75F0: slot 0 scalar dtor 0x00DA6150, slot 1 Main 0x00DA8BF0, slot 2 Init = CEntityScriptBase's 0x00CDEBB0 (no override),
// slot 3 GetParentScript 0x00DA6140. PDB names (Ego_r + FableWin): ?Main@CMinionSpawnAdv@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ
// (private), ?GetParentScript@...@UBEPAVCScriptBase@@XZ, ?Alloc@...@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
namespace NScript {
class CQ_MinionClifftopChaseScript::CMinionSpawnAdv : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_MinionClifftopChaseScript&, CScriptDataBase*, const CScriptThing&); // 0x00DA60A0
    virtual CScriptBase* GetParentScript() const;                        // 0x00DA6140
private:
    virtual void Main();                                                 // 0x00DA8BF0
protected:
    CQ_MinionClifftopChaseScript& ParentClass;                           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_MinionClifftopChaseScript::CMinionSpawnAdv) == 0x1c);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_MINIONCLIFFTOPCHASESCRIPT_H
