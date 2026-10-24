// CMemoryAllocator::DeallocateAligned(void* p) -- free a block handed out by AllocateAligned: the real block pointer sits in the dword just below the aligned pointer; under the allocator lock give it back to the global allocator and, when the allocator is flagged to die once empty and no allocation is left, destroy it and release its pages
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2B20, length 108 bytes.
// Leaf: ?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z
// PDB: ?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z (public static __fastcall, the pointer in ecx).
// Callees: KERNEL32 EnterCriticalSection (IAT 0x0143FE0C) / LeaveCriticalSection (IAT 0x0143FE10) on the lock @ 0x013CAA70,
//   CMemoryAllocator::InternalDeallocate(void*) @ 0x009F2920, ~CMemoryAllocator @ 0x009F2310, VirtualFree (IAT 0x0143FD40).
// Globals: the allocator @ 0x013CAA90, the destroy-when-empty flag (byte) @ 0x013CAA94.
// Shape: the inline Deallocate(block) of the landed out-of-line twin GFFreeFromGlobalAllocator 0x009A4350 with every helper
//   inlined (`__forceinline` Deallocate/Destroy, Enter/LeaveSynchronisation, GetNoAllocations = NoAllocations +0x40c); Destroy() is
//   `delete Allocator; Allocator = 0;` through the inline `__fastcall` class operator delete (VirtualFree): with the count test
//   inlined on the same register VC7.1 drops the delete-expression's null test (ecx copied to esi for VirtualFree). An explicit
//   `a->~CMemoryAllocator(); VirtualFree(a, ..)` through a local loads the global straight into esi instead.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* address, unsigned long size, unsigned long freeType);   // IAT 0x0143FD40
extern unsigned char g_allocatorLock[24];   // 0x013CAA70 (CRITICAL_SECTION)

class CMemoryAllocator {
protected:
    unsigned long MemoryAllocatedPerTrackingID[256];  // +0x000
    unsigned long MemoryAllocated;                    // +0x400
    void**        MemoryAuditBuffer;                  // +0x404
    void*         Heap;                               // +0x408
    unsigned long NoAllocations;                      // +0x40c
    unsigned long NoAllocators;                       // +0x410
    unsigned char _rest[0x49c - 0x414];
    ~CMemoryAllocator();                              // 0x009F2310
    void InternalDeallocate(void* block);             // 0x009F2920
    static void __fastcall operator delete(void* p) { VirtualFree(p, 0, 0x8000); }   // MEM_RELEASE
    static CMemoryAllocator* Allocator;               // 0x013CAA90
    static bool DestroyWhenEmpty;                     // 0x013CAA94
    static void EnterSynchronisation() { EnterCriticalSection(g_allocatorLock); }
    static void LeaveSynchronisation() { LeaveCriticalSection(g_allocatorLock); }
    unsigned long GetNoAllocations() { return NoAllocations; }
    static __forceinline void Destroy()
    {
        delete Allocator;
        Allocator = 0;
    }
public:
    static __forceinline void Deallocate(void* block)
    {
        EnterSynchronisation();
        if (block && Allocator) {
            Allocator->InternalDeallocate(block);
            if (DestroyWhenEmpty && Allocator->GetNoAllocations() == 0)
                Destroy();
        }
        LeaveSynchronisation();
    }
    static void __fastcall DeallocateAligned(void* p);
};

void __fastcall CMemoryAllocator::DeallocateAligned(void* p)
{
    if (p)
        Deallocate(((void**)p)[-1]);
}