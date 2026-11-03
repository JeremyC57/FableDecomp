#ifndef FABLE_ENGINE_CQ_AWAKENINGTHEORACLESCRIPT_H
#define FABLE_ENGINE_CQ_AWAKENINGTHEORACLESCRIPT_H
// Retail model (Codex pass 13): original Ego_r TPI0x12a91 complete owner and0x12b19 CNecropolisGhost.
// Native fields54/5c/74 and nested factory allocation20 confirm the retail layout.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    void WatchForAliveTablets();
    void PlayCutscene(CCharString,bool);
    bool MissionSucceeded;
    bool PlayedOpeningCS;
    bool PlayedLostBayCS;
    bool PlayedNecroCS;
    long TabletsCollected;
    bool SpawnedFinalSpotsStuff;
    long PositionTaken;
    bool GivenDiggingSpotsRewards;
    long GraveNumbersGivenOut;
    long FinalSummonersAlive;
    bool DisplayedBadTabletMessage;
    bool PlayedOpeningNecroCS;
    long TotalSpawnedCreaturesAlive;
    bool AutosavedInNW3;
    bool GotY;
    bool GotM;
    bool GotC;
    bool GotA;
    std::vector<CScriptThing,std::allocator<CScriptThing> > DiggingPoints;
    std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CCharString> > > cs_args;
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CScriptGameResourceObjectScriptedThingBase> > > cs_chars;
    class CFinalSummoner : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CATO_MagicBarrier : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CScytheBeforeGettingTablets : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CPretendGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CNecropolisEnemy : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CChunkCollectionIndicator : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BarID;
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CLostBayCutsceneTrigger : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CNecropolisGate : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CNecropolisGhost : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long MyGraveNumber;
        virtual void Init();
        virtual void OnPersist();
        virtual void Main();
    };
    class CFinalTabletSarcophagus : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class COracleTablet : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_AwakeningTheOracleScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool AlreadyDugHere;
        virtual void Init();
        virtual void OnPersist();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AwakeningTheOracleScript)==0x98);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost)==0x20);
#endif
