// CScriptThing::CScriptThing() -- default constructor: the CBaseClass base, the CScriptThing vtable, and a null PImp counted pointer
// Leaf: ??0CScriptThing@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7B40, 26 bytes, thiscall, plain `ret`, returns this.
// Identity: stores vtable 0x01238C8C (RTTI .?AVCScriptThing@@, direct base CBaseClass whose ctor 0x0099A2D0 it calls); Ego_r.pdb ??0CScriptThing@@QAE@XZ and member PImp (CCountedPointer<CScriptThing>, +4); 70 callers (by-value CScriptThing temporaries; agent950 F014 used it as the CScriptThing() member ctor).
// Shape (family K014, representative 0x006E7B40): out-of-line base ctor, then the vtable store, then the embedded
//   CCountedPointer's inline default ctor zeroing Data/Info through one zero register (a member object with its own
//   inline ctor stores after the vtable). Header debt: engine/CScriptThing.h (where present) flattens the vptr into data, so the
//   class is modelled locally from the PDB layout.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
CScriptThing::CScriptThing() { }