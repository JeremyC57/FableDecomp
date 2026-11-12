// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManager2DAnimatedMeshes>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManager2DAnimatedMeshes@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4B030, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B4B030`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b4b030, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManager2DAnimatedMeshes@@@@SIXPAX@Z; stored by Reset 0x00B4B410, called at 0x00B4ACDD with a fresh CEnginePrimitiveManager2DAnimatedMeshes (ctor 0x00BD1F30, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer2D::AnimatedMesh2DManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEnginePrimitiveManager2DAnimatedMeshes {
public:
    virtual ~CEnginePrimitiveManager2DAnimatedMeshes();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManager2DAnimatedMeshes>::DeleteData(void*);
