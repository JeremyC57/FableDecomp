#ifndef FABLE_ENGINE_CV_MAYORSINVITATIONSCRIPT_H
#define FABLE_ENGINE_CV_MAYORSINVITATIONSCRIPT_H
// Retail opt-in (chunk 789): NScript::CV_MayorsInvitationScript, new header (no includer before chunk 789; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CV_MayorsInvitationScript (struct_layouts_egor.tsv, 128 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): MissionSucceeded +0x48 .. destinationHSP +0x60, sizeof 0x6C (the
// landed Alloc 0x00EBCC10 news 0x6c bytes). Checked against retail: CMI_InfoKeeper::Main 0x00EB8900 reads HeroLearnedSecret +0x4c,
// SecretsLearned +0x54 (fild, inc) and WhichQuest +0x58, and writes HeroLearnedSecret.
// Nested CMI_InfoKeeper (Ego_r.pdb, 36 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, ChainNumber +0x1c,
// HaveSpoken +0x20 (Main 0x00EB8900 writes ChainNumber from GFCharStringToInt and tests / sets HaveSpoken).
// Retail methods: the Ego_r.pdb name set ?<Name>@CV_MayorsInvitationScript@NScript@@...; vtable slots from the landed rows.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_MayorsInvitationScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MayorsInvitationScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00EB6CB0 (slot 1)
    virtual void Main();                                                 // 0x00EB6D70 (slot 2)
    virtual void Init();                                                 // 0x00EB6BD0 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00EB98A0
    void DoMission();                                                    // 0x00EB75F0
    void MonitorParanoidQuest();
    void MonitorThunderHeroDuel();                                       // 0x00EB70F0
    void WatchExposeMayorProgress();                                     // 0x00EB94A0
    void WatchForTermination();                                          // 0x00EB7370
    bool MissionSucceeded;                                               // +0x48
    bool MissionFailed;                                                  // +0x49
    bool MissionOver;                                                    // +0x4a
    bool HeroAbortedQuest;                                               // +0x4b
    bool HeroLearnedSecret;                                              // +0x4c
    bool HeroReturnedFromParanoidQuest;                                  // +0x4d
    bool HeroHasDefeatedThunder;                                         // +0x4e
    bool HeroHasLostToThunder;                                           // +0x4f
    bool QuestCardGiven;                                                 // +0x50
    long SecretsLearned;                                                 // +0x54
    long WhichQuest;                                                     // +0x58
    long MonitorThunderHeroDuelRunning;                                  // +0x5c
    CScriptThing destinationHSP;                                         // +0x60
    class CMI_InfoKeeper : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MayorsInvitationScript&, CScriptDataBase*, const CScriptThing&); // 0x00EB8810
        virtual CScriptBase* GetParentScript() const;                    // 0x00EB88B0
    protected:
        CV_MayorsInvitationScript& ParentClass;                          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        long ChainNumber;                                                // +0x1c
        bool HaveSpoken;                                                 // +0x20
        virtual void Init();                                             // 0x00EB88C0
        virtual void OnPersist(CPersistContext&);                        // 0x00EB9950
        virtual void OnPredicateFail();                                  // 0x00EB88D0
        virtual void Main();                                             // 0x00EB8900
    };
    class CLadyGrey;                                                     // Alloc 0x00EB78B0
    class CWiseVillager;                                                 // Alloc 0x00EB79F0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MayorsInvitationScript) == 0x6C);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MayorsInvitationScript::CMI_InfoKeeper) == 0x24);
#ifdef FABLE_RETAIL_CV_MAYORSINVITATIONSCRIPT_LADYGREY
// Retail opt-in (chunk 818): the nested entity script CLadyGrey (Ego_r.pdb / FableWin.pdb ??0CLadyGrey@CV_MayorsInvitationScript@NScript@@
// QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z, ?Main / ?Init / ?OnPredicateFail@CLadyGrey@..@@EAEXXZ). Entity vtable 0x012F05F4 (stored
// by the landed Alloc 0x00EB78B0 at 0x00EB78E6): slot 0 scalar deleting dtor 0x00EB79D0, 1 Main 0x00EB9980, 2 Init 0x00EB7960, 3
// GetParentScript 0x00EB7950. Layout (32 bytes = the Alloc's new size): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18,
// HeroFollowed +0x1c (the landed Alloc / Init: Init clears +0x1c). Main reads ParentClass +0x48..+0x58 and PMasterData +0x63 / +0xd5.
// Defined out of the class body so the default path keeps its forward declaration unchanged.
namespace NScript {
class CV_MayorsInvitationScript::CLadyGrey : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MayorsInvitationScript&, CScriptDataBase*, const CScriptThing&); // 0x00EB78B0
    virtual CScriptBase* GetParentScript() const;                        // 0x00EB7950
protected:
    CV_MayorsInvitationScript& ParentClass;                              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    bool HeroFollowed;                                                   // +0x1c
    virtual void Init();                                                 // 0x00EB7960
    virtual void OnPredicateFail();
    virtual void Main();                                                 // 0x00EB9980
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MayorsInvitationScript::CLadyGrey) == 0x20);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_MAYORSINVITATIONSCRIPT_H
