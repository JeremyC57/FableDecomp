// CGameScriptInterface::CreateObject(const CCharString& defName, const C3DVector& position, float angle, const CCharString& scriptName) const -- create a thing object from the named def at the position, turned by the angle (an Euler-built orientation), player 4, and return its script thing (the null script thing when creation failed)
// Leaf: ?CreateObject@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@M0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00894950, length 324 bytes (+ int3 padding), `ret 14h` (hidden result + 4).
// The float overload of the landed CreateObject 0x00894820 (same TU): identical apart from the orientation, a
//   CRightHandedSet built by its Euler-angle constructor @ 0x00A229E0 from C3DVector(angle, 0, 0) (both frame locals).
// Callees: CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CDefinitionManager::GetDef<CThingObjectDef>
//   @ 0x0042B040, CRightHandedSet::CRightHandedSet(const C3DVector&) @ 0x00A229E0, CThingObject::Create(long, const
//   C3DVector&, long, const CRightHandedSet*, unsigned long, const CCharString&) @ 0x00703210 (static __fastcall),
//   CThing::GetScriptThing() @ 0x004C7E50 (by value), CScriptThing::operator= @ 0x008AB980, CBaseClass ctor/dtor
//   @ 0x0099A2D0 / 0x0099A2E0, operator delete @ 0x00BFE9BC, the def's slot 1. CScriptThing vtable 0x01238C8C.
// Layout / shape: as the landed 0x00894820 (CDefPointer homed in the dead defName slot, named result local, no NRVO).
#include "fable_compiler.h"
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    CRightHandedSet(const C3DVector& eulerAngles);      // 0x00A229E0
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }

static FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }
class CThingObjectDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
template <class T> class CDefPointer;
template <> class CDefPointer<const CThingObjectDef> { public:   // Ego_r.pdb CDefPointer<const CThingObjectDef> (was the row-local CDefPointerThingObjectDef)
    CThingObjectDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
};
class CDefinitionManager
{
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 GetDef<CThingObjectDef> (Ego_r.pdb member template)
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager { public:
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingObjectDef>(long globalIndex, CDefPointer<const CThingObjectDef>* out) const
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::CreateObject(const CCharString& defName, const C3DVector& position, float angle,
                                                const CCharString& scriptName) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CDefPointer<const CThingObjectDef> def;
    DefinitionManager->GetDef(index, &def);
    CRightHandedSet orientation(C3DVector(angle, 0.0f, 0.0f));
    CThingObject* object = CThingObject::Create(index, position, 4, &orientation, 0, scriptName);
    CScriptThing result;
    if (object)
        result = object->GetScriptThing();
    return result;
}