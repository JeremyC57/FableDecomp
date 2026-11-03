#ifndef FABLE_ENGINE_CV_SICKCHILDSCRIPT_H
#define FABLE_ENGINE_CV_SICKCHILDSCRIPT_H
// PDB Ego_r TPI 0x109e9 and retail ctor 00ec53c0: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>.
// Layout: base 0x48; seh_Mother +0x48, seh_Witch +0x58; FinishedQuest +0x68, MansLoverState +0x6c.
// Native owner ctor 00ec53c0 and registered factory confirm this layout; donor containers are larger.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SickChildScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
// Retail opt-in (pass13): complete original Ego_r109e9 methods/enum; existing layout/default retained.
    typedef CV_SickChildScript TMyClass;
    typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
    CV_SickChildScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CCharString FABLE_FASTCALL StatGetClassName();
    static CScriptDataBase* (FABLE_FASTCALL *FABLE_FASTCALL StatGetMasterScriptDataAlloc())();
    void PlayCutscene(CCharString,CCharString);
    enum MANS_LOVER_STATE { ML_NOT_RECEIVED_LETTER=0, ML_IN_LOVE_WITH_YOU=1, ML_NOT_IN_LOVE_WITH_YOU=2, ML_KILLED=3 };
#endif
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    CScriptGameResourceObjectScriptedThingBase seh_Mother;
    CScriptGameResourceObjectScriptedThingBase seh_Witch;
    bool FinishedQuest;
    bool GotFishingSpotMushroom;
    bool MotherIntroDone;
    long MansLoverState;
    bool OwnerAlive;
    bool LaughingWomanKilled;
    class CIngredientOwner : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CIngredientOwner(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool BoughtSpecialStuff;
    private:
        bool StoleSpecialStuff;
    };
    class CManInLove : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CManInLove(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void OnPersist(CPersistContext&);
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool GivenLetter;
    private:
        bool GotReward;
    private:
        bool HelpedGuyOut;
    };
    class CMansLover : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CMansLover(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void OnPredicateFail();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool LeavingRegion;
    };
    class CSickChild : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CSickChild(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CSickChildFishingSpot : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CSickChildFishingSpot(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CSickChildsMother : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CSickChildsMother(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CSickChildsSister : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CSickChildsSister(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool LeadHeroToSickChild;
    };
    class CTalkingTrader1 : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CTalkingTrader1(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long TimesTalkedTo;
    private:
        long TimesHit;
    };
    class CTalkingTrader2 : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CTalkingTrader2(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long TimesTalkedTo;
    private:
        long TimesHit;
    };
    class CWitch : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CWitch(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool IntroDone;
    private:
        long ThingsForPotionGot;
    };
    class CWomanToAttract : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        typedef CV_SickChildScript TParentClass;
        CWomanToAttract(CV_SickChildScript&,CScriptDataBase*,const CScriptThing&);
        CQ_SunnyvaleMasterData* GetMasterData();
#endif
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SickChildScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
#ifdef FABLE_RETAIL_CV_SICKCHILDSCRIPT_COMPLETE
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void OnPersist(CPersistContext&);
        void DoLeavingCutscene();
#endif
        virtual void Main();
    protected:
        NScript::CV_SickChildScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool GivenObject;
    private:
        long TimesMadeLaugh;
    private:
        CScriptGameResourceObjectScriptedThingBase seh_me;
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_SickChildScript) == 0x74);
#endif
