// Behaviour test for CTemporaryBuffer::CTemporaryBuffer (0x00A98860): the three allocation routes in priority
// order -- an unlocked scratch buffer large enough is locked (ALLOCATE_SCRATCH), else a memory pool that
// grants a handle is locked and its pointer taken (ALLOCATE_POOL), else the heap (ALLOCATE_HEAP).
#include <cstdio>
#include <cstdlib>
static int g_newCalls = 0; static unsigned int g_newSize = 0; static unsigned char g_heap[64];
void* operator_new_array_spy(unsigned int size) { ++g_newCalls; g_newSize = size; return g_heap; }
#define new_array(size) ((unsigned char*)operator_new_array_spy(size))
struct CBaseClassNonCopyable { virtual void v00() {} CBaseClassNonCopyable() {} };
class CScratchBuffer { public:
    unsigned char _pad00[8];
    unsigned char* Buffer;
    bool           Locked;
    long           NoBytes;
    int            lockCalls;
    bool Lock(void** ppBuffer) { ++lockCalls; Locked = true; *ppBuffer = Buffer; return true; }
};
class CHandleBasedMemoryPool { public:
    bool grant; int allocCalls; int lockCalls; long lockedHandle; unsigned long allocSize; unsigned long allocFlags; unsigned char poolMem[32];
    bool  Allocate(long* handle, unsigned long size, unsigned long flags) { ++allocCalls; allocSize = size; allocFlags = flags; if (!grant) return false; *handle = 0x1234; return true; }
    void  Lock(long handle) { ++lockCalls; lockedHandle = handle; }
    void* GetPointerFromHandle(long handle) { return handle == 0x1234 ? poolMem : 0; }
};
struct CTemporaryBuffer : CBaseClassNonCopyable {
    enum EAllocateType { ALLOCATE_POOL = 0, ALLOCATE_SCRATCH = 1, ALLOCATE_HEAP = 2 };
    void*                   Data;
    CHandleBasedMemoryPool* MemoryPool;
    long                    MemoryHandle;
    CScratchBuffer*         ScratchBuffer;
    EAllocateType           AllocateType;
    virtual void v00() {}
    CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch);
};
CTemporaryBuffer::CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch)
    : Data(0), MemoryPool(pool), ScratchBuffer(scratch)
{
    if (scratch && size <= (unsigned long)scratch->NoBytes && !scratch->Locked) {
        scratch->Lock(&Data);
        AllocateType = ALLOCATE_SCRATCH;
    } else if (pool && pool->Allocate(&MemoryHandle, size, 0)) {
        MemoryPool->Lock(MemoryHandle);
        Data = MemoryPool->GetPointerFromHandle(MemoryHandle);
        AllocateType = ALLOCATE_POOL;
    } else {
        Data = new_array(size);
        AllocateType = ALLOCATE_HEAP;
    }
}
static int fail(const char* why) { std::printf("00a98860_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static unsigned char scratchMem[16];
    CScratchBuffer scratch; scratch.Buffer = scratchMem; scratch.Locked = false; scratch.NoBytes = 16; scratch.lockCalls = 0;
    CHandleBasedMemoryPool pool; pool.grant = true; pool.allocCalls = 0; pool.lockCalls = 0; pool.lockedHandle = 0;
    { // 1. scratch fits and is free
        CTemporaryBuffer b(16, &pool, &scratch);
        if (b.AllocateType != CTemporaryBuffer::ALLOCATE_SCRATCH || b.Data != scratchMem || scratch.lockCalls != 1 || !scratch.Locked || pool.allocCalls != 0 || g_newCalls != 0) return fail("scratch route");
        if (b.MemoryPool != &pool || b.ScratchBuffer != &scratch) return fail("members");
    }
    { // 2. scratch already locked: pool route
        CTemporaryBuffer b(16, &pool, &scratch);
        if (b.AllocateType != CTemporaryBuffer::ALLOCATE_POOL || b.Data != pool.poolMem || b.MemoryHandle != 0x1234 || pool.allocCalls != 1 || pool.allocSize != 16 || pool.allocFlags != 0 || pool.lockCalls != 1 || pool.lockedHandle != 0x1234 || scratch.lockCalls != 1 || g_newCalls != 0) return fail("pool route");
    }
    scratch.Locked = false;
    { // 3. too big for the scratch, pool refuses: heap
        pool.grant = false;
        CTemporaryBuffer b(17, &pool, &scratch);
        if (b.AllocateType != CTemporaryBuffer::ALLOCATE_HEAP || b.Data != g_heap || g_newCalls != 1 || g_newSize != 17 || pool.allocCalls != 2 || pool.lockCalls != 1 || scratch.lockCalls != 1) return fail("heap route");
    }
    { // 4. no scratch, no pool
        CTemporaryBuffer b(5, 0, 0);
        if (b.AllocateType != CTemporaryBuffer::ALLOCATE_HEAP || g_newCalls != 2 || g_newSize != 5 || b.MemoryPool != 0 || b.ScratchBuffer != 0) return fail("null providers");
    }
    { // 5. exact fit on an unlocked scratch (size == NoBytes) is still the scratch
        CTemporaryBuffer b(16, 0, &scratch);
        if (b.AllocateType != CTemporaryBuffer::ALLOCATE_SCRATCH || scratch.lockCalls != 2) return fail("exact fit");
    }
    std::printf("00a98860_TEST PASS\n");
    return 0;
}