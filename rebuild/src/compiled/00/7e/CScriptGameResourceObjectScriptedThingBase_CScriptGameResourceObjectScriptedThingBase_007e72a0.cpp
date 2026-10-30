// CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase() -- default constructor: the CBaseObject base, the CScriptGameResourceObjectScriptedThingBase vtable, and a null PImp counted pointer
// Leaf: ??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E72A0, 26 bytes, thiscall, plain `ret`, returns this.
// Identity: stores vtable 0x0127094C (RTTI .?AVCScriptGameResourceObjectScriptedThingBase@@, bases CScriptGameResourceObjectBase -> CBaseObject (ctor 0x0099A380) -> CBaseClass); Ego_r.pdb ??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ and member PImp (+8); agent950 F014 identified it.
// Shape (family K014, representative 0x006E7B40): out-of-line base ctor, then the vtable store, then the embedded
//   CCountedPointer's inline default ctor zeroing Data/Info through one zero register (a member object with its own
//   inline ctor stores after the vtable). Header debt: engine/CScriptGameResourceObjectScriptedThingBase.h (where present) flattens the vptr into data, so the
//   class is modelled locally from the PDB layout.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {                  // Ego_r.pdb, 8 bytes
protected:
    CBaseObject();                                       // 0x0099A380 (PDB ??0CBaseObject@@IAE@XZ)
    virtual ~CBaseObject();
    CBaseObjectPointer* IntelligentPointer;              // +0x04
};
class __declspec(novtable) CScriptGameResourceObjectBase : public CBaseObject {   // RTTI base; its inline ctor's vtable store is dead
protected:
    CScriptGameResourceObjectBase() { }
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject {
public:
    CScriptGameResourceObjectScriptedThingBase();
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
};
CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase() { }