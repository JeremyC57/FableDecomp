// CScriptThing::GetDataString() -- the pointee's DataString string, or a copy of the static empty string when there is no pointee
// Leaf: ?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA900, length 42 bytes (`?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ`, a const virtual).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30; virtual slot 3 of the pointee.
// CScriptThing is a pimpl handle {vptr, CCountedPointer<CScriptThing> PImp}: every query forwards to the
// implementation object at PImp_Data (+0x04) through its vtable. engine/CScriptThing.h gives the
// (the landed GetPos 0x004AA980 / GetCurrentMapName 0x004AA9E0 siblings use the same model).
// Shape: `PImp_Data` loaded straight into ecx and null-tested; the hidden result pointer
// read into esi once for both arms. Null arm: the out-of-line CCharString copy constructor
// @ 0x0099EC30 builds the result from the global; else the virtual (slot 3, `call [eax+Ch]`)
// writes the result through the forwarded hidden pointer.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09), reached through the hdr opt-in
//   FABLE_RETAIL_CSCRIPTTHING_NULLSTRING on the flattened engine/CScriptThing.h; was g_EmptyString.
// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the script TUs (the
//   landed in-TU copies 0x00D75060 / 0x00DF6010 / 0x00E2B090 / 0x00E5EBD0 define it inline; the ones that compile CScriptThing's vftable emit it pick-any)
//   and 0x004AA900 is the vftable's out-of-line copy. So this row defines the same inline member on the real class and emits it as a
//   pick-any COMDAT, not a strong definition (a strong one is LNK2005 against every emitted copy: NF318 evidence/link_cs.txt, NF326 evidence/).
//   Was the stand-in ?GetDataString@CScriptThing_Methods@@... on the flattened header (a local vtable view of the pointee).
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CSCRIPTTHING_NULLSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

inline CCharString CScriptThing::GetDataString() const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return NullString;
    return imp->GetDataString();
}

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline GetDataString above with inline expansion switched off for
// this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004AA900, the vftable's copy; the
// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.
// Internal to this TU; never called.
#pragma inline_depth(0)
CCharString ReferenceScriptThingGetDataString(const CScriptThing& thing)
{
    return thing.CScriptThing::GetDataString();
}
#pragma inline_depth()
}
