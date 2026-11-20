// GFFreeFromGlobalAllocator(void* block) -- under the allocator lock: return `block` to the global CMemoryAllocator and, when the allocator is flagged to die when empty and its allocation count has dropped to zero, destroy it and release its virtual memory
// Inline-budget: FABLE_FORCEINLINE on the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE; the landed 0x009F2B20 forces both the same way, and the HEAD row wrote the body out
//   in place) -- retail expands both at every site here; plain inline is called out of line (0x00BD9B90: Destroy DIFFER 68v97, then
//   Deallocate DIFFER 16v97). Preference-2 per-row /d2inl flags were not swept.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A4350, length 92 bytes.
// Callees: CMemoryAllocator::EnterSynchronisation @ 0x009F2000 / LeaveSynchronisation @ 0x009F2010 (EnterCriticalSection / LeaveCriticalSection on the section @ 0x013CAA70;
// the leave is the tail jump), CMemoryAllocator::InternalDeallocate(void*) @ 0x009F2920, CMemoryAllocator::GetNoAllocations() @ 0x009F1F10 (`[this+40Ch]`),
// CMemoryAllocator::~CMemoryAllocator @ 0x009F2310 (direct, non-virtual) and the class's `__fastcall` operator delete @ 0x009F1F90 (VirtualFree(p, 0, MEM_RELEASE)) -- the `delete` lowering: the pointer tested in ecx, copied to esi for the second call.
// Globals: the allocator @ 0x013CAA90, the destroy-when-empty flag (byte) @ 0x013CAA94. Shape: a `__fastcall` free function (the block in ecx, saved in esi);
// the teardown is `delete` of the global (loaded into ecx, saved in esi) and the store of zero. The same body with a null test on the block and a
// stack argument is CEngineLandscapeMemoryManager::FreeVirtual @ 0x00BD9B90; the chunk label (CEngineLandscapeMemoryManager::FreeVirtual) is propagated from it.
// NF140 (q4197 / q4198): the body is the inline public static CMemoryAllocator::Deallocate(block) of engine/CMemoryAllocator.h (opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE: the protected statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ /
//   ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ, InternalDeallocate, GetNoAllocations, the inline Destroy). This row's own name stays the
//   landed stand-in: 0x009A4350 is the out-of-line ?Deallocate@CMemoryAllocator@@SIXPAX@Z (FableWin.pdb; NAME_FIXES), a rename that needs a
//   one-definition form for the header inline and its referencing rows.
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"

void __fastcall GFFreeFromGlobalAllocator(void* block)
{
    CMemoryAllocator::Deallocate(block);
}
