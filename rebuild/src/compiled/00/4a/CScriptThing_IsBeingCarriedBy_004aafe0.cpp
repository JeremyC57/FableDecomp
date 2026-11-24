// CScriptThing::IsBeingCarriedBy -- forwards to the implementation object (PImp.Data) through its vtable; the null-handle arm returns the default
// Leaf: ?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z
// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the script TUs (the
//   landed in-TU copies 0x00DCE230 / 0x00EF2290 define it inline; the ones that compile CScriptThing's vftable emit it pick-any)
//   and 0x004AAFE0 is the vftable's out-of-line copy. So this row defines the same inline member on the real class and emits it as a
//   pick-any COMDAT, not a strong definition (a strong one is LNK2005 against every emitted copy: NF318 evidence/link_cs.txt, NF326 evidence/).
//   Was the stand-in ?IsBeingCarriedBy@CScriptThing_Methods@@... on the flattened header (a local vtable view of the pointee).
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

inline bool CScriptThing::IsBeingCarriedBy(const CCharString& carrier) const
{
    const CScriptThing* imp = PImp.Data;
    if (!imp) return false;
    return imp->IsBeingCarriedBy(carrier);
}

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline IsBeingCarriedBy above with inline expansion switched off for
// this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004AAFE0, the vftable's copy; the
// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.
// Internal to this TU; never called.
#pragma inline_depth(0)
bool ReferenceScriptThingIsBeingCarriedBy(const CScriptThing& thing, const CCharString& carrier)
{
    return thing.CScriptThing::IsBeingCarriedBy(carrier);
}
#pragma inline_depth()
}
