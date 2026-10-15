// CSimpleMemoryPool::CalculateMemoryAllocatedByClient() -- bytes handed out: the total block size (CalculateMemoryAllocatedByPool inlined) minus every free area's size plus its 16-byte header
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C850, length 57 bytes.
// Leaf: CalculateMemoryAllocatedByClient
// Callees: none (the landed CalculateMemoryAllocatedByPool 0x00A5C5E0 is inlined).
// Layout: engine/CSimpleMemoryPool.h (FirstMemoryBlock +4, Root (CArea, opaque in the header) +0x10 with NextFreeArea at
// +0x14); CMemoryBlock BlockSize +0x10, NextBlock +0x14; CArea Size +0, NextFreeArea +4.
// Shape: the block walk with an aligned loop head (`lea ebx,[ebx]` filler), the total moved to eax for the free-list
// walk, each area subtracted as `eax += -16 - size` (`mov edx,-10h ; sub edx,esi ; add eax,edx`).
#include "engine/CSimpleMemoryPool.h"

struct CMemoryBlock {                           // CSimpleMemoryPool::CMemoryBlock (vtable 0x0129D568)
    unsigned char  _head[0x10];                 // vptr, IVRefCount, BlockData, OwnsData
    unsigned long  BlockSize;                   // +0x10
    CMemoryBlock*  NextBlock;                   // +0x14
};

struct CArea {                                  // CSimpleMemoryPool::Root and the free areas
    unsigned long Size;                         // +0x00
    CArea*        NextFreeArea;                 // +0x04
    long          Status;                       // +0x08 (EAreaStatus)
};

struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    CArea& RootArea() { return *reinterpret_cast<CArea*>(Root); }
    unsigned long CalculateMemoryAllocatedByPool()      // out of line at 0x00A5C5E0
    {
        unsigned long total = 0;
        for (CMemoryBlock* block = FirstMemoryBlock; block != 0; block = block->NextBlock)
            total += block->BlockSize;
        return total;
    }
    unsigned long CalculateMemoryAllocatedByClient();
};

unsigned long CSimpleMemoryPool_Methods::CalculateMemoryAllocatedByClient()
{
    unsigned long total = CalculateMemoryAllocatedByPool();
    for (CArea* area = RootArea().NextFreeArea; area != 0; area = area->NextFreeArea)
        total -= area->Size + 16;
    return total;
}