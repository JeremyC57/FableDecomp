// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<C3DAnimation2>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DAnimation2@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A282F0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A282F0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a282f0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VC3DAnimation2@@@@SIXPAX@Z; stored by CCountedPointer<C3DAnimation2>::Reset 0x00A29C50 (reloc_bindings, caller 0x00A24D70); Ego_r.pdb CAnimationData::SharedAnimation.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2
#include "engine/C3DAnimation2.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<C3DAnimation2>::DeleteData(void*);
