// CSimpleMemoryPool::ClearMemoryBlocks() -- deletes the block chain: every block when the first one owns its data, only the first (external buffer) otherwise; then no blocks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C600, length 54 bytes.
// Leaf: ClearMemoryBlocks
// Callees: CMemoryBlock's virtual scalar deleting destructor (slot 0, `push 1 ; call [eax]`; landed at 0x00A5C9B0).
// Layout: engine/CSimpleMemoryPool.h (FirstMemoryBlock +4); CMemoryBlock as the landed 0x00A5C670 / 0x00A5C6E0 bodies and
// the PDB (BlockData +8, OwnsData +0xc, BlockSize +0x10, NextBlock +0x14), polymorphic, so modelled locally.
// Shape: one `FirstMemoryBlock = 0` written after the if/else; VC7.1 tail-duplicates it into both arms (the loop arm
// stores the null loop pointer from ecx, the else arm an immediate) -- inlined into Reset 0x00A5C790 the same source
// keeps one shared store. The loop is a do/while on a local with the successor read before the delete.
#include "engine/CSimpleMemoryPool.h"

struct CMemoryBlock {                           // vtable 0x0129D568 (CSimpleMemoryPool::CMemoryBlock)
    virtual ~CMemoryBlock();
    unsigned long  IVRefCount;                  // +0x04
    unsigned char* BlockData;                   // +0x08
    bool           OwnsData;                    // +0x0c
    unsigned long  BlockSize;                   // +0x10
    CMemoryBlock*  NextBlock;                   // +0x14
};

struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    void ClearMemoryBlocks();
};

void CSimpleMemoryPool_Methods::ClearMemoryBlocks()
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