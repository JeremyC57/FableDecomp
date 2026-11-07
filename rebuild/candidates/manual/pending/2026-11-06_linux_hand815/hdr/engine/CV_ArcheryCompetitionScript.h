#ifndef FABLE_ENGINE_CV_ARCHERYCOMPETITIONSCRIPT_H
#define FABLE_ENGINE_CV_ARCHERYCOMPETITIONSCRIPT_H
// Original Ego_r TPI0x13011, native child vtable012e3178 and fields in00e36d50.
// Layout: original complete owner data, base0x48; child offsets match donor exactly.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#ifdef FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CACOWNER_MAIN
class CScriptGameResourceObjectScriptedThingBase;
#endif
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ArcheryCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void CalculateScoreForHit(long,float);
    void UpdateHighScore(long);
    void UpdateGameScore();
    void AddArcheryQuestInfo();
    void RemoveArcheryQuestInfo();
    void WatchForQuestFinished();
    void WatchForOwnerKilled();
    void WatchForSignpostConditions();
#ifdef FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CACOWNER_MAIN
    // Retail opt-in (chunk 815, CAC_Owner::Main 0x00E33EA0): the parent helpers it calls (Ego_r.pdb / FableWin.pdb
    // ?<Name>@CV_ArcheryCompetitionScript@NScript@@QAE..., the landed rows at these addresses).
    void ResetRandomSpeechTime();                                        // 0x00E337C0
    void WatchForAttack(CScriptThing);                                   // 0x00E36390
    void DoTimesUp(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&); // 0x00E36590
    void DoCountDown(long);                                              // 0x00E36750
#endif

    long Score; // +0x48
    bool IsOwnerDead; // +0x4c
    bool PrizeAlreadyGot; // +0x4d
    bool QuestActivated; // +0x4e
    bool AttackedByHero; // +0x4f
    bool GiveAttackWarning; // +0x50
    bool QuestFinished; // +0x51
    bool WelcomeComplete; // +0x52
    bool Pickpocketed; // +0x53
    long DummyCounter; // +0x54
    long ArcheryHighScoreInfoID; // +0x58
    long ArcheryGameScoreInfoID; // +0x5c
    long ArcheryGameTimerID; // +0x60
    bool SteppedOut; // +0x64
    bool UsedMagic; // +0x65
    bool ShotOwner; // +0x66
    bool GivenReallyCoolPrize; // +0x67
    bool GivenFirstHighScorePrize; // +0x68
    bool AddedReallyCoolPrizeToOwner; // +0x69
    bool AddedFirstHighScorePrizeToOwner; // +0x6a
    CTimer GameTimeLeft; // +0x6c
    CTimer RandomSpeechTimer; // +0x70
    CCharString RandomSpeech[10]; // +0x74
    enum { POWER_LEVEL_LOW=0, POWER_LEVEL_MEDIUM=1, POWER_LEVEL_HIGH=2, POWER_LEVEL_MAX_LEVELS=3 } ScorePowerLevel; // +0x9c
    enum { CONTEST_WAITING=0, CONTEST_COUNTDOWN=1, CONTEST_FINISHED=2, CONTEST_STARTED=3 } ContestState; // +0xa0
    class CAC_DummyMoving : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ArcheryCompetitionScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CV_ArcheryCompetitionScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        long PointsValue; // +0x1c
    private:
        long Steps; // +0x20
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CAC_DummyStatic : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ArcheryCompetitionScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CV_ArcheryCompetitionScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        long PointsValue; // +0x1c
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CM_AC_DummyMoving : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ArcheryCompetitionScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CV_ArcheryCompetitionScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CM_AC_DummyStatic : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ArcheryCompetitionScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CV_ArcheryCompetitionScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        virtual void Init();
    private:
        virtual void Main();
    };
    class CAC_Owner : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ArcheryCompetitionScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CV_ArcheryCompetitionScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        long lastRandomSpeechIdx1; // +0x1c
    private:
        long lastRandomSpeechIdx2; // +0x20
    private:
        virtual void Init();
    private:
        virtual void OnPredicateFail();
    private:
        virtual void Main();
#ifdef FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CACOWNER_MAIN
    private:
        CCharString GetRandomSpeech();                                   // 0x00E362B0 (Ego_r / FableWin ?GetRandomSpeech@CAC_Owner@...@@AAE?AVCCharString@@XZ)
#endif
    };
    class CAC_HighScorePost : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ArcheryCompetitionScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    protected:
        NScript::CV_ArcheryCompetitionScript & ParentClass; // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData; // +0x18
    private:
        CScriptThing HighScorePost; // +0x1c
    private:
        virtual void Init();
    private:
        virtual void OnPredicateFail();
    private:
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ArcheryCompetitionScript) == 0xa4);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ArcheryCompetitionScript::CAC_DummyMoving) == 0x24);
#endif
