// CHandleBasedMemoryPool::Uninitialise() -- return the pool's unaligned buffer to the global allocator (destroying the allocator when it is flagged to die empty), delete the critical section of a multi-threaded pool and zero the bookkeeping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02190, length 143 bytes.
// Callees: GFEnterAllocatorLock 0x009F2000 / GFLeaveAllocatorLock 0x009F2010, CMemoryAllocator::InternalDeallocate 0x009F2920,
// GetAllocationCount 0x009F1F10, ~CMemoryAllocator 0x009F2310, operator delete 0x009F1F90, DeleteCriticalSection (IAT 0x0143FDE4).
// Shape: GFFreeFromGlobalAllocator (0x009A4350) inlined -- its own null test of the block repeats the caller's; zero in ebx.
// Layout: engine/CHandleBasedMemoryPool.h puts the critical section at +0x450 behind a 16-byte Handles CArray;
// retail's CArray is 12 bytes, so the section sits at +0x44c (the local model below, as the landed
// BeginCritical 0x00A02140). CHeader as landed with InsertFreeArea (0x00A01DA0).
// Globals: the global CMemoryAllocator @ 0x013CAA90 and its destroy-when-empty flag @ 0x013CAA94 (as the
// landed GFFreeFromGlobalAllocator 0x009A4350 / CFileInstaller::InitBuffers 0x009C5310).
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };   // 24 bytes
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);       // IAT 0x0143FDE4
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);        // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);        // IAT 0x0143FE10

struct CMemoryAllocator {
    void* InternalAllocate(long size, unsigned long flags, bool flag);   // 0x009F2A40
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* block);   // 0x009F1F90
};
void __cdecl GFCreateGlobalAllocator();            // 0x009F2870
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;               // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty_013caa94;     // 0x013CAA94

__forceinline void* GFAllocateFromGlobalAllocator(long size, unsigned long flags)
{
    if (!g_pGlobalAllocator_013caa90)
        GFCreateGlobalAllocator();
    GFEnterAllocatorLock();
    void* block = g_pGlobalAllocator_013caa90->InternalAllocate(size, flags, false);
    GFLeaveAllocatorLock();
    return block;
}
__forceinline void GFFreeFromGlobalAllocator(void* block)
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

struct CHeader {                                  // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    long           Handle;                         // +0x00
    long           _unused04;                      // +0x04
    long           Size;                           // +0x08
    short          Padding;                        // +0x0c
    char           Type;                           // +0x0e ('N' free, 'T' taken)
    char           Flags;                          // +0x0f
    CHeader**      PPrevFreeLink;                  // +0x10
    CHeader*       PNextFree;                      // +0x14
    CHeader*       PPrevPhysical;                  // +0x18
    CHeader*       PNextPhysical;                  // +0x1c
};

struct CHandleBasedMemoryPool {
    void*                        __vftable;        // +0x00
    void*                        Buffer;           // +0x04 (16-byte aligned)
    void*                        UnalignedBuffer;  // +0x08 (from the global allocator)
    long                         PoolSize;         // +0x0c
    long                         AllocatedSize;    // +0x10
    long                         NumAllocations;   // +0x14
    long                         LockCount;        // +0x18
    long                         LargestFreeArea;  // +0x1c
    unsigned long                MemoryFlags;      // +0x20
    bool                         MultiThreaded;    // +0x24
    bool                         ShutdownFlag;     // +0x25
    unsigned char                _pad26[0x44c - 0x26];
    _RTL_CRITICAL_SECTION_Retail CriticalSection;  // +0x44c (PDB +0x450)
    void InsertFreeArea(CHeader* area);            // 0x00A01DA0
    void Uninitialise();                           // 0x00A02190
    void EndShutdown();
    void Initialise(long poolSize, bool multiThreaded, unsigned long memoryFlags);
    void InitialiseFromPreallocatedBuffer(void* buffer, long size, bool multiThreaded);
    __forceinline void CreateInitialFreeArea()     // the whole pool as one free area
    {
        CHeader* area = (CHeader*)Buffer;
        area->Size = PoolSize - sizeof(CHeader);
        area->Padding = 0;
        area->Type = 'N';
        area->Flags = 0;
        area->PPrevPhysical = 0;
        area->PNextPhysical = 0;
        area->Handle = 0;
        InsertFreeArea(area);
    }
};

void CHandleBasedMemoryPool::Uninitialise()
{
    if (UnalignedBuffer) {
        GFFreeFromGlobalAllocator(UnalignedBuffer);
        UnalignedBuffer = 0;
    }
    Buffer = 0;
    PoolSize = 0;
    if (MultiThreaded) {
        DeleteCriticalSection(&CriticalSection);
        MultiThreaded = false;
    }
    AllocatedSize = 0;
    NumAllocations = 0;
    LockCount = 0;
    LargestFreeArea = 0;
}
