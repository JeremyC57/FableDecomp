// CScriptThing::~CScriptThing() -- release the counted implementation pointer PImp, then the CBaseClass base destructor
// Leaf: ??1CScriptThing@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA840, 66 bytes, thiscall, tail `jmp ~CBaseClass`.
// Identity (proven): stores vtable 0x01238C8C first -- RTTI .?AVCScriptThing@@ : CBaseClass (rtti-vtables.tsv; engine/CScriptThing.h:
//   79 slots, slot 0 `??_G` 0x004AB170 calls this body); Ego_r.pdb and FableWin.pdb both spell ??1CScriptThing@@UAE@XZ (virtual:
//   CBaseClass's destructor is virtual); 4,187 callers, the landed NScript quest / entity scripts destroying CScriptThing locals
//   and members (chunks 693 / 694 expand it at some exits and call it at others). The catalog label "~C3DClothPrimitive" is
//   propagated from the masked byte twin 0x00AC3B80, which stores the C3DClothPrimitive vtable 0x0129DF74 and tail-jumps to
//   ~CBaseClassNonCopyable 0x0099A300: that copy is ??1C3DClothPrimitive, a different symbol.
// Callees: operator delete 0x00BFE9BC (the CCPPointerInfo block), the info block's DeleteFunc through +4,
//   CBaseClass::~CBaseClass 0x0099A2E0.
// Shape: an empty user destructor; the PImp member destructor is the inline CCountedPointer<T>::~CCountedPointer of
//   engine/CCountedPointer.h (`--Info->RefCount`, Info reloaded for the zero test, DeleteFunc(Info->Data), `delete Info`,
//   then Data and Info zeroed in that order). The vtable store comes first because the destructor is user-written.
// Layout: engine/CScriptThing.h retail model (FABLE_RETAIL_NSCRIPT): CBaseClass vptr +0, PImp CCountedPointer<CScriptThing> +4.
void __cdecl operator delete(void* block);              // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

CScriptThing::~CScriptThing()
{
}