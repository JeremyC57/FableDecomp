// Leaf: ?GetCurrentMapName@CScriptThing@@UBE?AVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (was ?GetCurrentMapName@CScriptThing_Methods@@QAE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here. The body forwards to the
//   implementation through the private PImp (CCountedPointer<CScriptThing>, +0x04 Data), as the landed NScript in-TU inline copies spell it.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09); the null branch copies it with the out-of-line
//   CCharString(const CCharString&) 0x0099EC30 (reached through the hdr opt-in FABLE_RETAIL_CSCRIPTTHING_NULLSTRING on the flattened
//   engine/CScriptThing.h). Was kDefaultMapName (a char array) through CCharString(const char*), neither of which retail references.
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CSCRIPTTHING_NULLSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

CCharString CScriptThing::GetCurrentMapName() const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return NullString;
    return imp->GetCurrentMapName();
}
