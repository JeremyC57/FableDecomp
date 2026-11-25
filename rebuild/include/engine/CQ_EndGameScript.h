#ifndef FABLE_ENGINE_CQ_ENDGAMESCRIPT_H
#define FABLE_ENGINE_CQ_ENDGAMESCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
// Original PDB owner and nested classes; native owner58, UndeadBar48, GuiCounter4c, flags50..54.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_EndGameScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    float UndeadBar;long GuiCounter;
    bool SeenAbbey,QuestStartScreened,SeenMotherDisappear,MotherSpoken,ShowUndeadBar;
    void CheckRegionLeaving();
    void CheckGuildEntering();
    void DoTheCoolEffects();
    class CM_UndeadSpawn:public CEntityScriptBase {
    protected:
        CQ_EndGameScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CM_UndeadSpawn(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    };
    class CGatewayUndead:public CEntityScriptBase {
    protected:
        CQ_EndGameScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CGatewayUndead(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        float RaiseUndeadBarAmount;
        virtual void Init();
        virtual void Main();
    };
    class CHookCoastTeleporter:public CEntityScriptBase {
    protected:
        CQ_EndGameScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CHookCoastTeleporter(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    };
    class CMK_GHC_HERO3:public CEntityScriptBase {
    protected:
        CQ_EndGameScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CMK_GHC_HERO3(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    };
    class CMazeJournal:public CEntityScriptBase {
    protected:
        CQ_EndGameScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CMazeJournal(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    };
#ifdef FABLE_RETAIL_CQ_ENDGAMESCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00D2A7C0 Alloc): FableWin.pdb ??_7CQ_EndGameScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00D2A7C0
#endif
};
}
FABLE_STATIC_ASSERT_AT(88, sizeof(NScript::CQ_EndGameScript)==0x58);
#endif
