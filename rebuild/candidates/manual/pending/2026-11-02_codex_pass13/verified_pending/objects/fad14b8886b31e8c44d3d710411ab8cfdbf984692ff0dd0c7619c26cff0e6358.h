#ifndef FABLE_ENGINE_CQ_OAKVALEBANDITRAIDSCRIPT_H
#define FABLE_ENGINE_CQ_OAKVALEBANDITRAIDSCRIPT_H
// Original Ego_r1064e/FableWin10808. Native factory00dc24e0 allocates78; vector is12 bytes.
// Original nested types1065e/1066d: ParentClass protected reference14, PMasterData pointer18, sizeof1c.
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OakValeBanditRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    bool HeroUnseenByBandits();
    void WatchForWhistling();
    void WatchForLevelChanges();
    void WatchForGateTimer();
    void DepopulateLevel();
    bool PopulateLevel();
    // Both original TPI records make the first reference non-const, unlike the older neighbor stand-in.
    long CanThingSeeAnythingInVector(CScriptThing&,std::vector<CScriptThing>&);
    bool BanditsAwareOfPlayer;
    bool BanditsAwareOfPlayerAgain;
    bool GateClosed;
    std::vector<CScriptThing> HerosFollowers;
    bool OpenGates;
    bool FirstSetup;
    long SoundPlayed;
    bool StopWhistling;
    bool BanditsKilled;
    bool IsBanditInRange;
    CScriptThing BanditInRange;
    long BanditsStopChasing;
    bool NoPersistence;
    class COakValeBanditRaidBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OakValeBanditRaidScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    protected:
        CQ_OakValeBanditRaidScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    };
    class CM_RestartMarker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OakValeBanditRaidScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    protected:
        CQ_OakValeBanditRaidScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OakValeBanditRaidScript)==0x78);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OakValeBanditRaidScript::COakValeBanditRaidBandit)==0x1c);
#endif
