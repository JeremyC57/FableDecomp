// CHandleBasedMemoryPool::TestIfAreaIsGrowable(CHeader*,long) -- inspect contiguous movable areas on both sides.
// Retail 00A02020, 209 bytes. Slot2 CanMoveResource(Resource,Handle); PDB names retained.
// Donor header retained for named-field/layout evidence. Native has a 12-byte
// Handles array instead of donor's 16 bytes; tail shifts four bytes earlier.
#define CHandleBasedMemoryPool CHandleBasedMemoryPool_Donor
#include "engine/CHandleBasedMemoryPool.h"
#undef CHandleBasedMemoryPool
struct CHeader {
    void* Resource;
    fable_i32 Handle;
    fable_i32 Size;
    short Padding;
    unsigned char Status, Locked;
    CHeader** RefAvailable;
    CHeader* NextAvailable;
    CHeader* PrevContiguous;
    CHeader* NextContiguous;
};
struct CHandleArray { CHeader** first; CHeader** last; CHeader** end; };
class CHandleBasedMemoryPool { public:
    virtual ~CHandleBasedMemoryPool(); // slot0: 00A03AA0 deleting destructor
    virtual void MoveResource(void*,long,void*,void*,unsigned long); // slot1 00A02120
    virtual bool CanMoveResource(void*,long); // slot2 00A03A40
    virtual void ValidateResource(void*); // slot3 00A03A50
    void* Buffer; void* UnalignedBuffer;
    fable_i32 PoolSize,AllocatedSize,NumAllocations,LockCount,LargestFreeArea;
    unsigned int MemoryFlags;
    bool MultiThreaded,ShutdownFlag,EnableDefrag,EnableFullDefragIfPartialFails;
    unsigned int MaxFreeSizeBeforeDefrag,MaxFreeSizeBeforeFailure;
    fable_i32 MaxBlocksToTryOnPartialDefrag;
    CFailedAllocationHandler* FailureHandler;
    CHeader* FreeLists[256];
    CHandleArray Handles;
    fable_i32 FirstUnusedHandle;
    CCharStringData* Name_PStringData;
    _RTL_CRITICAL_SECTION_DEBUG* CriticalSection_DebugInfo;
    fable_i32 CriticalSection_LockCount,CriticalSection_RecursionCount;
    void* CriticalSection_OwningThread; void* CriticalSection_LockSemaphore;
    unsigned int CriticalSection_SpinCount;
    void InsertFreeArea(CHeader*); // 00A01DA0
    void CombineAreas(CHeader*,CHeader*); // 00A01E40
    CHeader* FindFreeArea(long); // 00A01ED0
    void ResizeArea(CHeader*,long); // 00A02410
    void UpdateLargestFreeArea(); // 00A01F80
    long InternalAllocateHandle(); // 00A03040
    bool InternalAllocate(long*,unsigned long);
    bool TestIfAreaIsGrowable(CHeader*,long);
    void Free(long);
    void RemoveFreeArea(CHeader* a) {
        if(a->RefAvailable && a->Status=='N') {
            a->Status=0;
            *a->RefAvailable=a->NextAvailable;
            if(a->NextAvailable) a->NextAvailable->RefAvailable=a->RefAvailable;
            a->RefAvailable=0; a->NextAvailable=0;
        }
    }
};
FABLE_STATIC_ASSERT(sizeof(CHeader)==0x20);
FABLE_STATIC_ASSERT(offsetof(CHeader,Resource)==0);
FABLE_STATIC_ASSERT(offsetof(CHeader,Handle)==4);
FABLE_STATIC_ASSERT(offsetof(CHeader,Status)==14);
FABLE_STATIC_ASSERT(offsetof(CHeader,PrevContiguous)==24);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,AllocatedSize)==0x10);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,Handles)==0x438);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,FirstUnusedHandle)==0x444);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,CriticalSection_DebugInfo)==0x44c);

bool CHandleBasedMemoryPool::TestIfAreaIsGrowable(CHeader* area,long size)
{
    if(area->Status!='N') return false;
    char* start=(char*)(area+1);
    char* end=start+(area->Size+area->Padding);
    if(end-start>=size) return true;
    CHeader* next=area->PrevContiguous;
    while(next) {
        if(next->Status!='N' && (next->Locked || !CanMoveResource(next->Resource,next->Handle))) break;
        start=(char*)(next+1);
        if(end-start>=size) return true;
        next=next->PrevContiguous;
    }
    next=area->NextContiguous;
    while(next) {
        if(next->Status!='N' && (next->Locked || !CanMoveResource(next->Resource,next->Handle))) break;
        if((char*)(next+1)+next->Size+next->Padding-start>=size) return true;
        next=next->NextContiguous;
    }
    return false;
}