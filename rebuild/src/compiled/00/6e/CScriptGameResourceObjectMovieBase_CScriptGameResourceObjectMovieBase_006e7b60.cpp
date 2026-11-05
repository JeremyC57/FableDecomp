// CScriptGameResourceObjectMovieBase::CScriptGameResourceObjectMovieBase() -- default constructor: the CBaseObject base, the CScriptGameResourceObjectMovieBase vtable, and a null PImp counted pointer
// Leaf: ??0CScriptGameResourceObjectMovieBase@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7B60, 26 bytes, thiscall, plain `ret`, returns this.
// Identity: stores vtable 0x01260EF4 (RTTI .?AVCScriptGameResourceObjectMovieBase@@, bases CScriptGameResourceObjectBase -> CBaseObject (ctor 0x0099A380) -> CBaseClass); Ego_r.pdb ??0CScriptGameResourceObjectMovieBase@@QAE@XZ and member PImp (+8).
// Shape (family K014, representative 0x006E7B40): out-of-line base ctor, then the vtable store, then the embedded
//   CCountedPointer's inline default ctor zeroing Data/Info through one zero register (a member object with its own
//   inline ctor stores after the vtable). Header debt: engine/CScriptGameResourceObjectMovieBase.h (where present) flattens the vptr into data, so the
//   class is modelled locally from the PDB layout.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class __declspec(novtable) CScriptGameResourceObjectBase : public CBaseObject {   // RTTI base; its inline ctor's vtable store is dead
protected:
    CScriptGameResourceObjectBase() { }
};
#include "engine/CCountedPointer.h"

class CScriptGameResourceObjectMovieBase : public CBaseObject {
public:
    CScriptGameResourceObjectMovieBase();
    virtual ~CScriptGameResourceObjectMovieBase();
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;   // +0x08
};
CScriptGameResourceObjectMovieBase::CScriptGameResourceObjectMovieBase() { }