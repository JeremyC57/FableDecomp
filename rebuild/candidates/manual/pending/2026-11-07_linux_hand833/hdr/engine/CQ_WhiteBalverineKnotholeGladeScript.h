#ifndef FABLE_ENGINE_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_H
#define FABLE_ENGINE_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_H
// Retail opt-in (chunk 799): NScript::CQ_WhiteBalverineKnotholeGladeScript, new header (no includer before chunk 799).
// Layout: Ego_r.pdb CQ_WhiteBalverineKnotholeGladeScript (struct_layouts_egor.tsv, 120 bytes) at retail offsets = PDB - 0x14
// (retail CLevelScriptBase is 0x48): MissionFailed +0x48, MissionSucceeded +0x49, WifeCutsceneStart +0x4a, WifeCutsceneStop
// +0x4b, GreenWife +0x4c, BalverineState +0x50, KG_Chief +0x54, QuestStartScreenShown +0x60 -- the offsets Main 0x00E13F10
// reads and writes (+0x50 compared and stored, +0x49 / +0x4a / +0x4b / +0x4c / +0x60 stored, +0x60 tested); the landed
// WatchForTermination 0x00E15500 reads +0x48 / +0x49. Retail methods (Ego_r.pdb ?<Name>@CQ_WhiteBalverineKnotholeGladeScript@
// NScript@@...): RegisterMain 0x00E13E20, OnPersist 0x00E13EE0, Main 0x00E13F10 (slot 2 of vtable 0x012E0768), WatchForTermination
// 0x00E15500 (spawned by Main under its thread-name literal), Init.
// Nested entity scripts: the six classes Main binds (Ego_r.pdb ?Alloc@<Class>@CQ_WhiteBalverineKnotholeGladeScript@NScript@@SI...;
// the binding-name literal is the class name less its `C`), forward-declared only (an includer completes the ones it needs).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40, MasterScriptData +0x44).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_WhiteBalverineKnotholeGladeScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WhiteBalverineKnotholeGladeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E13E20
    virtual void Main();                                                 // 0x00E13F10
    virtual void Init();
    virtual void OnPersist(CPersistContext&);                            // 0x00E13EE0
    void WatchForTermination();                                          // 0x00E15500
    bool MissionFailed;                                                  // +0x48
    bool MissionSucceeded;                                               // +0x49
    bool WifeCutsceneStart;                                              // +0x4a
    bool WifeCutsceneStop;                                               // +0x4b
    bool GreenWife;                                                      // +0x4c
    long BalverineState;                                                 // +0x50
    CScriptThing KG_Chief;                                               // +0x54
    bool QuestStartScreenShown;                                          // +0x60
    class CWB_WhiteBalverine;                                            // Alloc 0x00E15750
    class CWB_ScaredVillager;                                            // Alloc 0x00E16990
    class CWB_Villager;                                                  // Alloc 0x00E16B50
    class CWB_Guard1;                                                    // Alloc 0x00E16D10
    class CWB_Guard2;                                                    // Alloc 0x00E17230
    class CKG_Chief;                                                     // Alloc 0x00E17750
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_WhiteBalverineKnotholeGladeScript) == 0x64);
#ifdef FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_ENTITIES
// Retail opt-in (chunk 803): the white balverine entity script. Ego_r.pdb CWB_WhiteBalverine (struct_layouts_egor.tsv, 32 bytes:
// ParentClass +0x14, PMasterData +0x18, WBCounter +0x1c; entity offsets equal retail's, CEntityScriptBase is 0x14 in both; the
// landed Alloc 0x00E15750 allocates 0x20). Entity vtable 0x012E0A50: 0 scalar deleting dtor 0x00E15810, 1 Main 0x00E158F0,
// 2 Init 0x00E15830, 3 GetParentScript 0x00E157F0 (OnPredicateFail 0x00E15800); Main / Init private (PDB EAEXXZ).
class CScriptDataBase;
namespace NScript {
class CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_WhiteBalverineKnotholeGladeScript&, CScriptDataBase*, const CScriptThing&); // 0x00E15750
    virtual CScriptBase* GetParentScript() const;                        // 0x00E157F0
private:
    virtual void Main();                                                 // 0x00E158F0
    virtual void Init();                                                 // 0x00E15830
protected:
    CQ_WhiteBalverineKnotholeGladeScript& ParentClass;                   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long WBCounter;                                                      // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine) == 0x20);
#endif
#ifdef FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_CKG_CHIEF
// Retail opt-in (chunk 833): the CKG_Chief entity script (Ego_r.pdb CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief, 28 bytes = the landed
// Alloc 0x00E17750's new size: ParentClass +0x14, PMasterData +0x18). Entity vtable 0x012E0B98 (stored by the Alloc; read from retail .rdata):
// 0 scalar deleting dtor 0x00E17820, 1 Main 0x00E17840, 2 Init 0x00E17800, 3 GetParentScript 0x00E177F0, 4 OnPersist inherited
// (CEntityScriptBase 0x00CDEBC0), 5 OnPredicateFail 0x00E17810. Main / Init private (PDB ?Main@CKG_Chief@...@@EAEXXZ).
class CScriptDataBase;
namespace NScript {
class CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_WhiteBalverineKnotholeGladeScript&, CScriptDataBase*, const CScriptThing&); // 0x00E17750
    virtual CScriptBase* GetParentScript() const;                        // 0x00E177F0
private:
    virtual void Main();                                                 // 0x00E17840
    virtual void Init();                                                 // 0x00E17800
    virtual void OnPredicateFail();                                      // 0x00E17810
protected:
    CQ_WhiteBalverineKnotholeGladeScript& ParentClass;                   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief) == 0x1c);
#endif
#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_H
