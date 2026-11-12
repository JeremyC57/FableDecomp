// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerParticleGroup>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleGroup@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36180, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36180`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36180, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleGroup@@@@SIXPAX@Z; stored by Reset 0x00B38110, called at 0x00B3409F on `this+0x318` with a fresh CEnginePrimitiveManagerParticleGroup (ctor 0x00BBBF40, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::ParticleGroupManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEnginePrimitiveManagerParticleGroup {
public:
    virtual ~CEnginePrimitiveManagerParticleGroup();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerParticleGroup>::DeleteData(void*);
