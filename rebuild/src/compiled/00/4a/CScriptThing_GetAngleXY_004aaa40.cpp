// CScriptThing::GetAngleXY() -- forward the XY heading to the wrapped implementation, 0.0f when the handle is null
// Leaf: ?GetAngleXY@CScriptThing@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AAA40, 20 bytes (`?GetAngleXY@CScriptThing@@UBEMXZ`, vtable 0x01238C8C slot 10).
// Callees: the implementation object's own GetAngleXY (virtual slot 10 = +0x28); 0.0f comes from the
// float pool at 0x0122DEDC.
//
// Shape: `mov ecx,[ecx+4] ; test ; jne ; fld [0.0f] ; ret ; mov eax,[ecx] ; call [eax+28h] ; ret`.
// CScriptThing (PDB: 0xc bytes, vptr + CCountedPointer<CScriptThing> PImp {Data +4, Info +8}) is a
// pimpl handle: every virtual of the handle forwards to the SAME virtual of the CScriptThing held in
// PImp.Data. The engine header (engine/CScriptThing.h, FABLE_RETAIL_NSCRIPT) supplies the real class with
// its private CCountedPointer<CScriptThing> PImp (slot 10 = GetAngleXY, per rtti-vtables.tsv).
// The float result goes through a named local: VC7.1 sibling-calls (`jmp`) a float-returning
// forwarder otherwise, where retail kept `call ; ret`.
// One definition (NF326, 2026-11-24, the CScriptThing pick-any unit): retail expands this member inline in the script TUs (the
//   landed in-TU copies 0x00DD9520 / 0x00DDAE40 / 0x00DDB720 / 0x00E03F70 / 0x00E4E0E0 define it inline; the ones that compile CScriptThing's vftable emit it pick-any)
//   and 0x004AAA40 is the vftable's out-of-line copy. So this row defines the same inline member on the real class and emits it as a
//   pick-any COMDAT, not a strong definition (a strong one is LNK2005 against every emitted copy: NF318 evidence/link_cs.txt, NF326 evidence/).
//   Was the stand-in ?GetAngleXY@CScriptThing_Methods@@... on the flattened header (a local vtable view of the pointee).
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

inline float CScriptThing::GetAngleXY() const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return 0.0f;
    float angle = imp->GetAngleXY();
    return angle;
}

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the inline GetAngleXY above with inline expansion switched off for
// this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004AAA40, the vftable's copy; the
// nf86 / nf88 / NF92 pattern, as 0x00540CF0). The address of a virtual member is a vcall thunk and would not reference the body.
// Internal to this TU; never called.
#pragma inline_depth(0)
float ReferenceScriptThingGetAngleXY(const CScriptThing& thing)
{
    return thing.CScriptThing::GetAngleXY();
}
#pragma inline_depth()
}
