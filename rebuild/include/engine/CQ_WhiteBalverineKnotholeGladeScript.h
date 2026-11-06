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
#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_H
