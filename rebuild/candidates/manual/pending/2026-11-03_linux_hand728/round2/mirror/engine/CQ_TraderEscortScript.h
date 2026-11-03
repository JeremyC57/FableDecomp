#ifndef FABLE_ENGINE_CQ_TRADERESCORTSCRIPT_H
#define FABLE_ENGINE_CQ_TRADERESCORTSCRIPT_H
// Original Ego_r TPI1257d/1258d; retail base shrinks0x14, owner0x7c, CEndTrader0x20.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderEscortScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum ETraderComment { TRADER_COMMENT_SINGLE=0, TRADER_COMMENT_MULTIPLE=1, TRADER_COMMENT_ATTACKED_SPEECH=2 };
    enum EInfectedState { INFECTED_NONE=0, INFECTED_ONLY_JUST=1, INFECTED_LOW=2, INFECTED_MEDIUM=3, INFECTED_HIGH=4, INFECTED_MAX=5 };
    enum ETRADER_STATE { TRADER_STATE_NONE=0, TRADER_STATE_JUST_ATTACKED=1, TRADER_STATE_FOLLOWING=2, TRADER_STATE_SCARED=3, TRADER_STATE_MAX=4 };
    CTimer CommentTimer;
    long BalverinesToSurpriseHeroNeeded;
    long TradersStillAliveCounter;
    bool InfectedTraderCanGetUp;
    bool TradersShouldBeScared;
    bool IntroFinished;
    bool FollowInfoGiven;
    bool EndStarted;
    bool SavedInMiddle;
    bool SavedNearEnd;
    long IncubationTimeLow;
    long IncubationTimeMedium;
    long IncubationTimeHigh;
    long TraderPickpocketedCount;
    bool MissionSucceeded;
    bool MissionFailed;
    bool QuestStartScreened;
    bool GoneWrongWay;
    bool ShownBalverine;
    bool ShownEarthTroll;
    long TimesBantered;
    CCharString TraderToTalk;
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    virtual void OnPersist(CPersistContext&);
    void WatchForMissionRules();
    void WatchForSurprisingBalverines();
    bool MakeTraderComment(const CCharString&,const CScriptThing&,ETraderComment);
    void TurnToBalv(CScriptThing);
    void WatchForPickpocketing(CScriptThing);
    class CEndTrader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool EndingCanStart;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CDW5_Balv : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CDW5_Bandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CMagicBarrier : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CDarkwoodRockTroll : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long HealthBarID;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CRockTrollTrigger : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool RockTrollSpawned;
        virtual void Init();
        virtual void Main();
    };
    class CBanditCronies : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CTE_BanditLeader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CDarkwoodBalverineTrader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CDarkwoodAssassinSpawn : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CTraderComment : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CInfectedBalverine : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CFleshEatingBalverine : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CDarkwoodTrader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_TraderEscortScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long Infected;
        long BrainState;
        long CurrentAIState;
        long PreviousAIState;
        CTimer IncubationTime;
        long TraderHealthID;
        bool GreetedBuddy;
        bool LeadToCamp;
        bool InSafeZone;
        CTimer RegulateFollowStateComment;
        CTimer RegulateBanterComment;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_TraderEscortScript)==0x7c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_TraderEscortScript::CEndTrader)==0x20);
#endif
