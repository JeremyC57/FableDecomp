// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerParticleLineGroup>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleLineGroup@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36140, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36140`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36140, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleLineGroup@@@@SIXPAX@Z; stored by Reset 0x00B37E50, called at 0x00B33FFA on `this+0x2F8` with a fresh CEnginePrimitiveManagerParticleLineGroup (ctor 0x00BC4D90, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::ParticleLineGroupManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLELINEGROUP_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerParticleLineGroup>::DeleteData(void*);
