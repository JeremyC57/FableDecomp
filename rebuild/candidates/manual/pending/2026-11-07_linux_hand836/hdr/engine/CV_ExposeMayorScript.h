#ifndef FABLE_ENGINE_CV_EXPOSEMAYORSCRIPT_H
#define FABLE_ENGINE_CV_EXPOSEMAYORSCRIPT_H
// Retail opt-in (chunk 789): NScript::CV_ExposeMayorScript, new header (no includer before chunk 789; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CV_ExposeMayorScript (struct_layouts_egor.tsv, 180 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): Path +0x48 .. MaxFlashDistFromStables +0x9c, sizeof 0xA0.
// Checked against retail: the landed CEM_FlashLightPoint::Main 0x00E76920 reads StageNumber +0x4c, HeroFlashedLightsCorrectly
// +0x50, ResetLamp +0x52, MaxFlashDistFromStables +0x9c; CEM_SuspiciousVillager::Main 0x00E776F0 reads Path +0x48, writes
// PrisonerTalkedAboutLamp +0x51 and scripts seh_Suspicious +0x58.
// Nested CEM_SuspiciousVillager (Ego_r.pdb, 32 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18,
// DoneIntro +0x1c, DoneOutro +0x1d (the Alloc 0x00E76360 news 0x20; Main 0x00E776F0 reads / writes +0x1c / +0x1d).
// Retail methods: the Ego_r.pdb name set ?<Name>@CV_ExposeMayorScript@NScript@@...; PlayCutscene 0x00E78100
// (?PlayCutscene@CV_ExposeMayorScript@NScript@@QAEXVCCharString@@_N0@Z), Main 0x00E75BA0, CheckLampSlot 0x00E76CA0,
// HeroFlashesLamp 0x00E76E90.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_ExposeMayorScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"   // needs FABLE_RETAIL_BASE_OBJECT
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ExposeMayorScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum PATH_ID { };                                                    // Ego_r.pdb enum (enumerators not recovered)
    enum STAGE_IN_PATH { };                                              // Ego_r.pdb enum (enumerators not recovered)
    virtual void RegisterMain();                                         // 0x00E756D0 (slot 1)
    virtual void Main();                                                 // 0x00E75BA0 (slot 2)
    virtual void Init();                                                 // 0x00E75680 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00E75790
    void CheckLampSlot();                                                // 0x00E76CA0
    void DoMission();                                                    // 0x00E7A410
    void EndMission();                                                   // 0x00E7B050
    void GossipSetup();
    void HeroFlashesLamp(long);                                          // 0x00E76E90
    void PhaseOne();                                                     // 0x00E7A580
    void PhaseTwo();                                                     // 0x00E7A5C0
    void PhaseThree();                                                   // 0x00E7A9F0
    void PhaseFour();
    void PhaseFive();                                                    // 0x00E7A740
    void PlayCutscene(CCharString, bool, CCharString);                   // 0x00E78100
    void PreventConflictWithTCE();
    PATH_ID Path;                                                        // +0x48
    STAGE_IN_PATH StageNumber;                                           // +0x4c
    bool HeroFlashedLightsCorrectly;                                     // +0x50
    bool PrisonerTalkedAboutLamp;                                        // +0x51
    bool ResetLamp;                                                      // +0x52
    bool SaidYesToBecomeMayor;                                           // +0x53
    bool QuestCardGiven;                                                 // +0x54
    bool MayorExposeGhostCutscenePlayed;                                 // +0x55
    CScriptGameResourceObjectScriptedThingBase seh_Suspicious;           // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_LadyGrey;             // +0x68
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;              // +0x78
    CScriptGameResourceObjectScriptedThingBase seh_Rhodri;               // +0x88
    CCharString Evidence;                                                // +0x98
    float MaxFlashDistFromStables;                                       // +0x9c
    class CEM_SuspiciousVillager : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ExposeMayorScript&, CScriptDataBase*, const CScriptThing&); // 0x00E76360
        virtual CScriptBase* GetParentScript() const;                    // 0x00E76400
    protected:
        CV_ExposeMayorScript& ParentClass;                               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        bool DoneIntro;                                                  // +0x1c
        bool DoneOutro;                                                  // +0x1d
        virtual void Init();                                             // 0x00E76410
        virtual void OnPersist(CPersistContext&);                        // 0x00E77690
        virtual void Main();                                             // 0x00E776F0
    };
    class CEM_Sheriff;                                                   // Alloc 0x00E76470
    class CEM_Rhodri;                                                    // Alloc 0x00E765D0
    class CEM_FlashLightPoint;                                           // Alloc 0x00E76730
    class CHH_CellarDoor;                                                // Alloc 0x00E76800
};
#ifdef FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_SHERIFF
// Retail opt-in (chunk 808): nested CEM_Sheriff (Ego_r.pdb, 32 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18,
// IntroDone +0x1c, StageTwoSpoken +0x1d, StageFourSpoken +0x1e, CommentedOnHeroMarriedStatus +0x1f. Retail: the Alloc 0x00E76470 news
// 0x20 and stores vtable 0x012E98B4 (slot 0 ??_G 0x00E76520, slot 1 Main 0x00E78630, slot 2 Init 0x00E76540, slot 3 GetParentScript
// 0x00E76510, slot 4 OnPersist 0x00E776C0); Main 0x00E78630 reads / writes +0x1c..+0x1f, OnPersist 0x00E776C0 transfers "IntroDone" at +0x1c.
// Methods: the Ego_r.pdb / FableWin.pdb name set ?<Name>@CEM_Sheriff@CV_ExposeMayorScript@NScript@@...
class CV_ExposeMayorScript::CEM_Sheriff : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ExposeMayorScript&, CScriptDataBase*, const CScriptThing&); // 0x00E76470
    virtual CScriptBase* GetParentScript() const;                        // 0x00E76510
protected:
    CV_ExposeMayorScript& ParentClass;                                   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    bool IntroDone;                                                      // +0x1c
    bool StageTwoSpoken;                                                 // +0x1d
    bool StageFourSpoken;                                                // +0x1e
    bool CommentedOnHeroMarriedStatus;                                   // +0x1f
    virtual void Init();                                                 // 0x00E76540
    virtual void OnPersist(CPersistContext&);                            // 0x00E776C0
    virtual void Main();                                                 // 0x00E78630
};
#endif
#ifdef FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_RHODRI
// Retail opt-in (chunk 836): the nested entity script CEM_Rhodri (Ego_r.pdb, 0x20: ParentClass +0x14, PMasterData +0x18,
// ExplainedPlight +0x1c (bool), struct_layouts_egor.tsv; retail Main 0x00E799F0 reads [this+0x14] / [this+0x18] and writes
// [this+0x1c]; the landed Init 0x00E766A0 models the same offsets). Methods: Ego_r.pdb ?Main@CEM_Rhodri@...@EAEXXZ (private
// virtual, vtable slot 1, 0x00E799F0), ?Init@...@EAEXXZ (0x00E766A0), ?Alloc@...@SI... (0x00E765D0); GetParentScript 0x00E76670.
class CV_ExposeMayorScript::CEM_Rhodri : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ExposeMayorScript&, CScriptDataBase*, const CScriptThing&); // 0x00E765D0
    virtual CScriptBase* GetParentScript() const;                        // 0x00E76670
protected:
    CV_ExposeMayorScript& ParentClass;                                   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    bool ExplainedPlight;                                                // +0x1c
    virtual void Init();                                                 // 0x00E766A0
    virtual void Main();                                                 // 0x00E799F0
};
#endif
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ExposeMayorScript) == 0xA0);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager) == 0x20);
#ifdef FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_SHERIFF
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ExposeMayorScript::CEM_Sheriff) == 0x20);
#endif
#ifdef FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_RHODRI
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ExposeMayorScript::CEM_Rhodri) == 0x20);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_EXPOSEMAYORSCRIPT_H
