#ifndef FABLE_ENGINE_CV_BORDELLOSCRIPT_H
#define FABLE_ENGINE_CV_BORDELLOSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_map.h"
// Original PDB owner; native constructore398f0 proves handles74,84,94,a4 and mapb4, sizec0.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BordelloScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BordelloScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();virtual void Main();virtual void Init();virtual void OnPersist(CPersistContext&);
    bool HeroPartying,HeroTricking,MagicianSleeping,CutscenePlaying,PlayerOwned,BecomeNunnery,PlayerHasLeftRegion,HeroFoundDeeds,HeroFoundDeedsLocation;
    long ClientsAlive;
    bool ClientInUse[3];
    CScriptThing BordelloVillage;
    bool WearingWhoreWig,HadSex;
    long BeersDrunk;
    bool BeerSetToDPad;
    CScriptGameResourceObjectScriptedThingBase seh_Boss,seh_Guard,seh_Madam,seh_Whore;
    std::map<CCharString,CCharString> csargs;
    void WatchForBordelloStatus();void WatchForDeedStatus();void WatchForHeroLeavingRegionWithBeer();void CreateCustomers();void AdjustTavernPrices();
    bool IsHeroDressedAsLady();bool IsHeroWearingBeard();bool IsHeroWearingTash();bool HeroWorksInBrothel();
    CCharString GetHeroStatusTextTag();void PlayCutscene(CCharString,bool);
    class CMadame:public CEntityScriptBase {
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CMadame(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        bool DoneIntro,MentionedNunnery;
        virtual void Init();virtual void Main();
    };
    class CBordelloLady:public CEntityScriptBase {
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CBordelloLady(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        bool WalkingDownstairs,PartiedAlready,HaveTalked;
        long GoldRequired;
        bool NoLongerWorking;
        CCharString Name;
#ifdef FABLE_RETAIL_CV_BORDELLOSCRIPT_CBORDELLOLADY
        // Retail opt-in (chunk 810): the private helper CBordelloLady::Main 0x00E3EB10 calls (0x00E403D0; Ego_r.pdb + FableWin.pdb
        // ?GetLHTSTag@CBordelloLady@CV_BordelloScript@NScript@@AAE?AVCCharString@@ABV4@@Z).
        CCharString GetLHTSTag(const CCharString&);                         // 0x00E403D0
#endif
        virtual void Init();virtual void Main();
    };
    class CBordelloGuard:public CEntityScriptBase {
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CBordelloGuard(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();virtual void Main();
    };
    class CMagicman:public CEntityScriptBase {
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CMagicman(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        bool DoneToutIntro,MentionedPimpHat;
        virtual void Init();virtual void Main();
    };
    class CBordelloClient:public CEntityScriptBase {
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CBordelloClient(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        long BrainState,ClientID,SexChance,NextLine;
        virtual void Init();virtual void Main();
    };
    class CM_HeroDownstairs:public CEntityScriptBase {
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CM_HeroDownstairs(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();virtual void Main();
    };
    class CBordelloEntrance:public CEntityScriptBase {
    protected:
        CV_BordelloScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CBordelloEntrance(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BordelloScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BordelloScript)==0xc0);
#endif
