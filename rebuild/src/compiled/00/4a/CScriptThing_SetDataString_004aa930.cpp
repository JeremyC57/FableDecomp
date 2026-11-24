// CScriptThing::SetDataString -- forwards to the implementation object (PImp.Data) through its vtable; the null-handle arm returns the default
// CCountedPointer<CScriptThing> forwarder: if the counted pointee (PImp_Data, +0x4)
// Leaf: ?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z
// is non-null, dispatch its virtual slot 4 ([vtbl+0x10]) with the string argument.
// The header models the pointee as CScriptThing with a plain `void* __vftable`, so the
// virtual dispatch is expressed through a local interface view of the same vtable.
// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the script TUs (the
//   landed in-TU copies 0x00D17470 / 0x00D38000 / 0x00D387A0 / 0x00E338E0 define it inline; the ones that compile CScriptThing's vftable emit it pick-any)
//   and 0x004AA930 is the vftable's out-of-line copy. So this row defines the same inline member on the real class and emits it as a
//   pick-any COMDAT, not a strong definition (a strong one is LNK2005 against every emitted copy: NF318 evidence/link_cs.txt, NF326 evidence/).
//   Was the stand-in ?SetDataString@CScriptThing_Methods@@... on the flattened header (a local vtable view of the pointee).
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

inline void CScriptThing::SetDataString(const CCharString& data)
{
    CScriptThing* imp = PImp.Data;
    if (imp)
        imp->SetDataString(data);
}

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline SetDataString above with inline expansion switched off for
// this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004AA930, the vftable's copy; the
// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.
// Internal to this TU; never called.
#pragma inline_depth(0)
void ReferenceScriptThingSetDataString(CScriptThing& thing, const CCharString& data)
{
    thing.CScriptThing::SetDataString(data);
}
#pragma inline_depth()
}
