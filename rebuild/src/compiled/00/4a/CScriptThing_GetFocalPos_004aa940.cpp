// Leaf: ?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (was ?GetFocalPos@CScriptThing_Methods@@QBE?AVC3DVector@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here. The body forwards to the
//   implementation through the private PImp (CCountedPointer<CScriptThing>, +0x04 Data), as the landed NScript in-TU inline copies spell it.
// CScriptThing::GetFocalPos() -- the pointee's FocalPos, or a copy of the static zero vector when there is no pointee
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA940, length 57 bytes (`?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ`, a const virtual).
// Callees: virtual slot 5 of the pointee.
// CScriptThing is a pimpl handle {vptr, CCountedPointer<CScriptThing> PImp}: every query forwards to the
// implementation object at PImp.Data (+0x04) through its vtable; engine/CScriptThing.h gives the handle layout
// (the landed GetPos 0x004AA980 / GetCurrentMapName 0x004AA9E0 siblings use the same model).
// Shape: `PImp_Data` loaded straight into ecx and null-tested. Null arm: the inline memberwise
// copy constructor of the global (x through the x87, y/z through integer registers) into the
// hidden result, no frame; else `push esi` of the hidden pointer and the virtual (slot 5,
// `call [eax+14h]`), returning that pointer.
// Preamble: the landed precedent CThingSwitch_vectordeletingdestructor_00ea86c0.cpp (CCountedPointer before the NScript class model).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"

C3DVector CScriptThing::GetFocalPos() const
{
    const CScriptThing* imp = PImp.Data;
    if (imp == 0)
        return NullCoord;
    return imp->GetFocalPos();
}
