// CScriptThing::GetPos -- forwards to the implementation object (PImp.Data) through its vtable; the null-handle arm returns the default
// Leaf: ?GetPos@CScriptThing@@UBEABVC3DVector@@XZ
// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the script TUs (the
//   landed in-TU copies 0x0089AA70 / 0x0089EF90 / 0x00D13580 / 0x00D17470 / 0x00D17F20 / 0x00D18DF0 / 0x00D19BD0 / 0x00D1A740 and 78 more define it inline; the ones that compile CScriptThing's vftable emit it pick-any)
//   and 0x004AA980 is the vftable's out-of-line copy. So this row defines the same inline member on the real class and emits it as a
//   pick-any COMDAT, not a strong definition (a strong one is LNK2005 against every emitted copy: NF318 evidence/link_cs.txt, NF326 evidence/).
//   Was the stand-in ?GetPos@CScriptThing_Methods@@... on the flattened header (a local vtable view of the pointee).
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

inline const C3DVector& CScriptThing::GetPos() const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return NullCoord;
    return imp->GetPos();
}

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline GetPos above with inline expansion switched off for
// this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004AA980, the vftable's copy; the
// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.
// Internal to this TU; never called.
#pragma inline_depth(0)
const C3DVector& ReferenceScriptThingGetPos(const CScriptThing& thing)
{
    return thing.CScriptThing::GetPos();
}
#pragma inline_depth()
}
