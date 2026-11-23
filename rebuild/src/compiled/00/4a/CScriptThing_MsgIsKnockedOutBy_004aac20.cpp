// CScriptThing::MsgIsKnockedOutBy -- forwards to the implementation object through the private PImp (null -> false)
// Leaf: ?MsgIsKnockedOutBy@CScriptThing@@UBE_NABVCCharString@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT with the PDB parameter
//   type (was ?MsgIsKnockedOutBy@CScriptThing_Methods@@QAE_NH@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
//   The body forwards through the private PImp (CCountedPointer<CScriptThing>, Data at +0x04) as the landed NScript in-TU copies spell
//   it; the forwarded call is the pointee's own virtual (vtable slot / address: 32 0x004AAC20).
// CScriptThing::MsgIsKnockedOutBy @ 0x004aac20
// mov ecx,[ecx+4]; test; jne L; xor al,al; ret 4; L: mov eax,[ecx]; jmp [eax+0x80]
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

bool CScriptThing::MsgIsKnockedOutBy(const CCharString& name) const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return false;
    return imp->MsgIsKnockedOutBy(name);
}
