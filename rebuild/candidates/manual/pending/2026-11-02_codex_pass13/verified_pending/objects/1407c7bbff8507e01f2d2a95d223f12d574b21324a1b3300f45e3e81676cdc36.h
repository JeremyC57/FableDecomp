#ifndef FABLE_ENGINE_CMARKTESTSCRIPTSSCRIPT_H
#define FABLE_ENGINE_CMARKTESTSCRIPTSSCRIPT_H
// Original Ego_r TPI: CMarkTestScriptsScript 0x107c7 (0x5c donor / 0x48 retail), CShopKeeper 0x107d7 (0x1c).
// Native Alloc 00efd9c0 requests 0x1c and stores CShopKeeper vtable 012f6e44; slot 1 is Main 00efdc40.
// Full inherited layouts and both original protected members are retained.
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CMarkTestScriptsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CMarkTestScriptsScript(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    // Original Ego_r107f5, full native1c, Alloc00efcb70/table012f6d54.
    class CTestMan : public CEntityScriptBase {
    protected:
        CMarkTestScriptsScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CTestMan(CMarkTestScriptsScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CMarkTestScriptsScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    };
    class CGift;
    class CShopKeeper : public CEntityScriptBase {
    protected:
        CMarkTestScriptsScript& ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData; // +0x18
    public:
        CShopKeeper(CMarkTestScriptsScript&, CScriptDataBase*, const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CMarkTestScriptsScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CMarkTestScriptsScript) == 0x48);
FABLE_STATIC_ASSERT(sizeof(NScript::CMarkTestScriptsScript::CShopKeeper) == 0x1c);
#endif
