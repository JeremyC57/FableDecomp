#ifndef FABLE_ENGINE_CQ_GUILDTRAININGWOODSWILLSCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGWOODSWILLSCRIPT_H
// Retail model (new header, B24A11 2026-11-24, from the chunk-845 draft header): NScript::CQ_GuildTrainingWoodsWillScript.
// Layout: Ego_r.pdb CQ_GuildTrainingWoodsWillScript (struct_layouts_egor.tsv, 100 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a, WhisperAnimate +0x4b, BanditsAlive
// +0x4c, sizeof 0x50. Checked against retail: Main 0x00D67890 clears +0x48..+0x4b and sets +0x4c at entry, loops on +0x4c, sets
// +0x4b before the heal and +0x48 after the outro; the landed CWillWhisper::Main 0x00D68810 reads WhisperAnimate +0x4b and
// BanditsAlive +0x4c through ParentClass, the landed WatchForLeaving 0x00D68C50 MissionSucceeded +0x48 / MissionFailed +0x49.
// Methods (Ego_r.pdb / FableWin.pdb ?<Name>@CQ_GuildTrainingWoodsWillScript@NScript@@): RegisterMain 0x00D677D0 (slot 1) and Main
// 0x00D67890 (slot 2) of the script vtable 0x012D1E10; WatchForTermination 0x00D68600 and DoMission 0x00D68AE0 (spawned by Main
// under those thread-name literals), WatchForLeaving 0x00D68C50, EndMission 0x00D68CD0. InitialiseVariables (FableWin 0x03774DC0
// ?InitialiseVariables@...@QAEXXZ: the five flag stores in member order, BanditsAlive = true) has no retail copy: retail expands it
// at the top of Main, so its body is here, inline.
// Nested entity script CWillWhisper (Ego_r.pdb, 28 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18): Alloc
// 0x00D68720 (Main binds it as "WillWhisper"), Main 0x00D68810 (FableWin ?Alloc@ / ?Main@CWillWhisper@...).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (as the landed CWillWhisper::Alloc 0x00D68720 and RegisterMain 0x00D677D0 models).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_GuildTrainingWoodsWillScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
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
    void InitialiseVariables()                                           // expanded in Main (FableWin 0x03774DC0)
    {
        MissionSucceeded = false;
        MissionFailed = false;
        MissionOver = false;
        WhisperAnimate = false;
        BanditsAlive = true;
    }
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
    protected:
        NScript::CQ_GuildTrainingWoodsWillScript & ParentClass;          // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWoodsWillScript) == 0x50);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWoodsWillScript::CWillWhisper) == 0x1c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGWOODSWILLSCRIPT_H
