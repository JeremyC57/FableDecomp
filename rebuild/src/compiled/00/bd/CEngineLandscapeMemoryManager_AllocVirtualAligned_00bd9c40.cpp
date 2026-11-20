// CEngineLandscapeMemoryManager::AllocVirtualAligned(long size, unsigned long alignment, long unused) -- allocates `size + alignment + 3` bytes from the global CMemoryAllocator under the lock, rounds the block up to the alignment and stores the offset back to the raw block in the word before the aligned address; null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD9C40, length 88 bytes (96 with padding).
// Callees: CMemoryAllocator::Create @ 0x009F2870 (when PMemoryAllocator @ 0x013CAA90 is null), CMemoryAllocator::EnterSynchronisation @ 0x009F2000 / CMemoryAllocator::LeaveSynchronisation @ 0x009F2010,
// CMemoryAllocator::InternalAllocate(long, unsigned long, bool) @ 0x009F2A40 (size + alignment + 3, 0, 0). Shape: `this` and the third argument are unused
// (`ret 0Ch`); the padded size is one `lea ecx,[eax+edi+3]`, the rounding is `lea eax,[esi+edi+3] ; dec edi ; not edi ; and eax,edi` (`& ~(alignment - 1)`),
// the offset `aligned - block` goes to `[aligned-4]`; the null block returns through its own `xor eax,eax` tail.
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
#include "engine/CMemoryAllocator.h"


class CEngineLandscapeMemoryManager { public:
    void* AllocVirtualAligned(long size, unsigned long alignment, long unused);
};

void* CEngineLandscapeMemoryManager::AllocVirtualAligned(long size, unsigned long alignment, long unused)
{
    unsigned char* block = (unsigned char*)CMemoryAllocator::Allocate(size + alignment + 3, 0);
    if (!block)
        return 0;
    unsigned char* aligned = (unsigned char*)(((unsigned long)block + alignment + 3) & ~(alignment - 1));
    ((unsigned long*)aligned)[-1] = aligned - block;
    return aligned;
}