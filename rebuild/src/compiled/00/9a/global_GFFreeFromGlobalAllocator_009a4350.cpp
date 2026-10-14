// GFFreeFromGlobalAllocator(void* block) -- under the allocator lock: return `block` to the global CMemoryAllocator and, when the allocator is flagged to die when empty and its allocation count has dropped to zero, destroy it and release its virtual memory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A4350, length 92 bytes.
// Callees: GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010 (EnterCriticalSection / LeaveCriticalSection on the section @ 0x013CAA70;
// the leave is the tail jump), CMemoryAllocator::InternalDeallocate(void*) @ 0x009F2920, CMemoryAllocator::GetAllocationCount() @ 0x009F1F10 (`[this+40Ch]`),
// CMemoryAllocator::~CMemoryAllocator @ 0x009F2310 (direct, non-virtual) and the class's `__fastcall` operator delete @ 0x009F1F90 (VirtualFree(p, 0, MEM_RELEASE)) -- the `delete` lowering: the pointer tested in ecx, copied to esi for the second call.
// Globals: the allocator @ 0x013CAA90, the destroy-when-empty flag (byte) @ 0x013CAA94. Shape: a `__fastcall` free function (the block in ecx, saved in esi);
// the teardown is `delete` of the global (loaded into ecx, saved in esi) and the store of zero. The same body with a null test on the block and a
// stack argument is CEngineLandscapeMemoryManager::FreeVirtual @ 0x00BD9B90; the chunk label (CEngineLandscapeMemoryManager::FreeVirtual) is propagated from it.
struct CMemoryAllocator {
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* block);   // 0x009F1F90 (VirtualFree(p, 0, MEM_RELEASE); the object lives in its own pages)
};

void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;       // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty_013caa94;   // 0x013CAA94

void __fastcall GFFreeFromGlobalAllocator(void* block)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator_013caa90) {
        g_pGlobalAllocator_013caa90->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty_013caa94) {
            if (g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}