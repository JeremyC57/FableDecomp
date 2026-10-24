// CHandleBasedMemoryPool::RemoveFreeArea(CHeader* area) -- unlink an area header from its free list (clear its status, splice the list through its back-link) and clear its list links
// Leaf: RemoveFreeArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27F50, 41 bytes (+ int3 padding).
// Counterpart of the landed CHandleBasedMemoryPool::InsertFreeArea @ 0x00C27EF0; called six times
// from the pool's allocate / defragment paths (0x00C27F8A, 0x00C27F96, 0x00C285AA, 0x00C28722,
// 0x00C2878B, 0x00C28BAF).  The manifest label Stack<unsigned long,64>::~Stack is propagated.
// CHandleBasedMemoryPool::CHeader (Ego_r.pdb, 32 bytes): Resource +0, Handle +4, Size +8, Padding
// +0xc, Status +0xe, Locked +0xf, RefAvailable +0x10 (the link that points at this header),
// NextAvailable +0x14, PrevContiguous +0x18, NextContiguous +0x1c.  `this` is unused (the pool
// members are not touched), but the function is a __fastcall member (`ret 4`, ecx free).
// Shape: `*RefAvailable = NextAvailable`, then the successor's back-link is repointed; the status
// byte store is scheduled first (one zero register, edx).
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPoolHeader {           // CHandleBasedMemoryPool::CHeader
    void* Resource;                             // +0x00
    long Handle;                                // +0x04
    long Size;                                  // +0x08
    short Padding;                              // +0x0c
    unsigned char Status;                       // +0x0e
    unsigned char Locked;                       // +0x0f
    CHandleBasedMemoryPoolHeader** RefAvailable;    // +0x10
    CHandleBasedMemoryPoolHeader* NextAvailable;    // +0x14
    CHandleBasedMemoryPoolHeader* PrevContiguous;   // +0x18
    CHandleBasedMemoryPoolHeader* NextContiguous;   // +0x1c
};

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void RemoveFreeArea(CHandleBasedMemoryPoolHeader* area);
};

void CHandleBasedMemoryPool_Methods::RemoveFreeArea(CHandleBasedMemoryPoolHeader* area)
{
    area->Status = 0;
    *area->RefAvailable = area->NextAvailable;
    if (area->NextAvailable)
        area->NextAvailable->RefAvailable = area->RefAvailable;
    area->RefAvailable = 0;
    area->NextAvailable = 0;
}