// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerRepeatedStaticMeshes>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerRepeatedStaticMeshes@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36080, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36080`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36080, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerRepeatedStaticMeshes@@@@SIXPAX@Z; stored by Reset 0x00B375D0, called at 0x00B33E72 on `this+0x290` with a fresh CEnginePrimitiveManagerRepeatedStaticMeshes (ctor 0x00B99D90, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::RepeatedStaticMeshManager (3rd counted manager member).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerRepeatedStaticMeshes>::DeleteData(void*);
