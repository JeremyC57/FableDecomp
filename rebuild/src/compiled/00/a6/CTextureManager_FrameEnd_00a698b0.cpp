// CTextureManager::FrameEnd() -- ends the frame on every preallocated texture pool, then clears the per-pool failed-allocation counters
// Retail: Fable.exe @ 0x00A698B0, 56 bytes (+ int3 padding). Callee: CPreallocTexturePool::FrameEnd @ 0x009FC150
// (manifest label "CResourceList::FrameEnd" -- a member called on each 0x54-byte pool).
// Layout: the retail CTextureManager is 0x40 bytes shorter than the PDB header (engine/CTextureManager.h): the pool
// stride is 0x54 (header 0x58), so FailedAllocations is at +0x58c (header +0x5cc). Modelled locally with the real names.
// Shape: a counted loop over PreallocPools (pointer strength-reduced in ebx, PoolCount re-read every pass because the
// call may change it), then a zero-fill loop over FailedAllocations[0..PoolCount) that VC7.1 turns into `rep stosd`.
// The fill loop needs the count AND the array pointer in named locals: with `FailedAllocations[i] = 0` against the
// member count the stores alias PoolCount (a real loop re-reading it); with only the count local the `lea edi` is
// scheduled after the `jbe` guard instead of between `test ecx,ecx` and `jbe` as retail.
struct CPreallocTexturePool { unsigned char _body[0x54]; void FrameEnd(); };   // 0x009FC150
class CTextureManager { public:
    void*                __vftable;                    // +0x00
    bool                 Initialised;                  // +0x04
    unsigned long        PoolCount;                    // +0x08
    unsigned long        PreallocSizes[16];            // +0x0c
    CPreallocTexturePool PreallocPools[16];            // +0x4c (retail stride 0x54)
    unsigned long        FailedAllocations[16];        // +0x58c
    long                 BufferSize;                   // +0x5cc
    void*                TextureBuffer;                // +0x5d0
    void FrameEnd();
};

void CTextureManager::FrameEnd()
{
    unsigned long i;
    for (i = 0; i < PoolCount; ++i)
        PreallocPools[i].FrameEnd();
    unsigned long count = PoolCount;
    unsigned long* failed = FailedAllocations;
    for (i = 0; i < count; ++i)
        failed[i] = 0;
}