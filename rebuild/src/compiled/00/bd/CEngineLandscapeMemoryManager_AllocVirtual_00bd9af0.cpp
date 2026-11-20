// CEngineLandscapeMemoryManager::AllocVirtual(long size, long unused) -- allocates `size` bytes from the global CMemoryAllocator (created on first use) under the allocator lock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD9AF0, length 51 bytes (64 with padding).
// Callees: CMemoryAllocator::Create @ 0x009F2870 (when PMemoryAllocator @ 0x013CAA90 is null), CMemoryAllocator::EnterSynchronisation @ 0x009F2000 / CMemoryAllocator::LeaveSynchronisation @ 0x009F2010,
// CMemoryAllocator::InternalAllocate(long size, unsigned long, bool) @ 0x009F2A40 (size, 0, 0). Shape: `this` and the second argument are unused (`ret 8`);
// the size is read from its slot after the lock call, the block rides in esi across the unlock.
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
#include "engine/CMemoryAllocator.h"


class CEngineLandscapeMemoryManager { public:
    void* AllocVirtual(long size, long unused);
};

void* CEngineLandscapeMemoryManager::AllocVirtual(long size, long unused)
{
    void* block = CMemoryAllocator::Allocate(size, 0);
    return block;
}