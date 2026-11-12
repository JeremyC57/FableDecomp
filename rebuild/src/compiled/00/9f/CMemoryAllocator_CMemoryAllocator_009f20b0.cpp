// CMemoryAllocator::CMemoryAllocator(void* heap) -- build the global allocator: create and take the allocator lock, initialise the memory system, clear the per-type tables, create a fixed-size or variable-size sub-allocator for every used record of the 30-record allocator-info table, then build each type's size -> allocator lookup array
// Leaf: ??0CMemoryAllocator@@IAE@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F20B0, length 598 bytes (0x256), `ret 4`, default /O2, no frame pointer.
// Callees: KERNEL32 InitializeCriticalSection / EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FEE4 / 0x0143FE0C /
//   0x0143FE10) on the allocator lock 0x013CAA70; InitializeMemorySystem 0x009F1D20 (__fastcall, no arguments);
//   CMemoryAllocatorBase::operator new 0x00A66730 (__fastcall size); CMemoryAllocatorFixedSize ctor 0x00A65FF0;
//   CMemoryAllocatorVariableSize ctor 0x00A65930; CMemoryAllocatorBase virtual slot 3 (+0xc) GetAllocationSize(void*);
//   __allmul 0x00BFEB50 (compiler helper for the 64-bit size product); malloc 0x00BFEA0E.
// Layout: engine/CMemoryAllocator.h with FABLE_RETAIL_CMEMORYALLOCATOR_CTOR (Ego_r.pdb storage, 0x49c bytes; CAllocatorInfo
//   0x1c bytes, table g_AllocatorInfo at 0x01396868: Flags +0x10 is the allocator type index 0..14, InternalFlags bit 1 =
//   fixed size; CAllocSizeToAllocator 8 bytes). The Heap member (+0x408) is never written by this constructor.
// Shape: MemoryAllocated is zeroed twice (initialiser list, then the body after the table clears). Loop 1 creates the
//   sub-allocators; a fixed-size one reports its real area size (GetAllocationSize(0)), which becomes the record's
//   MaxAllocSize and raises the type's MaxFixedAllocationSizes. Loop 2 allocates maxSize + 2 lookup entries per existing type
//   (the 64-bit byte count is range-checked; on failure the constructor returns without releasing the lock) and, for each
//   size, records the first allocator of that type that is variable-size or whose fixed range [previous max + 1, max] holds
//   the size. The fixed-size bit is read into a bool local in both loops (`shr ; and dl,1` / `shr ; test cl,1`).
//   The entry store + break is written out in BOTH arms (range match / variable size): VC7.1 merges the two tails
//   into one block after register allocation, and only with both arms present does `size` outrank `a` (size -> edx,
//   a -> ebx), which in turn splits the zero constant early so that it keeps ebx through loop 1 (heap, i and the fixed-size
//   flag stay in memory: the flag's store at [esp+0x13] is dead).
// Names: g_allocatorLock / g_AllocatorInfo are descriptive (no PDB name found; same spellings as the landed sibling rows).
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATORBASE_OPERATOR_NEW
#define FABLE_RETAIL_CMEMORYALLOCATORFIXEDSIZE_CLASS
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_CTOR
#define FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#include "engine/CMemoryAllocatorBase.h"
#include "engine/CMemoryAllocatorFixedSize.h"
#include "engine/CMemoryAllocatorVariableSize.h"
#include "engine/CMemoryAllocator.h"

extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);        // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);        // IAT 0x0143FE10
extern "C" void* __cdecl malloc(unsigned int size);                                         // 0x00BFEA0E
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)
extern unsigned char g_allocatorLock[24];   // 0x013CAA70
void __fastcall InitializeMemorySystem();   // 0x009F1D20

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
        bool fixedSize = ((info.InternalFlags >> 1) & 1) != 0;
        unsigned long type = info.Flags;
        AllocatorExists[type] = true;
        if (fixedSize) {
            info.PAllocator = new CMemoryAllocatorFixedSize(heap, info.MinChunkSize, info.MaxAllocSize, info.Alignment,
                                                            type, info.InternalFlags);
            info.MaxAllocSize = info.PAllocator->GetAllocationSize(0);
            if (info.MaxAllocSize > MaxFixedAllocationSizes[type])
                MaxFixedAllocationSizes[type] = info.MaxAllocSize;
        } else {
            info.PAllocator = new CMemoryAllocatorVariableSize(heap, info.MinChunkSize, 0, info.InitialSize, info.Alignment,
                                                               type, info.InternalFlags);
        }
    }
    for (long type = 0; type < 15; ++type) {
        long maxSize = MaxFixedAllocationSizes[type];
        if (!AllocatorExists[type])
            continue;
        __int64 tableBytes = ((__int64)maxSize + 2) * sizeof(CAllocSizeToAllocator);
        if (tableBytes < 0 || tableBytes > 0x7fffffff)
            return;
        AllocSizeToAllocator[type] = (CAllocSizeToAllocator*)malloc((maxSize + 2) * sizeof(CAllocSizeToAllocator));
        for (long size = 0; size <= maxSize + 1; ++size) {
            for (long a = 0; (unsigned long)a < NoAllocators; ++a) {
                CAllocatorInfo& info = g_AllocatorInfo[a];
                if (info.Flags != (unsigned long)type)
                    continue;
                bool fixedSize = ((info.InternalFlags >> 1) & 1) != 0;
                if (fixedSize) {
                    long minSize = 0;
                    if (a > 0)
                        minSize = g_AllocatorInfo[a - 1].MaxAllocSize + 1;
                    if (size >= minSize && size <= info.MaxAllocSize) {
                        AllocSizeToAllocator[type][size].PAllocator = info.PAllocator;
                        AllocSizeToAllocator[type][size].ID = (unsigned char)a;
                        break;
                    }
                } else {
                    AllocSizeToAllocator[type][size].PAllocator = info.PAllocator;
                    AllocSizeToAllocator[type][size].ID = (unsigned char)a;
                    break;
                }
            }
        }
    }
    MemoryAuditBuffer = 0;
    LeaveCriticalSection(g_allocatorLock);
}