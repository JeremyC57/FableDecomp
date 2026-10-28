// CMemoryAllocator::GetHeaderOverhead() -- Sum allocator statistics under the allocator lock.
// Leaf: ?GetHeaderOverhead@CMemoryAllocator@@QAEKXZ
// Identity: Own allocator virtual slot12; VariableSize/FixedSize vtables129db1c/129db64 and original PDB GetHeaderOverhead API. Table and owner layout independently supported by destructor009f2310; NoAllocators+1040. Do not reuse mislabeled neighboring sum names.
#define CMemoryAllocatorBase CMemoryAllocatorBase_HeaderLayout
#include "engine/CMemoryAllocatorBase.h"
#undef CMemoryAllocatorBase

#ifndef _MSC_VER
#define __stdcall
#define __declspec(x)
#endif
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
extern unsigned char g_allocatorLock[24];
// Layout: active allocator-base header is flattened. Restore real vptr; only getter slots6/8/12 are used. UnusedSlot names denote unmodeled entries, not recovered function names. Original PerformanceStats+4, Alignment+3092, AlignmentShift+3096 retained.
struct CMemoryAllocatorBase {
virtual unsigned long UnusedSlot00(){return 0;}
virtual unsigned long UnusedSlot01(){return 0;}
virtual unsigned long UnusedSlot02(){return 0;}
virtual unsigned long UnusedSlot03(){return 0;}
virtual unsigned long UnusedSlot04(){return 0;}
virtual unsigned long UnusedSlot05(){return 0;}
virtual unsigned long GetTotalMemoryControlledByAllocator(){return 0;}
virtual unsigned long UnusedSlot07(){return 0;}
virtual unsigned long GetNoFreeAreas(){return 0;}
virtual unsigned long UnusedSlot09(){return 0;}
virtual unsigned long UnusedSlot10(){return 0;}
virtual unsigned long UnusedSlot11(){return 0;}
virtual unsigned long GetHeaderOverhead(){return 0;}
unsigned char PerformanceStats[0xc10];unsigned long Alignment,AlignmentShift;
};
struct CAllocatorInfo {long MaxAllocSize,MinChunkSize,InitialSize,Alignment;unsigned long Flags,InternalFlags;CMemoryAllocatorBase* PAllocator;};
// Layout: existing table g_AllocatorInfo01396868 has original PAllocator at+24, stride28; original global name unproven. Critical-section name is an established descriptive binding.
extern CAllocatorInfo g_AllocatorInfo[];
class CMemoryAllocator {public:
 unsigned long MemoryAllocatedPerTrackingID[256],MemoryAllocated;void** MemoryAuditBuffer;void* Heap;unsigned long NoAllocations,NoAllocators;
 unsigned long GetTotalMemoryControlledByAllocator();unsigned long GetNoFreeAreas();unsigned long GetHeaderOverhead();
};
unsigned long CMemoryAllocator::GetHeaderOverhead(){unsigned long total=0;EnterCriticalSection(g_allocatorLock);for(unsigned long i=0;i<NoAllocators;++i)total+=g_AllocatorInfo[i].PAllocator->GetHeaderOverhead();LeaveCriticalSection(g_allocatorLock);return total;}