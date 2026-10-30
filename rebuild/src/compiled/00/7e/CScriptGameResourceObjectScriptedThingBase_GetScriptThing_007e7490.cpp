// CScriptGameResourceObjectScriptedThingBase::GetScriptThing() -- the implementation's script thing, or an empty (null) CScriptThing when there is no implementation
// Leaf: GetScriptThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E7490, length 57 bytes
// (`?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ`, a const virtual).
// Callees: CBaseClass::CBaseClass() @ 0x0099A2D0 (null arm); virtual slot 12 (+0x30) of the implementation.
// Shape: PImp_Data (+8, engine header) loaded straight into ecx and null-tested; the hidden result
// pointer read into esi once for both arms. Null arm: the inline default constructor of the returned
// CScriptThing -- the out-of-line CBaseClass base constructor, the CScriptThing vtable 0x01238C8C,
// then its PImp counted pointer zeroed by the member's own inline constructor (immediate zero stores
// after the vtable store). Else the virtual writes the result through the forwarded hidden pointer.
// `this` uses engine/CScriptGameResourceObjectScriptedThingBase.h; the returned CScriptThing needs a
// constructor and a base class, so it is a local model (engine/CScriptThing.h's flat {vftable, PImp_Data,
// PImp_Info} layout, same offsets).
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct CScriptThingImp;
struct CCountedPointer_CScriptThing {
    CScriptThingImp* Data;
    CCPPointerInfo*  Info;
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};

class CScriptThing : public CBaseClass { public:                    // vtable 0x01238C8C
    CCountedPointer_CScriptThing PImp;                // +0x04 PImp_Data / +0x08 PImp_Info
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);
};

struct CScriptedThingBaseVirtuals {                   // vtable view of the implementation object
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;      // slot 12 (+0x30)
};

struct CScriptGameResourceObjectScriptedThingBase_Methods : CScriptGameResourceObjectScriptedThingBase {
    CScriptThing GetScriptThing() const;
};

CScriptThing CScriptGameResourceObjectScriptedThingBase_Methods::GetScriptThing() const
{
    const CScriptedThingBaseVirtuals* imp = (const CScriptedThingBaseVirtuals*)PImp_Data;
    if (imp == 0)
        return CScriptThing();
    return imp->GetScriptThing();
}