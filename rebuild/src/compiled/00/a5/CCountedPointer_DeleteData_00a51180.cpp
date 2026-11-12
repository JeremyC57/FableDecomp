// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CSimpleMemoryPool>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCSimpleMemoryPool@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A51180, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A51180`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a51180, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCSimpleMemoryPool@@@@SIXPAX@Z; stored by CCountedPointer<CSimpleMemoryPool>::Reset 0x00A52BE0 (reloc_bindings, caller 0x00A4EE10); Ego_r.pdb C3DAnimation2::Allocator.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#include "engine/CSimpleMemoryPool.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CSimpleMemoryPool>::DeleteData(void*);
