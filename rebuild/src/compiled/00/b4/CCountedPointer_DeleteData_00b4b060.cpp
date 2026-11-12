// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManager2DParticleGroup>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManager2DParticleGroup@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4B060, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B4B060`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b4b060, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManager2DParticleGroup@@@@SIXPAX@Z; stored by Reset 0x00B4B620, called at 0x00B4AD33 on `this+0x38` (Ego_r.pdb CEnginePrimitiveRenderer2D::Particle2DManager, PDB +0x3C, the last of its five counted managers).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DPARTICLEGROUP_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManager2DParticleGroup.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManager2DParticleGroup>::DeleteData(void*);
