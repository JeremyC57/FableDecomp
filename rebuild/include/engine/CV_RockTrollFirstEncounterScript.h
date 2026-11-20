#ifndef FABLE_ENGINE_CV_ROCKTROLLFIRSTENCOUNTERSCRIPT_H
#define FABLE_ENGINE_CV_ROCKTROLLFIRSTENCOUNTERSCRIPT_H
// Retail opt-in (chunk 842): NScript::CV_RockTrollFirstEncounterScript, new header (no includer before chunk 842).
// Layout: Ego_r.pdb CV_RockTrollFirstEncounterScript (108 bytes) at retail offsets (PDB - 0x14 for the retail 0x48-byte
// CLevelScriptBase): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a, TrollAlive +0x4b, TrollAwake +0x4c,
// Activated +0x4d, RockTrollTriggered +0x4e, RockTrollHealthBarID +0x50, PlayedExhumeCutScene +0x54, AddedItemsToRockTroll +0x55;
// sizeof 0x58 (the landed Alloc 0x00EC5360). Checked against CRTFE_RockTroll::Main 0x00EC4A00 (reads / stores +0x54, +0x55).
// Methods (Ego_r.pdb name set): WatchForRockTrollKilled(CScriptThing) 0x00EC5010 and WatchForRockTrollHit(CScriptThing) 0x00EC5140
// (landed rows; QAEXVCScriptThing@@@Z; the "ParentClass." + "WatchForRockTroll..." CSpawnedFunc1 member pointers in 0x00EC4A00),
// WatchForRegionExit() (QAEXXZ), RegisterMain 0x00EC3CA0, OnPersist 0x00EC4970.
// Nested entity CRTFE_RockTroll: vtable 0x012F16C4 (0 scalar dtor 0x00EC45A0, 1 Main 0x00EC4A00, 2 Init 0x00EC4540, 3 GetParentScript
// 0x00EC4530, 4 OnPersist 0x00CDEBC0 (base), 5 OnPredicateFail 0x00EC4590, 6 OnInterrupted 0x00CDEBE0 (base)); ParentClass +0x14,
// PMasterData +0x18, sizeof 0x1c (Alloc 0x00EC4490). CRTFE_Sparrow / CM_RTFERockTrollTrigger forward-declared.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_RockTrollFirstEncounterScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RockTrollFirstEncounterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    typedef CV_RockTrollFirstEncounterScript TMyClass;
    typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
    CV_RockTrollFirstEncounterScript(CScriptDataBase*,CGameScriptInterfaceBase&);   // 0x00EC3C20
    virtual void RegisterMain();                                         // 0x00EC3CA0
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);                            // 0x00EC4970
    void WatchForRegionExit();
    void WatchForRockTrollKilled(CScriptThing);                          // 0x00EC5010
    void WatchForRockTrollHit(CScriptThing);                             // 0x00EC5140
    bool MissionSucceeded;                                               // +0x48
    bool MissionFailed;                                                  // +0x49
    bool MissionOver;                                                    // +0x4a
    bool TrollAlive;                                                     // +0x4b
    bool TrollAwake;                                                     // +0x4c
    bool Activated;                                                      // +0x4d
    bool RockTrollTriggered;                                             // +0x4e
    long RockTrollHealthBarID;                                           // +0x50
    bool PlayedExhumeCutScene;                                           // +0x54
    bool AddedItemsToRockTroll;                                          // +0x55
    class CM_RTFERockTrollTrigger;                                       // Alloc 0x00EC41F0
    class CRTFE_Sparrow;                                                 // Alloc 0x00EC45C0
    class CRTFE_RockTroll : public CEntityScriptBase {
    public:
        typedef CV_RockTrollFirstEncounterScript TParentClass;
        CRTFE_RockTroll(CV_RockTrollFirstEncounterScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_RockTrollFirstEncounterScript&, CScriptDataBase*, const CScriptThing&);   // 0x00EC4490
        virtual CScriptBase* GetParentScript() const;                    // 0x00EC4530
    private:
        virtual void Init();                                             // 0x00EC4540
        virtual void OnPredicateFail();                                  // 0x00EC4590
        virtual void Main();                                             // 0x00EC4A00
    protected:
        NScript::CV_RockTrollFirstEncounterScript & ParentClass;         // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_RockTrollFirstEncounterScript) == 0x58);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll) == 0x1c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_ROCKTROLLFIRSTENCOUNTERSCRIPT_H
