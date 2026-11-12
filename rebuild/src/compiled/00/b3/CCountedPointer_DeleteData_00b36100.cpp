// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveOcclusionGroupManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveOcclusionGroupManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36100, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36100`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36100, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveOcclusionGroupManager@@@@SIXPAX@Z; stored by Reset 0x00B37B90, called at 0x00B33F85 on `this+0x2D8` with an object built by ctor 0x00B90270 (RTTI vtables of CEnginePrimitiveManagerOcclusionBase); Ego_r.pdb CEnginePrimitiveRenderer::OcclusionGroupManager, the member between GameMeshManager (+0x2D0) and PrimitiveGroupManager (+0x2E0).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONGROUPMANAGER_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveOcclusionGroupManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveOcclusionGroupManager>::DeleteData(void*);
