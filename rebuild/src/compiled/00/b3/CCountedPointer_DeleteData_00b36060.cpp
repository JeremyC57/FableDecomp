// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerStaticMeshes>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerStaticMeshes@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36060, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36060`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36060, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerStaticMeshes@@@@SIXPAX@Z; stored by Reset 0x00B37470, called at 0x00B33E20 on `this+0x280` with a fresh CEnginePrimitiveManagerStaticMeshes (ctor 0x00BA1A70, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::StaticMeshManager (first of the 23 counted manager members, same order as the retail Reset calls 0x280..0x330).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEnginePrimitiveManagerStaticMeshes {
public:
    virtual ~CEnginePrimitiveManagerStaticMeshes();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerStaticMeshes>::DeleteData(void*);
