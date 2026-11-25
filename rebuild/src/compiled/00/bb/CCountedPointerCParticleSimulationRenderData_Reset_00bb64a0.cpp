// CCountedPointer::Reset(T* data) -- (CCountedPointer<CParticleSimulationRenderData>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCParticleSimulationRenderData@@@@QAEXPAVCParticleSimulationRenderData@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB64A0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00BB6330.
// Names (NF428, 2026-11-25; was the stand-in CCountedPointer_00bb6330::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_00bb6330@@QAEXPAX@Z): T from one source: the retail callers bind the PDB name (table, at 00bb2540), a whole FableWin.pdb
//   entry. No retail call site shows a freshly constructed T (NF428 rtti428), so there is no second source; it lands under
//   the owner's best-guess rule (2026-11-25) on the name the table already binds at this address. The deleter the body
//   stores, 0x00BB6330, is bound to ?DeleteData@?$CCountedPointer@VCParticleSimulationRenderData@@@@SIXPAX@Z, a name derived from the same callers (not counted).
// Form (NF371, landed 0x00447D90): a strong explicit specialization of engine/CCountedPointer.h's member with the landed body
//   (ReleaseData written out; Data = data; for a non-null data the count block re-reads Data through `this`; a failed new leaves Info 0).
//   FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL keeps the in-class body out, so the specialization is emitted as a plain function
//   (COMDAT selection 1) like the stand-in; FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY leaves the stored deleter an external
//   reference to its owner row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CParticleSimulationRenderData;

template <>
void CCountedPointer<CParticleSimulationRenderData>::Reset(CParticleSimulationRenderData* data)
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Info = 0;
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);   // re-read through `this` (retail loads [esi] for the block), not the argument
}
