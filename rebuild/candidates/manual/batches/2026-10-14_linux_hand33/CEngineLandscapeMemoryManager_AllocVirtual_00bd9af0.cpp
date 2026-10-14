// CEngineLandscapeMemoryManager::AllocVirtual(long size, long unused) -- allocates `size` bytes from the global CMemoryAllocator (created on first use) under the allocator lock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD9AF0, length 51 bytes (64 with padding).
// Callees: GFCreateGlobalAllocator @ 0x009F2870 (when the global @ 0x013CAA90 is null), GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010,
// CMemoryAllocator::InternalAllocate(long size, unsigned long, bool) @ 0x009F2A40 (size, 0, 0). Shape: `this` and the second argument are unused (`ret 8`);
// the size is read from its slot after the lock call, the block rides in esi across the unlock.
struct CMemoryAllocator {
    void* InternalAllocate(long size, unsigned long alignment, bool flag);   // 0x009F2A40
};

void __cdecl GFCreateGlobalAllocator();            // 0x009F2870
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90

struct CEngineLandscapeMemoryManager {
    void* AllocVirtual(long size, long unused);
};

void* CEngineLandscapeMemoryManager::AllocVirtual(long size, long unused)
{
    if (!g_pGlobalAllocator_013caa90)
        GFCreateGlobalAllocator();
    GFEnterAllocatorLock();
    void* block = g_pGlobalAllocator_013caa90->InternalAllocate(size, 0, false);
    GFLeaveAllocatorLock();
    return block;
}
