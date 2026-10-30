// CGameScriptInterface::CreateObject(const CCharString& defName, const C3DVector& position, float angle, const CCharString& scriptName) const -- create a thing object from the named def at the position, turned by the angle (an Euler-built orientation), player 4, and return its script thing (the null script thing when creation failed)
// Leaf: ?CreateObject@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@M0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00894950, length 324 bytes (+ int3 padding), `ret 14h` (hidden result + 4).
// The float overload of the landed CreateObject 0x00894820 (same TU): identical apart from the orientation, a
//   CRightHandedSet built by its Euler-angle constructor @ 0x00A229E0 from C3DVector(angle, 0, 0) (both frame locals).
// Callees: CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CDefinitionManager::GetDef<CThingObjectDef>
//   @ 0x0042B040, CRightHandedSet::CRightHandedSet(const C3DVector&) @ 0x00A229E0, CThingObject::Create(long, const
//   C3DVector&, long, const CRightHandedSet*, unsigned long, const CCharString&) @ 0x00703210 (static __fastcall),
//   CThing::GetScriptThing() @ 0x004C7E50 (by value), CScriptThing::operator= @ 0x008AB980, CBaseClass ctor/dtor
//   @ 0x0099A2D0 / 0x0099A2E0, operator delete @ 0x00BFE9BC, the def's slot 1. CScriptThing vtable 0x01238C8C.
// Layout / shape: as the landed 0x00894820 (CDefPointer homed in the dead defName slot, named result local, no NRVO).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class CCharString { public:
    void* PStringData;
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRightHandedSet { public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    CRightHandedSet(const C3DVector& eulerAngles);      // 0x00A229E0
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CThingObjectDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CDefPointerThingObjectDef {
    CThingObjectDef* Object;
    CDefPointerThingObjectDef() : Object(0) {}
    ~CDefPointerThingObjectDef() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
};
class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointerThingObjectDef* out);               // 0x0042B040
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long globalIndex, CDefPointerThingObjectDef* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};

class CGameScriptInterface { public:
    void*                   World;                      // +0x04
    void*                   Component;                  // +0x08
    void*                   DisplayEngine;              // +0x0c
    CGameDefinitionManager* DefinitionManager;          // +0x10
    virtual CScriptThing CreateObject(const CCharString& defName, const C3DVector& position, float angle, const CCharString& scriptName) const;
};

CScriptThing CGameScriptInterface::CreateObject(const CCharString& defName, const C3DVector& position, float angle,
                                                const CCharString& scriptName) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CDefPointerThingObjectDef def;
    DefinitionManager->GetDef(index, &def);
    CRightHandedSet orientation(C3DVector(angle, 0.0f, 0.0f));
    CThingObject* object = CThingObject::Create(index, position, 4, &orientation, 0, scriptName);
    CScriptThing result;
    if (object)
        result = object->GetScriptThing();
    return result;
}