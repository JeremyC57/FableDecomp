// CCountedPointer::Reset(T* data) -- (CCountedPointer<NParticleEngine::CParticleDataBank>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCParticleDataBank@NParticleEngine@@@@QAEXPAVCParticleDataBank@NParticleEngine@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE93E0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00AE91C0.
// Names (NF428, 2026-11-25; was the stand-in CCountedPointer_00ae91c0::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_00ae91c0@@QAEXPAX@Z): T from two sources: (1) the retail callers bind the PDB name (table, at 00ae8980), a whole
//   FableWin.pdb entry; (2) retail RTTI: a retail call site passes a freshly constructed NParticleEngine::CParticleDataBank (its vftable RTTI or ??0 call
//   just before the call; NF428 rtti428). The deleter the body stores, 0x00AE91C0, is bound to
//   ?DeleteData@?$CCountedPointer@VCParticleDataBank@NParticleEngine@@@@SIXPAX@Z, a name derived from (1), consistent with it.
// Form (NF371, landed 0x00447D90): a strong explicit specialization of engine/CCountedPointer.h's member with the landed body
//   (ReleaseData written out; Data = data; for a non-null data the count block re-reads Data through `this`; a failed new leaves Info 0).
//   FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL keeps the in-class body out, so the specialization is emitted as a plain function
//   (COMDAT selection 1) like the stand-in; FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY leaves the stored deleter an external
//   reference to its owner row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NParticleEngine { class CParticleDataBank; }

template <>
void CCountedPointer<NParticleEngine::CParticleDataBank>::Reset(NParticleEngine::CParticleDataBank* data)
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
