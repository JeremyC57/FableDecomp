// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerParticleSpriteTrailGroup>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleSpriteTrailGroup@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36170, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36170`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36170, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleSpriteTrailGroup@@@@SIXPAX@Z; stored by Reset 0x00B38060, called at 0x00B34079 on `this+0x310` with a fresh CEnginePrimitiveManagerParticleSpriteTrailGroup (ctor 0x00BBEAF0, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::ParticleSpriteTrailGroupManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITETRAILGROUP_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerParticleSpriteTrailGroup>::DeleteData(void*);
