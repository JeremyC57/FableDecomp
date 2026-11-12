// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerAnimatedMeshes>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerAnimatedMeshes@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36070, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36070`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36070, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerAnimatedMeshes@@@@SIXPAX@Z; stored by Reset 0x00B37520, called at 0x00B33E49 on `this+0x288` with a fresh CEnginePrimitiveManagerAnimatedMeshes (ctor 0x00BD1EF0, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::AnimatedMeshManager (second counted manager member).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEnginePrimitiveManagerAnimatedMeshes {
public:
    virtual ~CEnginePrimitiveManagerAnimatedMeshes();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerAnimatedMeshes>::DeleteData(void*);
