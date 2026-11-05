#ifndef FABLE_ENGINE_CV_MURDERTWISTFINALSCRIPT_H
#define FABLE_ENGINE_CV_MURDERTWISTFINALSCRIPT_H
// Retail model (chunk 746, 2026-11-04): NScript::CV_MurderTwistFinalScript (Ego_r.pdb TPI CV_MurderTwistFinalScript, 0xA4)
// with its four entity classes. Retail sizes: factory 0x00EB6A60 `push 88h` (level script 0x88); entity Allocs 0x00EAED10 /
// 0x00EB1F80 / 0x00EB3C60 / 0x00EB5450.
// Layout (retail code; Ego_r offsets run 0x14 high, 0x18 high after the 12-byte STLport Guards vector): SheriffState +0x48,
// AssassinState +0x4C, TaskDeclined +0x50, PreemptiveAttackInProgress +0x51, Sheriff +0x54, Guards +0x60, seh_Sheriff +0x6C,
// seh_Guards +0x7C (ctor 0x00EAE910, OnPersist 0x00EB6520). Entity members equal Ego_r (CEntityScriptBase 0x14): ParentClass
// +0x14, PMasterData +0x18, MyEntityHandle +0x1C, then the per-class members (0x00EB2900 NextLine +0x2C / Name +0x34,
// 0x00EAF810 AttractTimer +0x2C / NextLine +0x30 / flags +0x34..+0x37).
// Methods: Ego_r.pdb / FableWin.pdb name sets (class key V, nested in CV_MurderTwistFinalScript, namespace NScript); the
// entity Do* members are private (AAE), Init / Main / OnPredicateFail / OnPersist private virtual (EAE).
// Enumerators of ESheriffState / EAssassinState are not in either PDB dump: descriptive names.
// Contract: the includer defines FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CLEVELSCRIPTBASE, FABLE_RETAIL_BASE_OBJECT and
// FABLE_RETAIL_CCHARSTRING first.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CTimer.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum ESheriffState { SHERIFF_STATE_0=0, SHERIFF_STATE_1=1, SHERIFF_STATE_2=2, SHERIFF_STATE_3=3 };
    enum EAssassinState { ASSASSIN_STATE_0=0, ASSASSIN_STATE_1=1, ASSASSIN_STATE_2=2, ASSASSIN_STATE_3=3 };
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    virtual void OnPersist(CPersistContext&);
    bool DoesAssassinTrustHero();                                        // 0x00EB03A0
    bool DoesSheriffTrustHero();
    bool HasHeroEnflamedEntity(const CScriptThing&);                     // 0x00EB07D0
    void WaitForEnflameReactionIfNecessary(const CScriptThing&);         // 0x00EB0700
    void PlayBeingRumbledCutscene();                                     // 0x00EAF1E0
    ESheriffState  SheriffState;                                         // +0x48
    EAssassinState AssassinState;                                        // +0x4C
    bool TaskDeclined;                                                   // +0x50
    bool PreemptiveAttackInProgress;                                     // +0x51
    CScriptThing Sheriff;                                                // +0x54
    std::vector<CScriptThing> Guards;                                    // +0x60
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;              // +0x6C
    std::vector<CScriptGameResourceObjectScriptedThingBase> seh_Guards;  // +0x7C
    class CMurderTwistSheriff : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistFinalScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        CV_MurderTwistFinalScript* ParentClass;                          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        CScriptGameResourceObjectScriptedThingBase MyEntityHandle;       // +0x1C
        CTimer AttractTimer;                                             // +0x2C
        unsigned long NextLine;                                          // +0x30
        bool HasGivenReminderToHero;                                     // +0x34
        bool HasBeingRumbledCutscenePlayed;                              // +0x35
        bool HasExpressedSuspicionToHero;                                // +0x36
        bool HasSaidWellDoneToHero;                                      // +0x37
    private:
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
        virtual void Main();
        void DoAskingHeroToKillAssassin();                               // 0x00EAF810
        void DoRemindingHeroToKillAssassin();                            // 0x00EB0830
        void DoBecomingSuspiciousOfHero();                               // 0x00EB10C0
        void DoGivingRewardToHero();                                     // 0x00EB15C0
        void DoSayingWellDoneToHero();                                   // 0x00EB19C0
        void AttackHero();                                               // 0x00EB1EC0
    };
    class CMurderTwistSheriffGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistFinalScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        CV_MurderTwistFinalScript* ParentClass;                          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        CScriptGameResourceObjectScriptedThingBase MyEntityHandle;       // +0x1C
        unsigned long NextLine;                                          // +0x2C
        unsigned long ID;                                                // +0x30
        CCharString Name;                                                // +0x34
    private:
        virtual void Init();                                             // 0x00EB2070
        virtual void Main();
        void DoAskingHeroToSpeakToBoss();                                // 0x00EB23E0
        void DoRemindingHeroToKillAssassin();                            // 0x00EB2900
        void DoAskingHeroToSpeakToBossAgain();                           // 0x00EB2DC0
        void DoCongratulatingHero();                                     // 0x00EB3280
        void DoSayingThankYouToHero();                                   // 0x00EB3740
        void AttackHero();
    };
#ifdef FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
    class CMurderTwistAssassin;
#endif
};
#ifdef FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
// Retail opt-in (chunk 782): the nested assassin entity (Ego_r.pdb TPI CMurderTwistAssassin, 0x3C; retail Alloc 0x00EB3C60).
// Members equal Ego_r (CEntityScriptBase 0x14), checked against retail code: ParentClass +0x14 (0x00EB4B0A `mov ecx,[esi+14h]`),
// MyEntityHandle +0x1C (0x00EB47C9 `lea edi,[esi+1Ch]`), NextLine +0x2C (0x00EB4F80 DoRemindingHeroToKillSheriff). Methods:
// Ego_r.pdb / FableWin.pdb name sets (Do* / AttackHero / IsFeelingThreatened private AAE, Init / Main / OnPredicateFail /
// OnPersist private virtual EAE).
class CV_MurderTwistFinalScript::CMurderTwistAssassin : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistFinalScript&,CScriptDataBase*,const CScriptThing&);   // 0x00EB3C60
    virtual CScriptBase* GetParentScript() const;                    // 0x00EB3D00
protected:
    CV_MurderTwistFinalScript* ParentClass;                          // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;       // +0x1C
    unsigned long NextLine;                                          // +0x2C
    bool HasBeenThreatened;                                          // +0x30
    float ThreatDistSq;                                              // +0x34
    bool HasGivenIntroToHero;                                        // +0x38
    bool PreemptiveAttackInProgress;                                 // +0x39
private:
    virtual void Init();                                             // 0x00EB3E40
    virtual void OnPersist(CPersistContext&);
    virtual void OnPredicateFail();                                  // 0x00EB3D10
    virtual void Main();                                             // 0x00EB3F40
    void DoWatchingForTrouble();                                     // 0x00EB40C0
    bool IsFeelingThreatened();                                      // 0x00EB4710
    void DoDealWithHero();                                           // 0x00EB4760
    void DoRemindingHeroToKillSheriff();                             // 0x00EB4F80
    void AttackHero();
};
#endif
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MurderTwistFinalScript)==0x88);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff)==0x38);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard)==0x38);
#ifdef FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin)==0x3C);
#endif
#endif
