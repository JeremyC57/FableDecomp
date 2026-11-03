#ifndef FABLE_ENGINE_CQ_RANSOMVICTIMSCRIPT_H
#define FABLE_ENGINE_CQ_RANSOMVICTIMSCRIPT_H
// Original Ego_r1224b/FableWin owner TPI; native00debc60 allocatesbc, ctor00de0320.
// Full original members and all ten children. Retail base is48; donor owner base is5c.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    void PlayCutscene(CCharString,CCharString);
    bool QuestStartScreened; // +48
    bool MissionSucceeded; // +49
    bool MissionFailed; // +4a
    bool RescuedVictim; // +4b
    bool RansommerDied; // +4c
    bool RansomHelperStarted; // +4d
    bool RansomHelperSpoken; // +4e
    bool ExtraAssistanceGiven; // +4f
    bool RansomHelpersAttacked; // +50
    bool ToldGuardToWaitOutside; // +51
    bool EnteredNewRegion; // +52
    bool ThanksCutsceneStarting; // +53
    long HitChief; // +54
    long HitEvilBrotherAndProtection; // +58
    bool SomeoneDealingWithBeingHit; // +5c
    long KidnappersKilled; // +60
    CCharString ActiveEnemyName; // +64
    CTimer NPCCommentTimer; // +68
    CScriptGameResourceObjectScriptedThingBase seh_Guard; // +6c
    CScriptGameResourceObjectScriptedThingBase seh_Father; // +7c
    CScriptGameResourceObjectScriptedThingBase seh_Victim; // +8c
    CScriptGameResourceObjectScriptedThingBase seh_Brother; // +9c
    CScriptGameResourceObjectScriptedThingBase seh_Ransommer; // +ac
    class CNPCComment : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool VictimsFatherGuardSaidIt; // +1c
        bool VictimSaidIt; // +1d
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CRansomRevenge : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CVictimsFatherGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long TimesHit; // +1c
        virtual void Init();
        virtual void Main();
    };
    class CRandomAttacker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CVictimsBrotherHelper : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CVictimsBrother : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        void DealWithHeroHittingMe();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CRansomHelper : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CRansommer : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CVictim : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool WaitingForThingsToDie; // +1c
        bool ToldHeroOfThingsAttackingHim; // +1d
        bool GotOutOfCave; // +1e
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CVictimsFather : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_RansomVictimScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool IntroDone; // +1c
        bool ReactedToBodyguardsDeath; // +1d
        virtual void OnPersist(CPersistContext&);
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_RansomVictimScript)==0xbc);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_RansomVictimScript::CVictimsBrotherHelper)==0x1c);
#endif
