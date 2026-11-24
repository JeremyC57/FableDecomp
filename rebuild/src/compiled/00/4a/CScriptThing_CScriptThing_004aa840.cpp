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
// One definition (NF328, 2026-11-24, the NF326 Q4 follow-up unit): retail expands this destructor inline in the script TUs, and the
//   landed script rows define it `inline` after the header and emit it pick-any with the CScriptThing vftable. A strong definition here
//   is LNK2005 against every such copy (NF326 evidence/link2_dtor_head.txt), so this row defines the same inline destructor and emits it
//   pick-any through the helper below.
void __cdecl operator delete(void* block);              // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

inline CScriptThing::~CScriptThing()
{
}

namespace {
// Inline-budget: scaffolding, not retail: a qualified (non-virtual) explicit destructor call of the inline destructor above, with inline
// expansion switched off for this one function, so this object emits ??1CScriptThing@@UAE@XZ out of line as a pick-any COMDAT (retail's
// 0x004AA840, the out-of-line copy) like every script TU that defines the destructor inline (the nf86 / nf88 / NF326 pattern). The
// address of a destructor cannot be taken, hence the call. Internal to this TU; never called.
#pragma inline_depth(0)
void ReferenceScriptThingDestructor(CScriptThing& thing)
{
    thing.CScriptThing::~CScriptThing();
}
#pragma inline_depth()
}