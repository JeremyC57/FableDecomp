// CHandleBasedMemoryPool::CHandleBasedMemoryPool() -- empty pool: no buffer, zero counters, cleared free lists, empty handle array, defrag enabled with the default thresholds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02D20, length 129 bytes (+ int3 padding).
// Label correction: the catalog calls this CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo (propagated); the
// vtable it stores, 0x0129C964, is CHandleBasedMemoryPool's (rtti-vtables.tsv), and the PDB layout of that class
// matches every store. Its derived pools (CMovableResourceMemoryPool 0x00A62E20, CAsyncLoadingMemoryPool
// 0x00A148D0) call this body as their base constructor.
// Callees: 0x0099A2F0 CBase::CBase() (out of line: stores the root vtable 0x0129A7C4), 0x0099E4B0
// CCharString::CCharString() (the Name member).
// Layout: PDB CHandleBasedMemoryPool (0x468) with retail's 12-byte CArray (as the landed CPSCAttractor's
// C3DVectorArray), so FirstUnusedHandle/Name sit 4 bytes below the PDB offsets: Buffer +4, UnalignedBuffer +8,
// PoolSize +0xc, AllocatedSize +0x10, NumAllocations +0x14, LockCount +0x18, LargestFreeArea +0x1c,
// MemoryFlags +0x20, MultiThreaded +0x24, ShutdownFlag +0x25, EnableDefrag +0x26, EnableFullDefragIfPartialFails
// +0x27, MaxFreeSizeBeforeDefrag +0x28, MaxFreeSizeBeforeFailure +0x2c, MaxBlocksToTryOnPartialDefrag +0x30,
// FailureHandler +0x34, FreeLists[256] +0x38, Handles +0x438, FirstUnusedHandle +0x444, Name +0x448,
// CriticalSection +0x44c (the engine header carries the PDB offsets, so it is not used).
// Shape: base constructor call, vtable store, then the initialiser-list zeroes from one zero register (the
// CArray's inline constructor zeroes its three words), FirstUnusedHandle = -1, the Name constructor; the body
// clears the free lists with an intrinsic memset (`rep stosd`) and sets the defrag flags and thresholds.
// Leaf: ??0CHandleBasedMemoryPool
#include <string.h>

struct CBase {
    virtual ~CBase();
    CBase();                                             // 0x0099A2F0
};
class CCharString { public:
    void* m_pData;
    CCharString();                                       // 0x0099E4B0
};
struct CHandleInfo;
struct CHandleInfoArray {                                // retail CArray<CHandleBasedMemoryPool::CHandleInfo> (12 bytes)
    CHandleInfo* m_pData;
    long         m_size;
    long         m_capacity;
    CHandleInfoArray() : m_pData(0), m_size(0), m_capacity(0) { }
};
class CFailedAllocationHandler;
struct CHeader;
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };

class CHandleBasedMemoryPool : public CBase { public:                  // vtable 0x0129C964
    void*                     Buffer;                    // +0x04
    void*                     UnalignedBuffer;           // +0x08
    long                      PoolSize;                  // +0x0c
    long                      AllocatedSize;             // +0x10
    long                      NumAllocations;            // +0x14
    long                      LockCount;                 // +0x18
    long                      LargestFreeArea;           // +0x1c
    unsigned long             MemoryFlags;               // +0x20
    bool                      MultiThreaded;             // +0x24
    bool                      ShutdownFlag;              // +0x25
    bool                      EnableDefrag;              // +0x26
    bool                      EnableFullDefragIfPartialFails; // +0x27
    unsigned long             MaxFreeSizeBeforeDefrag;   // +0x28
    unsigned long             MaxFreeSizeBeforeFailure;  // +0x2c
    long                      MaxBlocksToTryOnPartialDefrag; // +0x30
    CFailedAllocationHandler* FailureHandler;            // +0x34
    CHeader*                  FreeLists[256];            // +0x38
    CHandleInfoArray          Handles;                   // +0x438
    long                      FirstUnusedHandle;         // +0x444
    CCharString               Name;                      // +0x448
    SCriticalSection          CriticalSection;           // +0x44c   sizeof == 0x464 (retail)
    CHandleBasedMemoryPool();
};

CHandleBasedMemoryPool::CHandleBasedMemoryPool()
    : Buffer(0)
    , UnalignedBuffer(0)
    , PoolSize(0)
    , AllocatedSize(0)
    , NumAllocations(0)
    , LockCount(0)
    , MultiThreaded(false)
    , ShutdownFlag(false)
    , FailureHandler(0)
    , FirstUnusedHandle(-1)
{
    memset(FreeLists, 0, sizeof(FreeLists));
    EnableDefrag = true;
    EnableFullDefragIfPartialFails = true;
    MaxFreeSizeBeforeDefrag = 0x80000;
    MaxFreeSizeBeforeFailure = 0x100000;
    MaxBlocksToTryOnPartialDefrag = 16;
}