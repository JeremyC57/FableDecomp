// CEngineLandscapeMemoryManager::AllocVirtualAligned(long size, unsigned long alignment, long unused) -- allocates `size + alignment + 3` bytes from the global CMemoryAllocator under the lock, rounds the block up to the alignment and stores the offset back to the raw block in the word before the aligned address; null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD9C40, length 88 bytes (96 with padding).
// Callees: GFCreateGlobalAllocator @ 0x009F2870 (when the global @ 0x013CAA90 is null), GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010,
// CMemoryAllocator::InternalAllocate(long, unsigned long, bool) @ 0x009F2A40 (size + alignment + 3, 0, 0). Shape: `this` and the third argument are unused
// (`ret 0Ch`); the padded size is one `lea ecx,[eax+edi+3]`, the rounding is `lea eax,[esi+edi+3] ; dec edi ; not edi ; and eax,edi` (`& ~(alignment - 1)`),
// the offset `aligned - block` goes to `[aligned-4]`; the null block returns through its own `xor eax,eax` tail.
struct CMemoryAllocator {
    void* InternalAllocate(long size, unsigned long alignment, bool flag);   // 0x009F2A40
};

void __cdecl GFCreateGlobalAllocator();            // 0x009F2870
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90

struct CEngineLandscapeMemoryManager {
    void* AllocVirtualAligned(long size, unsigned long alignment, long unused);
};

void* CEngineLandscapeMemoryManager::AllocVirtualAligned(long size, unsigned long alignment, long unused)
{
    if (!g_pGlobalAllocator_013caa90)
        GFCreateGlobalAllocator();
    GFEnterAllocatorLock();
    unsigned char* block = (unsigned char*)g_pGlobalAllocator_013caa90->InternalAllocate(size + alignment + 3, 0, false);
    GFLeaveAllocatorLock();
    if (!block)
        return 0;
    unsigned char* aligned = (unsigned char*)(((unsigned long)block + alignment + 3) & ~(alignment - 1));
    ((unsigned long*)aligned)[-1] = aligned - block;
    return aligned;
}
