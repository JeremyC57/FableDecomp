#ifndef FABLE_ENGINE_CV_AMBUSHSCAMSCRIPT_H
#define FABLE_ENGINE_CV_AMBUSHSCAMSCRIPT_H
// NScript::CV_AmbushScamScript (village script V_AmbushScam, save tag "S_VAS"), chunk 730.
// Layout: Ego_r.pdb CV_AmbushScamScript (0xA8) minus 0x14 (retail CScriptBase 0x40 vs Ego_r 0x54): CLevelScriptBase<T> Game +0x40 /
// MasterScriptData +0x44, QuestFinished +0x48 .. LeaderDead +0x70, HitCount +0x74, BuddyCounter +0x78, RandomSpeechTimer +0x7C,
// RandomSpeech[5] +0x80; sizeof 0x94 = the Alloc's `push 94h` (0x00E322C0). Every offset is read by a landed row (OnPersist
// 0x00E2AD50, ~CV_AmbushScamScript 0x00E32310, WatchForHeroAttack 0x00E2E520). Vtable 0x012E2428 (no RTTI): slot 1 RegisterMain
// 0x00E2AC90, slot 2 Main 0x00E2B090, slot 3 Init 0x00E2ABC0, slot 4 OnPersist 0x00E2AD50.
// Members and nested entity scripts: Ego_r.pdb / FableWin.pdb name sets (?Main@CV_AmbushScamScript@NScript@@UAEXXZ,
// ?GetTrickster@CV_AmbushScamScript@NScript@@QAE?AVCScriptThing@@XZ, ?SpringAmbush@CV_AmbushScamScript@NScript@@QAEXXZ,
// ?Alloc@CAS_Ambusher@CV_AmbushScamScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// = 0x00E2BB60, CAS_Trickster::Alloc 0x00E2BC90, CAS_Buddy::Alloc 0x00E2BEC0). T = NScript::CQ_SunnyvaleMasterData as the landed
// village-script rows (the CAS_* PMasterData members are typed CQ_SunnyvaleMasterData* in Ego_r.pdb).
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AmbushScamScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_AmbushScamScript(CScriptDataBase*, CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void WatchForQuestFinished();
    void WatchForRegionTransition();
    void TricksterApproach();
    void SpringAmbush();
    void WatchForAllAmbushersDead();
    void WatchForHeroAttack(CScriptThing);
    void WatchForExpressions(CScriptThing);
    void WatchForTricksterComplete(CScriptThing);
    void WatchForTricksterKilledAfterBeingSpared(CScriptThing);
    CScriptThing GetTrickster();
    void AssignMoralityEtcForGoodEnding();
    void AssignMoralityEtcForEvilEnding();
    bool GetRandomSpeechEnabled();
    void ResetRandomSpeechTime();
    bool        QuestFinished;                      // +0x48
    bool        QuestComplete;                      // +0x49
    bool        QuestActivated;                     // +0x4a
    bool        LeftRegion;                         // +0x4b
    bool        MoralitySwitch;                     // +0x4c
    bool        BuddyIntroComplete;                 // +0x4d
    bool        DoTricksterIntro;                   // +0x4e
    bool        DoAmbush;                           // +0x4f
    bool        TricksterApproachComplete;          // +0x50
    bool        TricksterSpeechComplete;            // +0x51
    bool        TricksterRetreatComplete;           // +0x52
    bool        TricksterQuestComplete;             // +0x53
    bool        TricksterWhiningComplete;           // +0x54
    bool        TricksterDeadOrLeft;                // +0x55
    bool        TricksterComplete;                  // +0x56
    bool        TricksterKilledBeforeAmbush;        // +0x57
    bool        TricksterKilledAfterAmbush;         // +0x58
    bool        TricksterKilledAfterBeingSpared;    // +0x59
    bool        TricksterResetAllegience;           // +0x5a
    bool        TricksterWaitHere;                  // +0x5b
    float       TricksterApproachProximity;         // +0x5c
    bool        AmbushSpringComplete;               // +0x60
    bool        AllAmbushersDead;                   // +0x61
    bool        ExpressionDetectedFollow;           // +0x62
    bool        ExpressionDetectedWait;             // +0x63
    bool        HeroAttackOnBuddy;                  // +0x64
    bool        HeroAttackOnTricksterBeforeAmbush;  // +0x65
    bool        HeroAttackOnTricksterAfterAmbush;   // +0x66
    bool        HeroAttackOnTricksterKillingBlow;   // +0x67
    bool        HeroTalkToTrickster;                // +0x68
    bool        HeroAttachOnTricksterWarning;       // +0x69
    bool        BuddyHelped;                        // +0x6a
    bool        SpeakTime1;                         // +0x6b
    bool        SpeakTime2;                         // +0x6c
    bool        AllDone;                            // +0x6d
    bool        BanditSpoken;                       // +0x6e
    bool        AllBanditsDead;                     // +0x6f
    bool        LeaderDead;                         // +0x70
    long        HitCount;                           // +0x74
    long        BuddyCounter;                       // +0x78
    CTimer      RandomSpeechTimer;                  // +0x7c
    CCharString RandomSpeech[5];                    // +0x80
    class CAS_Ambusher : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_AmbushScamScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
    protected:
        NScript::CV_AmbushScamScript & ParentClass;          // +0x14
        NScript::CQ_SunnyvaleMasterData * PMasterData;       // +0x18
    };
    class CAS_Trickster : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_AmbushScamScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
        CCharString GetRandomSpeech();
    protected:
        NScript::CV_AmbushScamScript & ParentClass;          // +0x14
        NScript::CQ_SunnyvaleMasterData * PMasterData;       // +0x18
    private:
        long lastRandomSpeechIdx1;                           // +0x1c
        long lastRandomSpeechIdx2;                           // +0x20
    };
    class CAS_Buddy : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_AmbushScamScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
    protected:
        NScript::CV_AmbushScamScript & ParentClass;          // +0x14
        NScript::CQ_SunnyvaleMasterData * PMasterData;       // +0x18
    private:
        bool BuddyHeroApproachComplete;                      // +0x1c
        bool BuddyHeroApproachSpeechComplete;                // +0x1d
        bool BuddyFollowSpeechComplete;                      // +0x1e
#ifdef FABLE_RETAIL_CV_AMBUSHSCAMSCRIPT_BUDDY
        // Retail opt-in (Codex pass21): original BuddyStartPos at +0x1f; native E30A use.
        // Uses the existing packed C3DVector model to retain its genuine 1-byte alignment.
        C3DVector BuddyStartPos;
#endif
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_AmbushScamScript) == 0x94);
#ifdef FABLE_RETAIL_CV_AMBUSHSCAMSCRIPT_BUDDY
FABLE_STATIC_ASSERT(sizeof(NScript::CV_AmbushScamScript::CAS_Buddy) == 0x2c);
#endif
#endif
