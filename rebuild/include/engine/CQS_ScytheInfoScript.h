#ifndef FABLE_ENGINE_CQS_SCYTHEINFOSCRIPT_H
#define FABLE_ENGINE_CQS_SCYTHEINFOSCRIPT_H
#ifdef FABLE_RETAIL_SCYTHE_INFO_PASS30
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_ScytheInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQS_ScytheInfoScript(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    bool MissionSucceeded;
    std::map<CCharString,CCharString> cs_args;
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cs_chars;
    class CScytheMarker;
    class CScytheNearOracle;
};
class CQS_ScytheInfoScript::CScytheMarker : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQS_ScytheInfoScript&, CScriptDataBase*, const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
protected:
    CQS_ScytheInfoScript& ParentClass;
    CQ_SunnyvaleMasterData* PMasterData;
private:
    virtual void Main();
};
class CQS_ScytheInfoScript::CScytheNearOracle : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQS_ScytheInfoScript&, CScriptDataBase*, const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
protected:
    CQS_ScytheInfoScript& ParentClass;
    CQ_SunnyvaleMasterData* PMasterData;
private:
    virtual void Init();
    virtual void Main();
};
}
// Layout: the original constructor places its map members after the byte at +48h.
// The two PDBs identify both nested entities' parent reference and master-data pointer.
FABLE_STATIC_ASSERT(sizeof(NScript::CQS_ScytheInfoScript)==0x64);
FABLE_STATIC_ASSERT(sizeof(NScript::CQS_ScytheInfoScript::CScytheMarker)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQS_ScytheInfoScript::CScytheNearOracle)==0x1c);
#endif
#endif
