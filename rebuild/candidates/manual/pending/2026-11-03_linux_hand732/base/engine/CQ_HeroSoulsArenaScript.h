#ifndef FABLE_ENGINE_CQ_HEROSOULSARENASCRIPT_H
#define FABLE_ENGINE_CQ_HEROSOULSARENASCRIPT_H
// Original Ego_r TPI10f73/10fde; native guard factory00d7dff0 allocates1c, vtable012d3ca4.
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CHeroSoulsRoundDef;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    typedef CQ_HeroSoulsArenaScript TMyClass;
    typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
    CQ_HeroSoulsArenaScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CCharString FABLE_FASTCALL StatGetClassName();
    static CScriptDataBase* (FABLE_FASTCALL *FABLE_FASTCALL StatGetMasterScriptDataAlloc())();
    virtual void RegisterMain();
    enum ARENA_LEVEL_REACHED { ALR_NOWHERE=0, ALR_ARENA_EXTERIOR=1, ALR_ARENA_HOH=2, ALR_ARENA_CELLS=3 };
    long CreatureCount;
    bool PlayedArenaCS;
    bool PlayerHitSummoner1;
    bool PlayerHitSummoner2;
    bool PlayerAttacksGuard;
    bool JackTauntNearHOH;
    bool FirstWaveAttack;
    bool PlayedHoHCS;
    long ArenaLevelReached;
    CTimer GuardChatTimer;
    std::vector<CHeroSoulsRoundDef> Rounds;
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    void PlayCutscene(CCharString);
    CCharString GetJackSpeech(long);
    void WatchArenaCellsToTurnOffVillage();
    void WatchForAudioInArena();
    void WatchForRoundsInArena();
    class CNotForODG : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CNotForODG(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CSummonerToKill : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CSummonerToKill(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CArenaHeroGate : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CArenaHeroGate(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CHOHGuard : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CHOHGuard(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CSummoner1 : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CSummoner1(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CSummoner2 : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CSummoner2(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CHeroSoulsEnemy : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CHeroSoulsEnemy(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CBlockedEntrance : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CBlockedEntrance(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CDyingVillager : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CDyingVillager(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool PlayedOnce;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CHSA_ScreamingVillager : public CEntityScriptBase {
    public:
        typedef CQ_HeroSoulsArenaScript TParentClass;
        CHSA_ScreamingVillager(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_HeroSoulsArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript)==0x68);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CHSA_ScreamingVillager)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CDyingVillager)==0x20);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CBlockedEntrance)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CHeroSoulsEnemy)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CSummoner2)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CSummoner1)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CHOHGuard)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CArenaHeroGate)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CSummonerToKill)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HeroSoulsArenaScript::CNotForODG)==0x1c);
#endif
