// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerParticleMeshGroup>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleMeshGroup@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36150, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36150`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36150, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleMeshGroup@@@@SIXPAX@Z; stored by Reset 0x00B37F00, called at 0x00B34023 on `this+0x300` with a fresh CEnginePrimitiveManagerParticleMeshGroup (ctor 0x00BC34D0, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::ParticleMeshGroupManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEMESHGROUP_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManagerParticleMeshGroup.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerParticleMeshGroup>::DeleteData(void*);
