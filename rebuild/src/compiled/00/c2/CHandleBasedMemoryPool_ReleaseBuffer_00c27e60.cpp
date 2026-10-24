// CHandleBasedMemoryPool::ReleaseBuffer() -- free the pool's unaligned buffer (if any) and clear the buffer/size words, then reset the allocation statistics
// Leaf: ReleaseBuffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27E60, 42 bytes.
// Callers: CHandleBasedMemoryPool::Initialize @ 0x00C28240 (first thing it does, before allocating a
// new buffer) and ~CHandleBasedMemoryPool @ 0x00C287E0 (landed; it calls this as its out-of-line
// "Cleanup").  The manifest label Stack<unsigned long,64>::~Stack is propagated; the PDB keeps no
// name for this helper, so it is named for what it does.
// Callee: 0x00C16680, the engine allocator's free (__fastcall, block in ecx: it forwards to the
// global allocator interface's slot 2); its partner 0x00C16670 allocates the buffer in Initialize.
// Layout: engine/CHandleBasedMemoryPool.h (Ego_r.pdb): Buffer +0x04, UnalignedBuffer +0x08,
// PoolSize +0x0c, AllocatedSize +0x10, NumAllocations +0x14, LockCount +0x18, LargestFreeArea +0x1c.
// Shape: the aligned/unaligned pointers and the size are cleared only when a buffer was freed; the
// four statistics are cleared unconditionally (one zero register, edi).
#include "engine/CHandleBasedMemoryPool.h"

void __fastcall PoolFree(void* block);      // 0x00C16680

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void ReleaseBuffer();
};

void CHandleBasedMemoryPool_Methods::ReleaseBuffer()
{
    if (this->UnalignedBuffer) {
        PoolFree(this->UnalignedBuffer);
        this->UnalignedBuffer = 0;
        this->Buffer = 0;
        this->PoolSize = 0;
    }
    this->AllocatedSize = 0;
    this->NumAllocations = 0;
    this->LockCount = 0;
    this->LargestFreeArea = 0;
}