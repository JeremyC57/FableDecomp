// CScriptThing::GetDefName() -- the pointee's DefName string, or a copy of the static empty string when there is no pointee
// Leaf: ?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA8D0, length 42 bytes (`?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ`, a const virtual).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30; virtual slot 2 of the pointee.
// CScriptThing is a pimpl handle {vptr, CCountedPointer<CScriptThing> PImp}: every query forwards to the
// implementation object at PImp.Data (+0x04) through its vtable; engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT) gives the class.
// Shape: `PImp.Data` loaded straight into ecx and null-tested; the hidden result pointer
// read into esi once for both arms. Null arm: the out-of-line CCharString copy constructor
// @ 0x0099EC30 builds the result from CScriptThing::NullString (0x0143E8EC); else the virtual (slot 2, `call [eax+8h]`)
// writes the result through the forwarded hidden pointer.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09).
// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the NScript script TUs (the
//   landed copies 0x00E6A0B0 / 0x00E9CB20 / 0x00EDBD80 / 0x00EE2A80 define it in-TU and emit it pick-any with the CScriptThing vftable),
//   and 0x004AA8D0 is the vftable's out-of-line copy. So this row defines the same inline member and emits it as a pick-any COMDAT, not a
//   strong definition (a strong one is LNK2005 against every copy, NF318 evidence/link_cs.txt). Was the stand-in
//   ?GetDefName@CScriptThing_Methods@@QBE?AVCCharString@@XZ on the flattened header.
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CSCRIPTTHING_NULLSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

inline CCharString CScriptThing::GetDefName() const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return NullString;
    return imp->GetDefName();
}

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline GetDefName above with inline expansion switched off for
// this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004AA8D0, the vftable's copy; the
// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.
// Internal to this TU; never called.
#pragma inline_depth(0)
CCharString ReferenceScriptThingGetDefName(const CScriptThing& thing)
{
    return thing.CScriptThing::GetDefName();
}
#pragma inline_depth()
}
