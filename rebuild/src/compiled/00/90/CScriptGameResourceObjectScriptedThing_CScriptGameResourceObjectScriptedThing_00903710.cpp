// CScriptGameResourceObjectScriptedThing::CScriptGameResourceObjectScriptedThing(CGameScriptInterface* scriptInterface) -- constructor: empty handle base, the script interface, a null PThing, ScriptID 0, NullResource true, ActionPriority 0
// Leaf: ??0CScriptGameResourceObjectScriptedThing@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903710, length 73 bytes.
// Callees: CBaseObject::CBaseObject() @ 0x0099A380, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 and CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (PThing's inline default constructor).
// Vtables: 0x0128D86C (CScriptGameResourceObjectScriptedThing), 0x01238C6C (CIntelligentPointer).
// Shape: the implementation object derives from its own handle class CScriptGameResourceObjectScriptedThingBase
// (CBaseObject + CCountedPointer PImp, sizeof 0x10): the handle's inline default constructor runs first
// (CBaseObject call, PImp zeroed through the shared zero register, its own vtable store dead), then the
// derived vtable store, ScriptInterface (+0x10) from the argument, PThing's inline constructor (base call,
// vtable 0x01238C6C, SetPItem(0)), and the scalar initialisers ScriptID (+0x1c), ActionPriority (+0x24)
// and the bool NullResource (+0x20 = 1; VC schedules the byte store last).
// Layout: engine/CScriptGameResourceObjectScriptedThing.h (PDB, sizeof 0x28) agrees on every offset but
// is a flat record with no base classes or constructors, so the constructor needs this local model.
// The same holds for the base classes CScriptGameResourceObjectScriptedThingBase (engine header: PImp_Data
// +8, PImp_Info +0xc, sizeof 0x10) and CBaseObject (engine header: vftable, IntelligentPointer +4): both are
// modelled as polymorphic bases with the header offsets.
class CCPPointerInfo;
class CGameScriptInterface;

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectScriptedThingBase;
struct CCountedPointer_ScriptedThingBase {            // CCountedPointer<CScriptGameResourceObjectScriptedThingBase>
    CScriptGameResourceObjectScriptedThingBase* Data;
    CCPPointerInfo*                             Info;
    CCountedPointer_ScriptedThingBase() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:
    CCountedPointer_ScriptedThingBase PImp;           // +0x08 PImp_Data / +0x0c PImp_Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() { }
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase { public:   // vtable 0x0128D86C
    CGameScriptInterface* ScriptInterface;            // +0x10
    CIntelligentPointer<CThing>   PThing;                     // +0x14
    long                  ScriptID;                   // +0x1c
    bool                  NullResource;               // +0x20
    long                  ActionPriority;             // +0x24
    virtual void v00();
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface* scriptInterface);
};

CScriptGameResourceObjectScriptedThing::CScriptGameResourceObjectScriptedThing(CGameScriptInterface* scriptInterface)
    : ScriptInterface(scriptInterface), ScriptID(0), NullResource(true), ActionPriority(0)
{
}