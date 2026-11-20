// CScriptThing::IsAlive() const -- script-handle proxy: false without an implementation object, else the implementation's IsAlive
// Leaf: ?IsAlive@CScriptThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AB130, 28 bytes, thiscall, `ret`: PImp.Data null -> 0; else `call [vtbl+12Ch]` (slot 75, the
//   same virtual on the implementation) and a 0/1 result.
// Identity (two sources): Ego_r.pdb 0x00429B90 / FableWin.pdb 0x018CD5F0 ?IsAlive@CScriptThing@@UBE_NXZ; CScriptThing vtable 0x01238C8C
//   slot 75 (engine/CScriptThing.h FABLE_RETAIL_NSCRIPT declares `virtual bool IsAlive() const; // 75 0x004AB130`); the 54 landed
//   callers already bind this spelling.
// Was: a non-virtual CScriptThing_Methods::IsAlive (int) on a derived stand-in over a local CScriptThingImplementation vtable model
//   (rebuild/src/compiled/00/4a/CScriptThing_IsAlive_004ab130.cpp).
// Header: engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT) over the shared engine/CCountedPointer.h, as 0x004AB170.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

bool CScriptThing::IsAlive() const
{
    return !PImp.IsNull() && PImp->IsAlive();
}
