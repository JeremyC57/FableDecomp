#ifndef FABLE_ENGINE_CQ_WHITEBALVERINEWWSCRIPT_H
#define FABLE_ENGINE_CQ_WHITEBALVERINEWWSCRIPT_H
// Retail opt-in (chunk 797): NScript::CQ_WhiteBalverineWWScript, new header (no includer before chunk 797).
// Layout: Ego_r.pdb CQ_WhiteBalverineWWScript (struct_layouts_egor.tsv, 112 bytes) at retail offsets PDB - 0x14 (retail CLevelScriptBase
// is 0x48; no STLport container in the class): MissionFailed +0x48, MissionSucceeded +0x49, WhiteBalverineAlive +0x4a,
// WhiteBalverineHealth +0x4c, WhiteBalverine (CScriptThing) +0x50; 0x5c bytes. Checked against retail: the landed ctor 0x00E18440 builds
// the CScriptThing at +0x50, Init 0x00E18480 clears +0x48..+0x4a, MonitorBalverine 0x00E18EB0 reads +0x4a / +0x50, Main 0x00E18630 reads
// +0x48 / +0x49 and assigns +0x50 (PImp +0x54 / +0x58).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (Main writes MasterScriptData->WhiteBalverineFinished, +0xe1, the Ego_r PDB member).
// Methods (Ego_r.pdb ?<Name>@CQ_WhiteBalverineWWScript@NScript@@QAEXXZ unless noted): RegisterMain / Main / Init (UAEXXZ; vtable
// 0x012E0CE4 slots 1-3: 0x00E18570 / 0x00E18630 / 0x00E18480), MonitorBalverine 0x00E18EB0, SpawnBalverines 0x00E19560 (thread-name
// literals "MonitorBalverine" / "SpawnBalverines" next to each CSpawnedFunc in Main).
// Nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CQ_WhiteBalverineWWScript@NScript@@SI...; binding name = class name less its `C`):
// CKG_Chief 0x00E192E0, CWBWW_WhiteBalverine 0x00E190A0, CWBWW_SoldierBalverine 0x00E191E0 (all landed Alloc rows), forward-declared only.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_WhiteBalverineWWScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WhiteBalverineWWScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E18570
    virtual void Main();                                                 // 0x00E18630
    virtual void Init();                                                 // 0x00E18480
    void MonitorBalverine();                                             // 0x00E18EB0
    void SpawnBalverines();                                              // 0x00E19560
    bool MissionFailed;                                                  // +0x48
    bool MissionSucceeded;                                               // +0x49
    bool WhiteBalverineAlive;                                            // +0x4a
    long WhiteBalverineHealth;                                           // +0x4c
    CScriptThing WhiteBalverine;                                         // +0x50
    class CKG_Chief;                                                     // Alloc 0x00E192E0
    class CWBWW_WhiteBalverine;                                          // Alloc 0x00E190A0
    class CWBWW_SoldierBalverine;                                        // Alloc 0x00E191E0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_WhiteBalverineWWScript) == 0x5c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_WHITEBALVERINEWWSCRIPT_H
