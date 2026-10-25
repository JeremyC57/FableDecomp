// CHandleBasedMemoryPool::Initialise(long poolSize, long maxBlocksToTry) -- drop any previous buffer, allocate the pool plus 16 bytes, align the buffer to 16 and make the whole pool one free area
// Leaf: ?Initialise@CHandleBasedMemoryPool_Methods@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28240, 73 bytes (`ret 8`). The libpk build of the engine's
//   CHandleBasedMemoryPool::Initialise (engine copy 0x00A02570 takes (size, multiThreaded, memoryFlags) and inlines the
//   free-area setup); the manifest's "CHandleBasedMemoryPool_Initialize" is descriptive.
// Callees: ReleaseBuffer @ 0x00C27E60 (landed), the audio allocator's alloc @ 0x00C16670 (`__fastcall`, size in ecx;
//   ReleaseBuffer frees through its partner 0x00C16680), CreateArea @ 0x00C27EA0, InsertFreeArea @ 0x00C27EF0.
// Shape: the second argument goes to +0x30 (MaxBlocksToTryOnPartialDefrag in engine/CHandleBasedMemoryPool.h); with
//   the size assigned FIRST VC7.1 loads both arguments up front (size eax, maximum ecx) and schedules the +0x30 store
//   ahead of the +0x0C one (the maximum assigned first loads it alone through eax); the allocation size comes from
//   the argument register, PoolSize is re-read (edx) for CreateArea; the alignment is `(p + 15) / 16 * 16`
//   (`add eax,0Fh ; shr eax,4 ; shl eax,4`).
struct CHeader {                                // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area (landed 0x00C27F50 names)
    void*          Resource;                    // +0x00
    long           Handle;                      // +0x04
    long           Size;                        // +0x08
    short          Padding;                     // +0x0c
    unsigned char  Status;                      // +0x0e ('N' free)
    unsigned char  Locked;                      // +0x0f
    CHeader**      RefAvailable;                // +0x10 (the free-list link that points at this area)
    CHeader*       NextAvailable;               // +0x14
    CHeader*       PrevContiguous;              // +0x18
    CHeader*       NextContiguous;              // +0x1c
};
#include "engine/CHandleBasedMemoryPool.h"

void* __fastcall PoolAlloc(unsigned long size);   // 0x00C16670

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void     ReleaseBuffer();                   // 0x00C27E60
    CHeader* CreateArea(void* address, unsigned long size, CHeader* prev, CHeader* next);   // 0x00C27EA0
    void     InsertFreeArea(CHeader* area);    // 0x00C27EF0
    void     Initialise(long poolSize, long maxBlocksToTry);
};

void CHandleBasedMemoryPool_Methods::Initialise(long poolSize, long maxBlocksToTry)
{
    ReleaseBuffer();
    PoolSize = poolSize;
    MaxBlocksToTryOnPartialDefrag = maxBlocksToTry;
    UnalignedBuffer = PoolAlloc(poolSize + 16);
    Buffer = (void*)(((unsigned long)UnalignedBuffer + 15) / 16 * 16);
    InsertFreeArea(CreateArea(Buffer, PoolSize, 0, 0));
}