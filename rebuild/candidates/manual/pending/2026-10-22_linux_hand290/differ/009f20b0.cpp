// Grade: DIFFER(596v598) -- register allocation only: retail keeps a zero register in ebx through the first loop (cmp/push ebx, bl store) and reads heap from its argument slot each time (frame 8); ours keeps heap in ebx and uses immediates (frame 4). Register-masked diff otherwise identical.
// CMemoryAllocator::CMemoryAllocator(void* heap) -- build the global allocator: create and take the allocator lock, initialise the memory system, clear the per-type tables, create a fixed-size or variable-size sub-allocator for every used entry of the 30-entry allocator-info table, then build each type's size -> allocator lookup array
// Leaf: ??0CMemoryAllocator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F20B0, length 608 bytes (+ int3 padding), `ret 4`.
// Callees: KERNEL32 InitializeCriticalSection / EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FEE4 / 0x0143FE0C /
//   0x0143FE10) on the lock 0x013CAA70, InitializeMemorySystem 0x009F1D20 (__fastcall, no arguments), the sub-allocator
//   class operator new 0x00A66730 (__fastcall size), CMemoryAllocatorFixedSize ctor 0x00A65FF0, CMemoryAllocatorVariableSize
//   ctor 0x00A65930, sub-allocator virtual slot 3 (+0xc), __allmul 0x00BFEB50, malloc 0x00BFEA0E.
// Layout: PDB CMemoryAllocator (0x49c, as the landed dtor 0x009F2310); PDB CAllocatorInfo (0x1c; Flags +0x10 is the
//   allocator type index 0..14, InternalFlags bit 1 = fixed size), table at 0x01396868; PDB CAllocSizeToAllocator (8 bytes).
// Shape: the array allocation's 64-bit size check returns out of the constructor (no lock release) when it fails.
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);        // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);        // IAT 0x0143FE10
extern "C" void* __cdecl malloc(unsigned int size);                                         // 0x00BFEA0E
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)
extern unsigned char g_allocatorLock[24];   // 0x013CAA70
void __fastcall InitializeMemorySystem();   // 0x009F1D20

struct CMemoryAllocatorBase {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual long GetMaxAllocationSize(long reserved);                  // slot 3 (+0xc)
    static void* __fastcall operator new(unsigned int size);           // 0x00A66730
};
struct CMemoryAllocatorFixedSize : CMemoryAllocatorBase {
    unsigned char _body[0xc4c - 4];
    CMemoryAllocatorFixedSize(void* heap, unsigned long minChunkSize, unsigned long maxAllocSize, unsigned long alignment,
                              unsigned long id, unsigned long flags);   // 0x00A65FF0
};
struct CMemoryAllocatorVariableSize : CMemoryAllocatorBase {
    unsigned char _body[0xc38 - 4];
    CMemoryAllocatorVariableSize(void* heap, long minChunkSize, void* base, long initialSize, unsigned long alignment,
                                 unsigned long id, unsigned long flags);   // 0x00A65930
};

struct CAllocatorInfo {
    long                  MaxAllocSize;     // +0x00
    long                  MinChunkSize;     // +0x04
    long                  InitialSize;      // +0x08
    long                  Alignment;        // +0x0c
    unsigned long         Flags;            // +0x10 allocator type index
    unsigned long         InternalFlags;    // +0x14 bit 1: fixed size
    CMemoryAllocatorBase* PAllocator;       // +0x18
    bool IsFixedSize() const { return ((InternalFlags >> 1) & 1) != 0; }
};
extern CAllocatorInfo g_AllocatorInfo[];   // 0x01396868

struct CAllocSizeToAllocator {
    CMemoryAllocatorBase* PAllocator;       // +0
    unsigned char         ID;               // +4
};

struct CMemoryAllocator {
    unsigned long          MemoryAllocatedPerTrackingID[256];   // +0x000
    unsigned long          MemoryAllocated;                     // +0x400
    void**                 MemoryAuditBuffer;                   // +0x404
    void*                  Heap;                                // +0x408
    unsigned long          NoAllocations;                       // +0x40c
    unsigned long          NoAllocators;                        // +0x410
    unsigned char          CurrentTrackingID;                   // +0x414
    bool                   AllocatorExists[15];                 // +0x415
    long                   MaxFixedAllocationSizes[15];         // +0x424
    CAllocSizeToAllocator* AllocSizeToAllocator[15];            // +0x460
    CMemoryAllocator(void* heap);
};

static __forceinline bool IsValidSize(__int64 size) { return size >= 0 && size <= 0x7fffffff; }

CMemoryAllocator::CMemoryAllocator(void* heap)
    : MemoryAllocated(0), NoAllocations(0), CurrentTrackingID(0)
{
    InitializeCriticalSection(g_allocatorLock);
    EnterCriticalSection(g_allocatorLock);
    InitializeMemorySystem();
    memset(MaxFixedAllocationSizes, 0, sizeof(MaxFixedAllocationSizes));
    memset(AllocSizeToAllocator, 0, sizeof(AllocSizeToAllocator));
    memset(AllocatorExists, 0, sizeof(AllocatorExists));
    memset(MemoryAllocatedPerTrackingID, 0, sizeof(MemoryAllocatedPerTrackingID));
    MemoryAllocated = 0;
    NoAllocators = 30;
    for (unsigned long i = 0; i < NoAllocators; ++i) {
        CAllocatorInfo& info = g_AllocatorInfo[i];
        if (info.MaxAllocSize == 0)
            continue;
        bool fixedSize = info.IsFixedSize();
        unsigned long type = info.Flags;
        AllocatorExists[type] = true;
        if (fixedSize) {
            info.PAllocator = new CMemoryAllocatorFixedSize(heap, info.MinChunkSize, info.MaxAllocSize, info.Alignment,
                                                            type, info.InternalFlags);
            info.MaxAllocSize = info.PAllocator->GetMaxAllocationSize(0);
            if (info.MaxAllocSize > MaxFixedAllocationSizes[type])
                MaxFixedAllocationSizes[type] = info.MaxAllocSize;
        } else {
            info.PAllocator = new CMemoryAllocatorVariableSize(heap, info.MinChunkSize, 0, info.InitialSize, info.Alignment,
                                                               type, info.InternalFlags);
        }
    }
    for (long type = 0; type < 15; ++type) {
        if (!AllocatorExists[type])
            continue;
        long maxSize = MaxFixedAllocationSizes[type];
        if (!IsValidSize(((__int64)maxSize + 2) * sizeof(CAllocSizeToAllocator)))
            return;
        AllocSizeToAllocator[type] = (CAllocSizeToAllocator*)malloc((maxSize + 2) * sizeof(CAllocSizeToAllocator));
        for (long size = 0; size <= maxSize + 1; ++size) {
            for (long a = 0; (unsigned long)a < NoAllocators; ++a) {
                CAllocatorInfo& info = g_AllocatorInfo[a];
                if (info.Flags != (unsigned long)type)
                    continue;
                if (info.IsFixedSize()) {
                    long minSize = 0;
                    if (a > 0)
                        minSize = g_AllocatorInfo[a - 1].MaxAllocSize + 1;
                    if (size < minSize || size > info.MaxAllocSize)
                        continue;
                }
                AllocSizeToAllocator[type][size].PAllocator = info.PAllocator;
                AllocSizeToAllocator[type][size].ID = (unsigned char)a;
                break;
            }
        }
    }
    MemoryAuditBuffer = 0;
    LeaveCriticalSection(g_allocatorLock);
}
