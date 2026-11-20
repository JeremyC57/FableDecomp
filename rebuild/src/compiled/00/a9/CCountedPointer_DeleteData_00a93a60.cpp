// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<C3DClothPrimitive>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A93A60, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: the CCountedPointer<C3DClothPrimitive> count block stores it (`mov [eax+4], 0x00A93A60` in Reset 0x00A94DF0 and in
//   the cloth loop of C3DPrimitive2::InitialiseFromMeshFileData 0x00A91640, which binds this spelling).
// Name (NF197, 2026-11-20): FableWin.pdb 0x03306EA0 ?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@SIXPAX@Z (engine/CCountedPointer.h
//   DeleteData = `delete static_cast<T*>(data)`), Ego_r.pdb C3DClothPrimitive is polymorphic (vfptr +0x00). Was the gapscan stand-in
//   extern "C" DeleteIfSet over a local `Obj` (catalog _gapscan::sub_00a93a60). Same recipe as 0x00BB6330 (CParticleSimulationRenderData).
#define FABLE_RETAIL_C3DCLOTHPRIMITIVE_VIRTUAL_DTOR
#include "engine/C3DClothPrimitive.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<C3DClothPrimitive>::DeleteData(void*);
