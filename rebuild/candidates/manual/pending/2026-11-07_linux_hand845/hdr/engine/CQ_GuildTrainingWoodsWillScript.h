#ifndef FABLE_ENGINE_CQ_GUILDTRAININGWOODSWILLSCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGWOODSWILLSCRIPT_H
// Retail opt-in (chunk 845): NScript::CQ_GuildTrainingWoodsWillScript, new header (no includer before chunk 845; the landed sibling
// rows keep their local models).
// Layout: Ego_r.pdb CQ_GuildTrainingWoodsWillScript (struct_layouts_egor.tsv, 100 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a, WhisperAnimate +0x4b, BanditsAlive
// +0x4c, sizeof 0x50. Checked against retail: Main 0x00D67890 clears +0x48..+0x4b and sets +0x4c at entry, loops on +0x4c, sets
// +0x4b before the heal and +0x48 after the outro; the landed CWillWhisper::Main 0x00D68810 reads WhisperAnimate +0x4b and
// BanditsAlive +0x4c through ParentClass, the landed WatchForLeaving 0x00D68C50 MissionSucceeded +0x48 / MissionFailed +0x49.
// Retail methods (Ego_r.pdb / FableWin.pdb name set ?<Name>@CQ_GuildTrainingWoodsWillScript@NScript@@): RegisterMain 0x00D677D0
// (slot 1) and Main 0x00D67890 (slot 2) of the script vtable 0x012D1E10; WatchForTermination 0x00D68600 and DoMission 0x00D68AE0
// (spawned by Main under those thread-name literals; DoMission is in both PDB name sets), EndMission 0x00D68CD0. WatchForLeaving
// 0x00D68C50 is the catalog's name (not in the Ego_r name set: TLC-only, kept as landed).
// Nested entity script CWillWhisper (Ego_r.pdb, 28 bytes): Alloc 0x00D68720 (Main binds it as "WillWhisper"), Main 0x00D68810.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the landed CWillWhisper::Alloc 0x00D68720 and RegisterMain 0x00D677D0 models).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingWoodsWillScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D677D0 (slot 1)
    virtual void Main();                                                 // 0x00D67890 (slot 2)
    void WatchForTermination();                                          // 0x00D68600
    void DoMission();                                                    // 0x00D68AE0
    void WatchForLeaving();                                              // 0x00D68C50
    void EndMission();                                                   // 0x00D68CD0
    bool MissionSucceeded;                                               // +0x48
    bool MissionFailed;                                                  // +0x49
    bool MissionOver;                                                    // +0x4a
    bool WhisperAnimate;                                                 // +0x4b
    bool BanditsAlive;                                                   // +0x4c
    class CWillWhisper : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingWoodsWillScript&, CScriptDataBase*, const CScriptThing&); // 0x00D68720
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWoodsWillScript) == 0x50);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGWOODSWILLSCRIPT_H
