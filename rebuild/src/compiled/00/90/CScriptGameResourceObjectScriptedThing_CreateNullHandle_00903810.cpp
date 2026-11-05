// CScriptGameResourceObjectScriptedThing::CreateNullHandle(CGameScriptInterface&) -- builds a scripted-thing handle that owns a new, unbound scripted-thing implementation
// Leaf: ?CreateNullHandle@CScriptGameResourceObjectScriptedThing@@SI?AVCScriptGameResourceObjectScriptedThingBase@@AAVCGameScriptInterface@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail 0x00903810, 0xB2 bytes, static __fastcall (ecx = hidden result, edx = scriptInterface), plain `ret`.
//   Manifest label "CGameScriptInterface_CreateScriptedEntity" (_global); PDB (FableWin) name set has
//   `?CreateNullHandle@CScriptGameResourceObjectScriptedThing@@SI?AVCScriptGameResourceObjectScriptedThingBase@@AAVCGameScriptInterface@@@Z`.
//   Only caller: CGameScriptInterface::StartScriptingEntity 0x0089B5B0 (call at 0x0089B71B).
// What it does: default-constructs a CScriptGameResourceObjectScriptedThingBase handle (vtable 0x0127094C, empty PImp),
//   gives it a `new CScriptGameResourceObjectScriptedThing(scriptInterface)` (0x28 bytes), returns a copy (PImp AddRef)
//   and destroys the local (PImp release, CScriptGameResourceObjectBase vtable 0x0126008C, CBaseObject dtor).
// Callees: CBaseObject::CBaseObject() 0x0099A380, CBaseObject::CBaseObject(const CBaseObject&) 0x0099A390,
//   CBaseObject::~CBaseObject() 0x0099A430, operator new 0x00BFEA1A, operator delete 0x00BFE9BC,
//   CScriptGameResourceObjectScriptedThing(CGameScriptInterface&) 0x00903710, and the handle's impl setter 0x008AB960
//   (`add ecx,8; jmp 0x008AB230` = PImp.Reset(T*), a folded out-of-line member; its real name is not in the PDB name sets).
// Shape: no NRVO in VC7.1, so local + copy ctor + local dtor. CCountedPointer has a user copy ctor (null-tested AddRef)
//   and an inline dtor whose release is null-tested, then zeroes Data and Info; the intermediate
//   CScriptGameResourceObjectBase vptr store of the constructors is dead and dropped, the one in its user dtor stays.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject {
public:
    CScriptGameResourceObjectBase() {}
    virtual ~CScriptGameResourceObjectBase() {}
};
class CGameScriptInterface;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    void SetImplementation(CScriptGameResourceObjectScriptedThingBase* imp);   // 0x008AB960
};
class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase {
public:
    CGameScriptInterface* ScriptInterface;               // +0x10
    void* PThing[2];                                     // +0x14
    long ScriptID;                                       // +0x1c
    bool NullResource;                                   // +0x20
    long ActionPriority;                                 // +0x24
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface);   // 0x00903710
    static CScriptGameResourceObjectScriptedThingBase __fastcall CreateNullHandle(CGameScriptInterface& scriptInterface);
};
CScriptGameResourceObjectScriptedThingBase __fastcall
CScriptGameResourceObjectScriptedThing::CreateNullHandle(CGameScriptInterface& scriptInterface)
{
    CScriptGameResourceObjectScriptedThingBase handle;
    handle.SetImplementation(new CScriptGameResourceObjectScriptedThing(scriptInterface));
    return handle;
}