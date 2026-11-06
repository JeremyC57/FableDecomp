#ifndef FABLE_ENGINE_CV_FISHERMANSCRIPT_H
#define FABLE_ENGINE_CV_FISHERMANSCRIPT_H
// Retail opt-in (chunk 808): NScript::CV_FishermanScript, new header (no includer before chunk 808; the landed sibling rows keep
// their local models).
// Layout: Ego_r.pdb CV_FishermanScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets = PDB - 0x14 (retail CLevelScriptBase
// is 0x48): IntroPlayed +0x48, HeroHasSavedFishermanFromWasps +0x49, WaspsAlive +0x4C, FishermanMustRunAway +0x50, QuestCardGiven
// +0x51, FishermanTasks +0x54, sizeof 0x58. CFisherman::Main 0x00E7C8E0 reads every one of them through ParentClass.
// Script vtable 0x012EA024: RegisterMain 0x00E7B440, Main 0x00E7B4E0, Init 0x00E7B420, OnPersist 0x00E7C890 (all landed).
// Nested CFisherman (Ego_r.pdb, 32 bytes): CEntityScriptBase (0x14), CV_FishermanScript* ParentClass +0x14 (a pointer in the PDB),
// CQ_SunnyvaleMasterData* PMasterData +0x18, CTimer ChatterTimer +0x1C. Entity vtable 0x012EA0B4 (the landed Alloc 0x00E7B7B0 stores
// it): Main 0x00E7C8E0, Init 0x00E7BD60, GetParentScript 0x00E7B850; IdleChatter 0x00E7DE60 (Ego_r.pdb
// ?IdleChatter@CFisherman@CV_FishermanScript@NScript@@AAEXXZ, private).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
class CScriptDataBase;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E7B440 (slot 1)
    virtual void Main();                                                 // 0x00E7B4E0 (slot 2)
    virtual void Init();                                                 // 0x00E7B420 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00E7C890 (slot 4)
    void NPCAttitudeProcess(CScriptThing);                               // Ego_r.pdb ?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z
    bool IntroPlayed;                                                    // +0x48
    bool HeroHasSavedFishermanFromWasps;                                 // +0x49
    long WaspsAlive;                                                     // +0x4C
    bool FishermanMustRunAway;                                           // +0x50
    bool QuestCardGiven;                                                 // +0x51
    long FishermanTasks;                                                 // +0x54
    class CFisherman : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_FishermanScript&, CScriptDataBase*, const CScriptThing&); // 0x00E7B7B0
        virtual CScriptBase* GetParentScript() const;                    // 0x00E7B850
    protected:
        CV_FishermanScript* ParentClass;                                 // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        CTimer ChatterTimer;                                             // +0x1C
        virtual void Init();                                             // 0x00E7BD60
        virtual void Main();                                             // 0x00E7C8E0
        void IdleChatter();                                              // 0x00E7DE60
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_FishermanScript) == 0x58);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_FishermanScript::CFisherman) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_FISHERMANSCRIPT_H
