#ifndef FABLE_ENGINE_CPERSONALSCRIPT_GLOBALTHINGSSCRIPT_H
#define FABLE_ENGINE_CPERSONALSCRIPT_GLOBALTHINGSSCRIPT_H
// PDB Ego_r TPI 0x12364 and retail ctor 00cde350: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>.
// Layout: CLevelScriptBase is 0x48 with Game +0x40 and MasterScriptData +0x44; owner has no fields.
// Native owner ctor 00cde350 and registered factory confirm this layout; donor containers are larger.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_GlobalThingsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();
    // Init and OnPersist are inherited from CScriptBase.
    class CArcheryCompetitionSignpost : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CBSGateGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CBowerstoneTeleportPointMarker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CClickableDoor : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool IsDoorOpen;
    };
    class CDragonGate : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CEndGameBarrier : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        CScriptThing ForceField1;
    private:
        CScriptThing ForceField2;
    };
    class CFifteenKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CFiftyKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CFiveKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CHERO_GUILD_TELEPORT_MARKER : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CHundredKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CInvisibleBarrier : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CKG_Gate : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CLadyGreysBed : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool GotKey;
    };
    class CMK_HS2_SOUL : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CMK_HS3_SOUL : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CMK_HSE_SOUL : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class COF_MainGates : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CSeventyFiveKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CTargetForKey : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool TargetForKeyDone;
    };
    class CTeleportPoint : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CTenKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CTestGateSwitch : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool GatesOpen;
    };
    class CThirtyKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CTwentyFiveKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CTwentyKeyChest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_GlobalThingsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CPersonalScript_GlobalThingsScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
#ifdef FABLE_RETAIL_CPERSONALSCRIPT_GLOBALTHINGSSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00CE19A0 Alloc): FableWin.pdb ??_7CPersonalScript_GlobalThingsScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: CPersonalScript_GlobalThingsScript, Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    CPersonalScript_GlobalThingsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0CPersonalScript_GlobalThingsScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00CE19A0
#endif
};
}
FABLE_STATIC_ASSERT_AT(319, sizeof(NScript::CPersonalScript_GlobalThingsScript) == 0x48);
#endif
