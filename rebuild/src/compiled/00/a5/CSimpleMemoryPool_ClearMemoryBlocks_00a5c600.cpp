// CSimpleMemoryPool::ClearMemoryBlocks() -- deletes the block chain: every block when the first one owns its data, only the first (external buffer) otherwise; then no blocks
// Leaf: ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C600, length 54 bytes.
// Identity (proven, chunk c673): called by the CSimpleMemoryPool scalar deleting dtor 0x00987F30 (vtable 0x0129A064 store, call,
//   ~CBaseClassNonCopyable) and inline in ~CEngine 0x00B261D0; Ego_r + FableWin list only the private
//   ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ.
// Callees: CMemoryBlock's virtual scalar deleting destructor (slot 0, `push 1 ; call [eax]`; landed at 0x00A5C9B0).
// Layout: Ego_r.pdb CSimpleMemoryPool (FirstMemoryBlock +4, Capacity +8, GrowthGranularity +0xc, Root +0x10); CMemoryBlock as the
//   landed 0x00A5C670 / 0x00A5C6E0 bodies and the PDB (BlockData +8, OwnsData +0xc, BlockSize +0x10, NextBlock +0x14),
//   polymorphic, so modelled locally. engine/CSimpleMemoryPool.h is the flat layout and cannot carry the private member
//   (the landed CSimpleMemoryPool_Methods view derived from it named a class retail does not have), so the class is modelled
//   here with the header's members.
// Shape: one `FirstMemoryBlock = 0` written after the if/else; VC7.1 tail-duplicates it into both arms (the loop arm
// stores the null loop pointer from ecx, the else arm an immediate) -- inlined into Reset 0x00A5C790 the same source
// keeps one shared store. The loop is a do/while on a local with the successor read before the delete.
// Naming wave 16: landed as CSimpleMemoryPool_Methods::ClearMemoryBlocks (public, a view class derived from the header);
//   respelled on the PDB private member (name-only: the bytes are unchanged).
struct CMemoryBlock {                           // vtable 0x0129D568 (CSimpleMemoryPool::CMemoryBlock)
    virtual ~CMemoryBlock();
    unsigned long  IVRefCount;                  // +0x04
    unsigned char* BlockData;                   // +0x08
    bool           OwnsData;                    // +0x0c
    unsigned long  BlockSize;                   // +0x10
    CMemoryBlock*  NextBlock;                   // +0x14
};
struct CArea { unsigned long Size; CArea* NextFreeArea; long Status; };

class CSimpleMemoryPool {
public:
    void*         __vftable;                    // +0x00 (the CBaseClassNonCopyable vptr; flat, as engine/CSimpleMemoryPool.h)
    CMemoryBlock* FirstMemoryBlock;             // +0x04
    unsigned long Capacity;                     // +0x08
    unsigned long GrowthGranularity;            // +0x0c
    CArea         Root;                         // +0x10
private:
    void ClearMemoryBlocks();
};

void CSimpleMemoryPool::ClearMemoryBlocks()
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
