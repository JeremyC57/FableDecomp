#ifndef FABLE_ENGINE_CQ_SUMMONINGTHESHIPSCRIPT_H
#define FABLE_ENGINE_CQ_SUMMONINGTHESHIPSCRIPT_H
// Retail model (Codex pass 13): Ego_r TPI0x10e8f complete owner and0x10eea CSTS_BriarRose.
// Native factory/vtable012de1ac confirms nested size0x20; donor base0x5c is retail0x48.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SummoningTheShipScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    long SummonersAlive;
    long CurrentAttackWave;
    bool LighthouseStarted;
    bool SummonerAttacksStarted;
    bool MissionFailed;
    CTimer CommentaryTimer;
    long ConversationIndex;
    void PlayCutscene(CCharString);
    void DoMission();
    bool MakeBriarRoseComment(const CCharString&);
    class CM_ActivateLighthouse : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SummoningTheShipScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_SummoningTheShipScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CFireHeart : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SummoningTheShipScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_SummoningTheShipScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CFireHeartHolder : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SummoningTheShipScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_SummoningTheShipScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CSummonerMinion : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SummoningTheShipScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_SummoningTheShipScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long WaveID;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CSummonerAttacker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SummoningTheShipScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_SummoningTheShipScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long WaveID;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CSTS_BriarRose : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SummoningTheShipScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_SummoningTheShipScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
        void FollowHero(bool);
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SummoningTheShipScript)==0x5c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SummoningTheShipScript::CSTS_BriarRose)==0x20);
#endif
