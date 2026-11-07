#ifndef FABLE_ENGINE_CV_DEMONDOORSSCRIPT_H
#define FABLE_ENGINE_CV_DEMONDOORSSCRIPT_H
// PDB Ego_r TPI 0x10567 and retail ctor 00e6e2b0: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>.
// Layout: CLevelScriptBase is 0x48 with Game +0x40 and MasterScriptData +0x44; owner has no fields.
// Native owner ctor 00e6e2b0 and registered factory confirm this layout; donor containers are larger.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_DemonDoorsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();
    // Original Ego_r TPI10567 and native call00e712e8 ->00e6f040.
    void FadeOutFaceAndOpenDoor(const CScriptThing&, const CCharString&);
    // Init and OnPersist are inherited from CScriptBase.
    class CDF_BeEvil : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool EvilEnough;
    };
    class CDF_CombatMult : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_DoDamage : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_Dragon : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_DressAsBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_EatPies : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_GiveGift : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_KeyHoarder : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_MarryLadyGrey : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_SexMad : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_ShineLight : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_SummonsCreatures : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_ThreeSuits : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
#ifdef FABLE_RETAIL_CDF_THREESUITS
        // Retail opt-in (chunk 834): the three private outfit tests Main 0x00E72630 calls on `this` (ecx = esi):
        //   Ego_r.pdb / FableWin.pdb ?DressedAsBandit@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@AAE_NXZ (0x00E72F00),
        //   ?DressedAsGoodKnight@...@@AAE_NXZ (0x00E730A0), ?DressedAsEvilMage@...@@AAE_NXZ (0x00E731F0); non-virtual, no layout change.
        bool DressedAsBandit();
        bool DressedAsGoodKnight();
        bool DressedAsEvilMage();
#endif
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CDF_ThunderHelmet : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_DemonDoorsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_DemonDoorsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_DemonDoorsScript) == 0x48);
#endif
