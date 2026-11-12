// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CMovableResourceMemoryPool>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCMovableResourceMemoryPool@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3E070, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B3E070`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b3e070, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCMovableResourceMemoryPool@@@@SIXPAX@Z; stored by Reset 0x00B3E4A0 (reloc_bindings stand-in ?Reset@CMemoryPoolPointer@@QAEXPAUCMemoryPool@@@Z, caller 0x00B3B240), called at 0x00B3B297 / 0x00B3B2F8 / 0x00B3B35F with a fresh CMovableResourceMemoryPool (ctor 0x00A62E20, RTTI vtable); Ego_r.pdb CEngineResourceManager::LoadingMemoryPool / AnimationMemoryPool.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#include "engine/CMovableResourceMemoryPool.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CMovableResourceMemoryPool>::DeleteData(void*);
