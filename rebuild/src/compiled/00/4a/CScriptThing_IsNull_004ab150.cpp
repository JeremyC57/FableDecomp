// CScriptThing::IsNull() -- true when the handle has no implementation, else the implementation's own IsNull (slot 77)
// Leaf: ?IsNull@CScriptThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AB150, length 28 bytes (`?IsNull@CScriptThing@@UBE_NXZ`, a const virtual, vtable
//   0x01238C8C slot 77).
// Shape: `PImp.Data` loaded straight into ecx and null-tested (true), else `call [eax+134h]` (slot 77 of the pointee) and its result
//   normalised to 0 / 1.
// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the script TUs (the landed
//   copies 0x008A8AF0 / 0x00E0DE90 / 0x00E6A0B0 / 0x00E9CB20 / 0x00EDBD80 / 0x00EE2A80 define it in-TU and emit it pick-any with the
//   CScriptThing vftable), and 0x004AB150 is the vftable's out-of-line copy. So this row defines the same inline member and emits it as a
//   pick-any COMDAT, not a strong definition (a strong one is LNK2005 against every copy, NF318 evidence/link_cs.txt). Was the stand-in
//   ?IsNull@CScriptThing_Methods@@QBE_NXZ over a local 78-slot implementation view.
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

inline bool CScriptThing::IsNull() const
{
    const CScriptThing* imp = PImp.Data;
    return imp == 0 || imp->IsNull();
}

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline IsNull above with inline expansion switched off for
// this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004AB150, the vftable's copy; the
// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.
// Internal to this TU; never called.
#pragma inline_depth(0)
bool ReferenceScriptThingIsNull(const CScriptThing& thing)
{
    return thing.CScriptThing::IsNull();
}
#pragma inline_depth()
}
