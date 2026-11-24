// CScriptThing::GetName() const -- the implementation object's name, or the static empty string when there is no pointee
// Leaf: ?GetName@CScriptThing@@UBEABVCCharString@@XZ
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT with the PDB return
//   type (was ?GetName@CScriptThing_Methods@@QAEPAXXZ); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this
//   PDB member here. The body forwards through the private PImp (CCountedPointer<CScriptThing>, Data at +0x04) as the landed NScript
//   in-TU copies spell it; the forwarded call is the pointee's own virtual (slot 1).
// mov eax,[ecx+4]; test eax,eax; jnz V; mov eax,offset CScriptThing::NullString; ret
// V: mov edx,[eax]; mov ecx,eax; jmp/call [edx+4]  (virtual slot 1 on PImp_Data)
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09).
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

const CCharString& CScriptThing::GetName() const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return NullString;
    return imp->GetName();
}
