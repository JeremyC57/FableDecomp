#ifndef FABLE_ENGINE_CQ_GUILDTRAININGWOODSDEPARTURESCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGWOODSDEPARTURESCRIPT_H
// Retail opt-in (chunk 810; CFinalMaze added by chunk 814): NScript::CQ_GuildTrainingWoodsDepartureScript, new header (the landed rows of this script carry local
// models). Layout: Ego_r.pdb CQ_GuildTrainingWoodsDepartureScript (struct_layouts_egor.tsv, 100 bytes) at retail offsets = PDB - 0x14
// (retail CLevelScriptBase is 0x48; no STLport container in the class): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver
// +0x4a, DepartureMissionPoint +0x4c, sizeof 0x50 (the landed Alloc 0x00D664D0 news 0x50).
// Script vtable 0x012D15FC (Alloc 0x00D664D0, "Q_GuildTrainingWoodsDeparture"): RegisterMain 0x00D61C60, Main 0x00D61D20.
// Nested CArtifactThief (Ego_r.pdb, 32 bytes = the retail Alloc new size): CEntityScriptBase (0x14), ParentClass +0x14 (a pointer in
// the PDB; the PDB ctor ??0CArtifactThief@..@QAE@AAV12@.. takes the parent by reference), PMasterData +0x18, HoldingArtifact +0x1c,
// AlreadyTalkedTo +0x1d, NotAttacked +0x1e. Entity vtable 0x012D16DC (stored by the landed Alloc 0x00D62360): 0 ??_G 0x00D62460,
// 1 Main 0x00D62480, 2 Init 0x00D62410, 3 GetParentScript 0x00D62400, 4 the CEntityScriptBase default 0x00CDEBC0, 5 OnPredicateFail
// 0x00D62450, 6 the default 0x00CDEBE0. Checked against retail: Main 0x00D62480 reads and writes +0x1c / +0x1d / +0x1e.
// Methods: the Ego_r.pdb / FableWin.pdb name set ?<Name>@CArtifactThief@CQ_GuildTrainingWoodsDepartureScript@NScript@@...
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingWoodsDepartureScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D61C60 (slot 1)
    virtual void Main();                                                 // 0x00D61D20 (slot 2)
    bool MissionSucceeded;                                               // +0x48
    bool MissionFailed;                                                  // +0x49
    bool MissionOver;                                                    // +0x4a
    long DepartureMissionPoint;                                          // +0x4c
    // Chunk 814: nested CFinalMaze (Ego_r.pdb, 36 bytes = the landed Alloc 0x00D62270 new size): CEntityScriptBase (0x14),
    // ParentClass +0x14 (pointer), PMasterData +0x18, NotFighting +0x1c, NotBeaten +0x1d, BeenHit +0x20. Entity vtable 0x012D16C0
    // (stored by the Alloc at 0x00D622A6): 0 ??_G 0x00D62340, 1 Main 0x00D647F0, 2 Init 0x00D62320, 3 GetParentScript 0x00D62310,
    // 4 the CEntityScriptBase default 0x00CDEBC0, 5 OnPredicateFail 0x00D62330. Checked against retail: Main 0x00D647F0 writes
    // +0x1c / +0x1d / +0x20 and the parent's MissionSucceeded +0x48.
    class CFinalMaze : public CEntityScriptBase {
    public:
        CFinalMaze(CQ_GuildTrainingWoodsDepartureScript&, CScriptDataBase*, const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingWoodsDepartureScript&, CScriptDataBase*, const CScriptThing&); // 0x00D62270
        virtual CScriptBase* GetParentScript() const;                    // 0x00D62310
    protected:
        CQ_GuildTrainingWoodsDepartureScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        bool NotFighting;                                                // +0x1c
        bool NotBeaten;                                                  // +0x1d
        long BeenHit;                                                    // +0x20
        virtual void Init();                                             // 0x00D62320
        virtual void OnPredicateFail();                                  // 0x00D62330
        virtual void Main();                                             // 0x00D647F0
    };
    class CScorpionHome;                                                 // Main 0x00D643A0
    class CArtifactThief : public CEntityScriptBase {
    public:
        CArtifactThief(CQ_GuildTrainingWoodsDepartureScript&, CScriptDataBase*, const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingWoodsDepartureScript&, CScriptDataBase*, const CScriptThing&); // 0x00D62360
        virtual CScriptBase* GetParentScript() const;                    // 0x00D62400
    protected:
        CQ_GuildTrainingWoodsDepartureScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        bool HoldingArtifact;                                            // +0x1c
        bool AlreadyTalkedTo;                                            // +0x1d
        bool NotAttacked;                                                // +0x1e
        virtual void Init();                                             // 0x00D62410
        virtual void OnPredicateFail();                                  // 0x00D62450
        virtual void Main();                                             // 0x00D62480
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWoodsDepartureScript) == 0x50);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWoodsDepartureScript::CArtifactThief) == 0x20);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze) == 0x24);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGWOODSDEPARTURESCRIPT_H
