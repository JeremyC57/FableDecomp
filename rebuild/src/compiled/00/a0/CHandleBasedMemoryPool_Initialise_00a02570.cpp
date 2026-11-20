// CHandleBasedMemoryPool::Initialise(long poolSize, bool multiThreaded, unsigned long memoryFlags) -- reset the pool, set up its critical section when multi-threaded, allocate PoolSize + 16 bytes from the global allocator, align the buffer to 16 and make the whole pool one free area
// Inline-budget: FABLE_FORCEINLINE on the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE; the landed 0x009F2B20 forces both the same way, and the HEAD rows forced their GFFree
//   helper) -- retail expands both at every site here; plain inline is called out of line (0x00BD9B90: Destroy DIFFER 68v97, then
//   Deallocate DIFFER 16v97). Preference-2 per-row /d2inl flags were not swept.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02570, length 151 bytes.
// Callees: CHandleBasedMemoryPool::Uninitialise 0x00A02190, InitializeCriticalSection (IAT 0x0143FEE4), CMemoryAllocator::Create 0x009F2870,
// CMemoryAllocator::EnterSynchronisation 0x009F2000, CMemoryAllocator::InternalAllocate 0x009F2A40, CMemoryAllocator::LeaveSynchronisation 0x009F2010, InsertFreeArea 0x00A01DA0.
// Shape: GFAllocateFromGlobalAllocator inlined (size from the argument register, flags in edi); the initial free area by the inline helper.
// Layout: engine/CHandleBasedMemoryPool.h puts the critical section at +0x450 behind a 16-byte Handles CArray;
// retail's CArray is 12 bytes, so the section sits at +0x44c (the local model below, as the landed
// BeginCritical 0x00A02140). CHeader as landed with InsertFreeArea (0x00A01DA0).
// Globals: the global CMemoryAllocator @ 0x013CAA90 and its destroy-when-empty flag @ 0x013CAA94 (as the
// landed GFFreeFromGlobalAllocator 0x009A4350 / CFileInstaller::InitBuffers 0x009C5310).
#include "fable_compiler.h"
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
// NF140 (q4197 / q4198): the frees are the inline public static CMemoryAllocator::Deallocate(block) (FableWin.pdb ?Deallocate@CMemoryAllocator@@SIXPAX@Z,
// out of line at 0x009A4350; engine/CMemoryAllocator.h opt-in FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, the body of the landed 0x009F2B20).
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };   // 24 bytes
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);       // IAT 0x0143FDE4
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);        // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);        // IAT 0x0143FE10

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

class CHandleBasedMemoryPool { public:
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
    FABLE_FORCEINLINE void CreateInitialFreeArea()     // the whole pool as one free area
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

void CHandleBasedMemoryPool::Initialise(long poolSize, bool multiThreaded, unsigned long memoryFlags)
{
    Uninitialise();
    MultiThreaded = multiThreaded;
    if (multiThreaded)
        InitializeCriticalSection(&CriticalSection);
    PoolSize = poolSize;
    MemoryFlags = memoryFlags;
    UnalignedBuffer = CMemoryAllocator::Allocate(PoolSize + 16, MemoryFlags);
    Buffer = (void*)(((unsigned long)UnalignedBuffer + 15) / 16 * 16);
    CreateInitialFreeArea();
}