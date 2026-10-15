// CSimpleMemoryPool::Reset() -- deletes the block chain (ClearMemoryBlocks inlined), empties the free-area list and restores the default 0x400 capacity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C790, length 65 bytes.
// Leaf: Reset
// Callees: CMemoryBlock's virtual scalar deleting destructor (slot 0, `push 1 ; call [eax]`; landed at 0x00A5C9B0).
// Layout: engine/CSimpleMemoryPool.h (FirstMemoryBlock +4, Capacity +8, Root (CArea, opaque in the header) +0x10: its
// NextFreeArea at +0x14 is reached through a CArea view of the member); CMemoryBlock modelled locally (polymorphic).
// Shape: the inlined ClearMemoryBlocks keeps ONE `FirstMemoryBlock = 0` store shared by both arms (the loop arm jumps
// to it; out of line, 0x00A5C600 tail-duplicates it), then `Root.NextFreeArea = 0` and `Capacity = 0x400`.
#include "engine/CSimpleMemoryPool.h"

struct CMemoryBlock {                           // vtable 0x0129D568 (CSimpleMemoryPool::CMemoryBlock)
    virtual ~CMemoryBlock();
    unsigned long  IVRefCount;                  // +0x04
    unsigned char* BlockData;                   // +0x08
    bool           OwnsData;                    // +0x0c
    unsigned long  BlockSize;                   // +0x10
    CMemoryBlock*  NextBlock;                   // +0x14
};

struct CArea {                                  // CSimpleMemoryPool::Root
    unsigned long Size;                         // +0x00
    CArea*        NextFreeArea;                 // +0x04
    long          Status;                       // +0x08 (EAreaStatus)
};

struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    CArea& RootArea() { return *reinterpret_cast<CArea*>(Root); }
    void ClearMemoryBlocks()                    // out of line at 0x00A5C600
    {
        CMemoryBlock* block = FirstMemoryBlock;
        if (block) {
            if (block->OwnsData) {
                do {
                    CMemoryBlock* next = block->NextBlock;
                    delete block;
                    block = next;
                } while (block);
            } else {
                delete block;
            }
            FirstMemoryBlock = 0;
        }
    }
    void Reset();
};

void CSimpleMemoryPool_Methods::Reset()
{
    ClearMemoryBlocks();
    RootArea().NextFreeArea = 0;
    Capacity = 0x400;
}