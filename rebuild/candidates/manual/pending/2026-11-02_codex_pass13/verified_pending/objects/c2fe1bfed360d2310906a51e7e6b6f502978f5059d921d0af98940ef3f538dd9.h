#ifndef FABLE_ENGINE_CQ_BREAKSIEGESCRIPT_H
#define FABLE_ENGINE_CQ_BREAKSIEGESCRIPT_H
// PDB Ego_r TPI 0x1165e and retail ctor 00d1cc10: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>.
// Layout: native base 0x48; fifteen flags 0x48..0x56; ReasonForFailure +0x58; OnPersist keys prove state names.
// Native owner ctor 00d1cc10 and registered factory confirm this layout; donor containers are larger.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BreakSiegeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void WatchForOpenGates();
    void WatchForWinLoseConditions();
    void WatchForTermination();
    bool MissionSucceeded;
    bool MissionFailed;
    bool BanditDemandsKnown;
    bool ChiefRefusesDemands;
    bool BanditLeaderFree;
    bool BanditSiegeStart;
    bool HasFirstWaveAttacked;
    bool StartSecondWave;
    bool BanditsAttackHero;
    bool HeroLetInTown;
    bool GuardFollowingHero;
    bool EndCutsceneStarts;
    bool EndCutsceneDone;
    bool AttackedBanditLeader;
    bool QuestStartScreened;
    long ReasonForFailure;
    class CBanditAttacker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool HeroHitMe;
    };
    class CBanditFirstWave : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CBanditSiegeLeader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool CloseToHero;
    private:
        bool HeroAttacked;
    };
    class CDisplacedTrader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long TimesSpokenTo;
    private:
        CTimer RandomMutteringsDelay;
    };
    class CGuardHoldingBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CGuardToHelpDefend : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool GateGuard;
    private:
        bool FollowingHero;
    private:
        long TimesSpokenTo;
    };
    class CImprisonedBanditLeader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CKG_ChiefBreakSiege : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CKG_Sheriff : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BreakSiegeScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_BreakSiegeScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool ToldHeroToGatherGuards;
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BreakSiegeScript) == 0x5c);
#endif
