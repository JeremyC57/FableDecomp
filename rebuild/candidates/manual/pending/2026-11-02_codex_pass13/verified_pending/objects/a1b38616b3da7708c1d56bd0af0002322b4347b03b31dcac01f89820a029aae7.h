#ifndef FABLE_ENGINE_CV_BORDELLOSCRIPT_H
#define FABLE_ENGINE_CV_BORDELLOSCRIPT_H
// Original Ego_r10140/full child records; retail base48 versus Ego5c and map12 versus16.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_map.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BordelloScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool HeroPartying; // +48
    bool HeroTricking; // +49
    bool MagicianSleeping; // +4a
    bool CutscenePlaying; // +4b
    bool PlayerOwned; // +4c
    bool BecomeNunnery; // +4d
    bool PlayerHasLeftRegion; // +4e
    bool HeroFoundDeeds; // +4f
    bool HeroFoundDeedsLocation; // +50
    long ClientsAlive; // +54
    bool ClientInUse[3]; // +58
    CScriptThing BordelloVillage; // +5c
    bool WearingWhoreWig; // +68
    bool HadSex; // +69
    long BeersDrunk; // +6c
    bool BeerSetToDPad; // +70
    CScriptGameResourceObjectScriptedThingBase seh_Boss; // +74
    CScriptGameResourceObjectScriptedThingBase seh_Guard; // +84
    CScriptGameResourceObjectScriptedThingBase seh_Madam; // +94
    CScriptGameResourceObjectScriptedThingBase seh_Whore; // +a4
    std::map<CCharString,CCharString> csargs; // +b4
    virtual void RegisterMain();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    void WatchForBordelloStatus();
    void WatchForDeedStatus();
    void WatchForHeroLeavingRegionWithBeer();
    void CreateCustomers();
    void AdjustTavernPrices();
    bool IsHeroDressedAsLady();
    bool IsHeroWearingBeard();
    bool IsHeroWearingTash();
    bool HeroWorksInBrothel();
    CCharString GetHeroStatusTextTag();
    void PlayCutscene(CCharString,bool);
    class CBordelloEntrance : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CM_HeroDownstairs : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class CBordelloClient : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState; // +1c
        long ClientID; // +20
        long SexChance; // +24
        long NextLine; // +28
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CMagicman : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool DoneToutIntro; // +1c
        bool MentionedPimpHat; // +1d
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CBordelloGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CBordelloLady : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool WalkingDownstairs; // +1c
        bool PartiedAlready; // +1d
        bool HaveTalked; // +1e
        long GoldRequired; // +20
        bool NoLongerWorking; // +24
        CCharString Name; // +28
        CCharString GetLHTSTag(const CCharString&);
        virtual void OnPersist(CPersistContext&);
        virtual void Init();
        virtual void Main();
    };
    class CMadame : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool DoneIntro; // +1c
        bool MentionedNunnery; // +1d
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BordelloScript)==0xc0);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BordelloScript::CBordelloGuard)==0x1c);
#endif
